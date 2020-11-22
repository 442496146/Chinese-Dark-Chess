//  程序名：main.cpp
//  功能：主函数，含绘图窗口，显示主界面，根据用户反馈的鼠标交互信息选择模块（1开始游戏、2退出游戏、3游戏说明）
#include<stdio.h>
#include<conio.h>
#include<graphics.h>

extern void DispMenu(void);//显示主界面
extern char ChooseMenu(void);//返回用户用鼠标交互的选择（1开始游戏、2退出游戏、3游戏说明）
extern void ChooseGame(void);//选择游戏模式（0人人对战、1人机对战）
extern void Closegraph(void);//关闭图形环境
extern void Introduce(void);//游戏规则说明，载入并显示规则图片，按任意键返回

void main(void)
{
	int done=0;//标志为1时退出系统
	initgraph(960,488);//初始化绘图窗口
	do
	{
		//每轮循环都重新显示界面、接受用户选择、根据用户选择执行相应分支
		DispMenu();//显示主界面
		switch( ChooseMenu() )//返回用户用鼠标交互的选择
		{
		case '1': 
			ChooseGame();//开始游戏
			break;
		case '2':
			Closegraph();//关闭图形环境
			done=1;
			break;
		case '3':
			Introduce();//查看规则
			break;
		}
	}while(!done);
}