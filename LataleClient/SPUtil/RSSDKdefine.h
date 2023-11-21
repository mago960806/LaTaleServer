#define PACK_MAX 20480

#define RETSZ_NET_UNINIT			"网络配置尚未初始化，请调用SetNetParam函数设置服务器IP和端口。"
#define RETSZ_NET_SENDFAIL			"发送信息失败，错误码为："
#define RETSZ_NET_RECVTIMEOUT		"接收超时。"
#define RETSZ_NET_RECVERR			"服务器不存在，接收失败。"
#define RETN_NET_UNINIT				0x0100		//Please call “SetNetParam” first
#define RETN_NET_SENDFAIL			0x0101		//Send failed
#define RETN_NET_RECVTIMEOUT		0x0102		//Receive message time out
#define RETN_NET_RECVERR			0x0103		//Server not exist

#define	RETSZ_PARAM_ERR				"输入参数错误。"
#define RETSZ_PARAM_COMBINEFAIL		"输入参数组合失败。"
#define RETSZ_PARAM_ANALYZEFAIL		"接受的content解析失败。"
#define RETSZ_PARAM_OVERFLOW		"字节数过长溢出可用的输出空间，请开设更大的接受缓冲区。"
#define	RETN_PARAM_ERR				0x0200		//Parameters is empty or wrong value     
#define RETN_PARAM_COMBINEFAIL		0x0201		//Combine request packet failed
#define RETN_PARAM_ANALYZEFAIL		0x0202		//Analyze receive packet failed
#define RETN_PARAM_OVERFLOW			0x0203		//szRetContent is smaller than needed

#define	RETSZ_PARAM_NULL			"输入参数错误。"
#define	RETSZ_PARAM_EMPTY			"输入参数为空。"
#define	RETSZ_PARAM_2LONG			"输入参数过长。"
#define	RETSZ_PARAM_IDTYPE			"IDTYPE错误。"
#define	RETSZ_PARAM_POINT			"评价分数错误。"
#define	RETN_PARAM_NULL				0x0211		//point is NULL		
#define	RETN_PARAM_EMPTY			0x0212		//“”
#define	RETN_PARAM_2LONG			0x0213		//too long
#define	RETN_PARAM_IDTYPE			0x0214		//idtype is not 0,1 or 2
#define	RETN_PARAM_POINT			0x0215		//point is not one of 1,2,3

#define RETSZ_RECV_PARAMERR			"返回参数非法"
#define RETN_RECV_PARAMERR			0x0300		//Return value wrong

#define	RETSZ_CATCH					"catch error"
#define RETSZ_NEVERREACH			"never run to here"
#define	RETN_CATCH					0x0900		//unknown error throw by catch(ˇ)
#define RETN_NEVERREACH				0x0901		//logical impossibly

///////////////////////////////////////////////////以下为服务器返回////////////////////////////

//SERVER RETURN VALUE
#define SENDQ						0x1000
#define RETN_SENDQ_OK				0x1010		//：（内容）返回答案										//Return right content
#define RETN_SENDQ_SYSERR			0x1011		//：系统故障												//System Error
#define	RETN_SENDQ_SYSREFUSE		0x1012		//：系统拒绝												//System Refused request
#define	RETN_SENDQ_PARAMERR			0x1013		//：参数错误												//Parameter error
#define	RETN_SENDQ_BUSY				0x1014		//：系统忙（内容）例如：返回“系统忙，请1分钟后再试….”	//System busy
#define	RETN_SENDQ_NOMATCH			0x1015		//：无匹配答案												//no match answer or question check failed

#define RETSZ_SENDQ_OK				"（内容）返回答案"			
#define RETSZ_SENDQ_SYSERR			"系统故障"					
#define	RETSZ_SENDQ_SYSREFUSE		"系统拒绝"					
#define	RETSZ_SENDQ_PARAMERR		"参数错误"					
#define	RETSZ_SENDQ_BUSY			"系统忙，请1分钟后再试…"	
#define	RETSZ_SENDQ_NOMATCH			"无匹配答案"				

#define JUDGE_GOOD					"1"
#define JUDGE_JUSTSOSO				"2"
#define JUDGE_AWFUL					"3"

#define	JUDGEANSWER					0x1100
#define RETN_JUDGEANSWER_OK			0x1110		//：成功			//Send OK
#define RETN_JUDGEANSWER_SYSERR		0x1111		//：系统故障		// System Error
#define RETN_JUDGEANSWER_SYSREFUSE	0x1112		//：系统拒绝		// System Refused request
#define RETN_JUDGEANSWER_PARAMERR	0x1113		//：参数错误		// Parameter error
#define RETN_JUDGEANSWER_BUSY		0x1114		//：系统忙			// System busy
#define RETN_JUDGEANSWER_NOMATCH	0x1115		//：无匹配问题		// no match answer or question check failed

#define RETSZ_JUDGEANSWER_OK		"成功"
#define RETSZ_JUDGEANSWER_SYSERR	"系统故障"
#define RETSZ_JUDGEANSWER_SYSREFUSE	"系统拒绝"
#define RETSZ_JUDGEANSWER_PARAMERR	"参数错误"
#define RETSZ_JUDGEANSWER_BUSY		"系统忙，请1分钟后再试…"
#define RETSZ_JUDGEANSWER_NOMATCH	"无匹配问题"

#define SENDQEX						0x1300
#define RETN_SENDQEX_OK				0x1310		//：（内容）返回答案									// Return right content
#define RETN_SENDQEX_SYSERR			0x1311		//：系统故障											// System Error
#define	RETN_SENDQEX_SYSREFUSE		0x1312		//：系统拒绝											// System Refused request
#define	RETN_SENDQEX_PARAMERR		0x1313		//：参数错误											// Parameter error
#define	RETN_SENDQEX_BUSY			0x1314		//：系统忙（内容）例如：返回“系统忙，请1分钟后再试….”// System busy
#define	RETN_SENDQEX_NOMATCH		0x1315		//：无匹配答案											// no match answer or question check failed

#define RETSZ_SENDQEX_OK			"（内容）返回答案"
#define RETSZ_SENDQEX_SYSERR		"系统故障"
#define	RETSZ_SENDQEX_SYSREFUSE		"系统拒绝"
#define	RETSZ_SENDQEX_PARAMERR		"参数错误"
#define	RETSZ_SENDQEX_BUSY			"系统忙，请1分钟后再试…"
#define	RETSZ_SENDQEX_NOMATCH		"无匹配答案"

#define SENDQNEW					0x1400
#define RETN_SENDQNEW_OK			0x1410		//：（内容）返回答案										// Return right content
#define RETN_SENDQNEW_SYSERR		0x1411		//：系统故障												// System Error
#define RETN_SENDQNEW_SYSREFUSE		0x1412		//：系统拒绝												// System Refused request
#define RETN_SENDQNEW_PARAMERR		0x1413		//：参数错误												// Parameter error
#define RETN_SENDQNEW_BUSY			0x1414		//：系统忙（内容）例如：返回"系统忙，请1分钟后再试…."		// System busy
#define RETN_SENDQNEW_NOMATCH		0x1415		//：（内容）无匹配答案或者脏语等警告语						// no match answer or question check failed

#define RETSZ_SENDQNEW_OK			"（内容）返回答案"
#define RETSZ_SENDQNEW_SYSERR		"系统故障"
#define	RETSZ_SENDQNEW_SYSREFUSE	"系统拒绝"
#define	RETSZ_SENDQNEW_PARAMERR		"参数错误"
#define	RETSZ_SENDQNEW_BUSY			"系统忙，请1分钟后再试…"
#define	RETSZ_SENDQNEW_NOMATCH		"无匹配答案或者脏语等警告语"


#define RECVANS						0x1500
#define RETN_RECVANS_OK				0x1510		//：（内容）返回答案										// Return right content
#define RETN_RECVANS_SYSERR			0x1511		//：系统错误												// System Error
#define RETN_RECVANS_SYSREFUSE		0x1512		//：系统拒绝												// System Refused request
#define RETN_RECVANS_PARAMERR		0x1513		//：参数错误												// Parameter error
#define RETN_RECVANS_BUSY			0x1514		//：系统忙（内容）例如：返回“系统忙，请1分钟后再试….”	// System busy 

#define RETSZ_RECVANS_OK			"（内容）返回答案"
#define RETSZ_RECVANS_SYSERR		"系统故障"
#define	RETSZ_RECVANS_SYSREFUSE		"系统拒绝"
#define	RETSZ_RECVANS_PARAMERR		"参数错误"
#define	RETSZ_RECVANS_BUSY			"系统忙，请1分钟后再试…"

#define HotQTopN					0x1600
#define RETN_HotQTopN_OK			0x1610		//：（内容）返回HotQuestionTopN内容							// Return right content
#define RETN_HotQTopN_SYSERR		0x1611		//：系统错误												// System Error
#define RETN_HotQTopN_SYSREFUSE		0x1612		//：系统拒绝												// System Refused request
#define RETN_HotQTopN_PARAMERR		0x1613		//：参数错误												// Parameter error
#define RETN_HotQTopN_BUSY			0x1614		//：系统忙（内容）例如：返回“系统忙，请1分钟后再试….”	// System busy

#define RETSZ_HotQTopN_OK			"（内容）返回HotQuestionTopN内容"
#define RETSZ_HotQTopN_SYSERR		"系统故障"
#define	RETSZ_HotQTopN_SYSREFUSE	"系统拒绝"
#define	RETSZ_HotQTopN_PARAMERR		"参数错误"
#define	RETSZ_HotQTopN_BUSY			"系统忙，请1分钟后再试…"

#define REASONSTAISFIED  0x1A00
#define REASONSTAISFIED_OK 0x1A10
#define REASONSTAISFIED_SYSERR 0x1A11
#define REASONSTAISFIED_SYSREFUSE 0x1A12
#define REASONSTAISFIED_PARAMERR 0x1A13
#define REASONSTAISFIED_BUSY 0x1A14

#define DESC_REASONSTAISFIED_OK "（内容）返回不满意内容"
#define DESC_REASONSTAISFIED_SYSERR "系统故障"
#define DESC_REASONSTAISFIED_SYSREFUSE "系统拒绝"
#define DESC_REASONSTAISFIED_PARAMERR "参数错误"
#define DESC_REASONSTAISFIED_BUSY "系统忙，请1分钟后再试…"

// DDSS连续问答(0x1700) 
/*
0x1710：（内容）返回答案
0x1711：系统故障
0x1712：系统拒绝
0x1713：参数错误
0x1714：系统忙（内容）例如：返回"系统忙，请1分钟后再试…."
0x1715：（内容）无匹配答案或者脏语等警告语

Command为0x1715时: 内容为 "0" \0无答案提示或者脏语等警告语 \0 "0"
Command为0x1710时:
以Xml格式返回匹配答案(MasterAns<游戏类型、答案Id、答案内容、用户日志标记>)
和相关问题(OtherAns<游戏类型、答案Id、问题Id、问题内容>)
*/
#define SENDQV2						0x1700
#define RETN_SENDQV2_OK				0x1710		//：（内容）返回答案
#define RETN_SENDQV2_SYSERR			0x1711		//：系统故障
#define RETN_SENDQV2_SYSREFUSE		0x1712		//：系统拒绝
#define RETN_SENDQV2_PARAMERR		0x1713		//：参数错误
#define RETN_SENDQV2_BUSY			0x1714		//：系统忙（内容）例如：返回"系统忙，请1分钟后再试…."
#define RETN_SENDQV2_NOMATCH		0x1715		//：（内容）无匹配答案或者脏语等警告语

#define RETSZ_SENDQV2_OK			"（内容）返回答案"
#define RETSZ_SENDQV2_SYSERR		"系统故障"
#define	RETSZ_SENDQV2_SYSREFUSE		"系统拒绝"
#define	RETSZ_SENDQV2_PARAMERR		"参数错误"
#define	RETSZ_SENDQV2_BUSY			"系统忙，请1分钟后再试…"
#define	RETSZ_SENDQV2_NOMATCH		"无匹配答案或者脏语等警告语"