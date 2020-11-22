//  程序名：ChooseMenu.cpp
//  功能：返回用户用鼠标交互选择的模块（1开始游戏、2退出游戏、3游戏说明）
#include<stdio.h>
#include<conio.h>
#include<graphics.h>
////////////////////////////////////////////////////////////////////////////////
// 名称:    ChooseMenu(void)
// 功能:    获取鼠标信息，判断玩家选择
// 算法:      
// 参数:    无
// 返回:    开始游戏返回1，结束游戏返回2，使用说明返回3	
// 注意事项:
////////////////////////////////////////////////////////////////////////////////
char ChooseMenu(void)
{
	char choose=0;//默认为无效选择
	MOUSEMSG msg;//定义鼠标信息
	msg = GetMouseMsg();
	FlushMouseMsgBuffer();
	if(msg.uMsg==WM_LBUTTONDOWN)//如果鼠标点左键
	{
		if((340<=msg.x&&msg.x<=620)&&(215<=msg.y&&msg.y<=275))//点击开始游戏
			choose='1';
		if((340<=msg.x&&msg.x<=620)&&(305<=msg.y&&msg.y<=370))//点击结束游戏
			choose='2';
		if((340<=msg.x&&msg.x<=620)&&(400<=msg.y&&msg.y<=460))//点击游戏说明
			choose='3';
	}
	return choose;
}