var net = require('net');
var DanmuServer = net.createServer();

var http = require('http');
var wechat = require('node-wechat');
var crypto = require('crypto');

//数据库
var mysql = require('mysql');
var connection = mysql.createConnection({
	host:'localhost',
	user:'root',
	password:'541536638yy.',
	database:'ydanmu_system',
	port:'3306'
});
//连接数据库
connection.connect(function(err){
		if(err){
			console.log('[query] - :' + err);
			return;
		}else{
			console.log('[Database connection connect] succeed!');
		}
});

const EventEmitter = require('events');
class MyEmitter extends EventEmitter{}
const myEmitter = new MyEmitter();

DanmuServer.on('connection',function(client){
	//获取客户端的IP地址和端口号
	var remoteAddress = client.remoteAddress;
	var remotePort = client.remotePort;
	var remoteIpPort = remoteAddress +":"+ remotePort;
	var username;
	console.log('New connection:'+remoteIpPort);        
	
	client.on('data',function(data){
		username = data;
	});
	client.on('end',function(){
		console.log("Connection end:"+remoteIpPort);
		client.end();
		client.destroy();
	});
	myEmitter.on('event',function(danmu,teacher){
		if(username == teacher){
			client.write(danmu);
		}
	});
});
DanmuServer.on('error',function(err){
	console.log('[err]' + err);
});
DanmuServer.listen(23333,function(){
	console.log("Client server running at http://127.0.0.1:23333");
});

wechat.token = "weixin";
http.createServer(function(req,res){
	wechat.checkSignature(req,res);
	wechat.handler(req,res);
	wechat.event(function(events){
		if(events.Event == 'subscribe'){
			var sb_come = {
				FromUserName : events.ToUserName,
				ToUserName : events.FromUserName,
				Content : '欢迎关注“Y弹幕”。回复“发送弹幕-老师用户名”选择老师。'
			}
			wechat.send(sb_come);
		}
		if(events.Event == 'unsubscribe'){
			//如果这个用户在student_teacher表中，则将他删除
			res.writeHead(200, {'Content-Type': 'text/plain'});
			res.end();
			var check_student_teacher = 'DELETE FROM student_teacher where student_ID = ?';
			var stu_name = events.FromUserName;
			connection.query(check_student_teacher,stu_name,function(err){
				if(err){
					console.log('[Mysql delete]:' + err);
				}
			})
		}
	});
	wechat.text(function(data){
		var str = '发送弹幕-';
		var wechat_content = data.Content;
		var strStart = wechat_content.indexOf(str);
		if(strStart == 0){		//将以“发送弹幕+”开头的字符串都拦截下来
			start_send_danmu(data);
		}else if(wechat_content == '结束发送弹幕'){  //如果收到的消息是结束发送弹幕
			end_send_danmu(data);
		}else{
			send_danmu(data);
		}
	});
}).listen(80,function(){
	console.log("Wechat server is listening 80 port");
});

function start_send_danmu(data){
	/*-------------------截取出用户老师的用户名-------------------*/
	var start_str = data.Content;
	var flag_pos = 5;  
	var teacher_name = start_str.substring(flag_pos);
	var student_query = 'SELECT * FROM student_teacher where student_ID = ?';
	var student_ID = data.FromUserName;
	connection.query(student_query,student_ID,function(err,res){
		if(err){
			console.log('[Mysql select]' + err);
			return;
		}else{
			if(res != ''){   //如果之前已经选择了老师了
				var have_teacher = {
					FromUserName : data.ToUserName,
					ToUserName : data.FromUserName,
					Content : '对不起，你现在已经选择了用户名为“' + res[0].teacher_ID + '”的老师，请输入“结束发送弹幕”后再重新选择老师。'
				}
				wechat.send(have_teacher);
			}else{			//如果之前没选择过老师
				//查看是否存在这个老师
				var teacher_query = 'SELECT * FROM teacher_info where userName = ?';
				connection.query(teacher_query,teacher_name,function(err,res){
					if(err){
						console.log('[Mysql select]' + err);
					}else{
						if(res == ''){     //假如输入的老师名字不存在
							var no_teacher = {
								FromUserName : data.ToUserName,
								ToUserName : data.FromUserName,
								Content : '对不起，没有用户名为“' + teacher_name + '”的老师。'
							}
							wechat.send(no_teacher);
						}else{  //这个老师是存在的
							select_from_blacklist(student_ID,teacher_name,data,function(flag){
								if(flag){
									var input_teacher = {
									FromUserName : data.ToUserName,
									ToUserName : student_ID,
									Content : '成功选择老师，现在可以发送弹幕了，回复“结束发送弹幕”停止发送。'
									}
									wechat.send(input_teacher);	
									var stu_teacher_query = 'INSERT INTO student_teacher(student_ID,teacher_ID) VALUES(?,?)';
									var stu_teacher = [student_ID,teacher_name];
									connection.query(stu_teacher_query,stu_teacher,function(err){
										if(err){
											console.log('[Mysql insert]' + err);
											return;
										}
									});
								}
							});
						}	
					}
				});
			}
		}
	});	
};

function end_send_danmu(data){
	var del = 'DELETE FROM student_teacher where student_ID = ?';
	var student_name = data.FromUserName;
	connection.query(del,student_name,function(err,res){
		if(err){
			console.log('[Mysql delete]' + err);
		}else{
			var	end_danmu = {
				FromUserName : data.ToUserName,
				ToUserName : data.FromUserName,
				Content : '成功结束，回复“发送弹幕-老师用户名”选择老师。'
			}
			wechat.send(end_danmu);
		}
	});
};

function send_danmu(data){
	var stu_tea_query = 'SELECT * FROM student_teacher where student_ID = ?';
	var student = data.FromUserName;
	connection.query(stu_tea_query,student,function(err,res){
		if(err){
			console.log('[Mysql select]' + err);
		}else{
			if(res == ''){
				var msg = {
					FromUserName : data.ToUserName,
					ToUserName : data.FromUserName,
					Content : '你当前没有选择任何老师，回复“发送弹幕-老师用户名”选择老师。'
				}
				wechat.send(msg);
			}else{ 
				//获取老师名字
				var now_teacher = res[0].teacher_ID;
				select_from_blacklist(data.FromUserName,now_teacher,data,function(flag){
					if(flag){
						var danmu_ok = {			
							FromUserName : data.ToUserName,
							ToUserName : data.FromUserName,
							Content : '弹幕成功发送，回复“结束发送弹幕”不再继续发送弹幕。'
						}
						wechat.send(danmu_ok);
						var str1 = 'INSERT INTO {table}(userID,content) values (?,?)';
						var re = /{table}/;
						var table_name = now_teacher + '_danmulist';
						var Messages_insert = str1.replace(re,table_name);
						var Addpara = [data.FromUserName,data.Content];
						connection.query(Messages_insert,Addpara,function(err,res){
							if(err){
								console.log('[Mysql insert]' + err);
							}else{
								myEmitter.emit('event',data.Content,now_teacher,function(err){
									if(err){
										console.log("发送失败");
									}
								});
							}
						});
					}
				});
			}
		}
	});
};

function select_from_blacklist(studentName,teacherName,data,callback){
	var str = 'SELECT * FROM {blacklist} where b_userID = ?';
	var re = /{blacklist}/;
	var blacklistName = teacherName + '_blacklist';
	var select_query = str.replace(re,blacklistName);
	connection.query(select_query,studentName,function(err,res){
		if(err){
			console.log('[Mysql select]' + err);
		}else{
			if(res != ''){  //如果被拉黑了
				var in_blacklist = {
					FromUserName : data.ToUserName,
					ToUserName : data.FromUserName,
					Content : '无法发送弹幕，你已经被用户名为“'+ teacherName +'”的老师禁言，回复“结束发送弹幕”选择其他老师'
				}
				wechat.send(in_blacklist);
				callback(false);
			}else{
				callback(true);
			}
		}
	});
}

//服务器：root 64578537yy.

