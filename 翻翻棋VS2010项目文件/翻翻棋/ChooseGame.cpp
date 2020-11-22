//  程序名：ChooseGame.cpp
//  功能：根据用户反馈的鼠标交互信息选择对战模式（1人人对战、2人机对战）
#include<stdio.h>
#include<conio.h>
#include<graphics.h>
extern void Game(int i);
////////////////////////////////////////////////////////////////////////////////
// 名称:    ChooseGame(void)
// 功能:    获取鼠标信息，判断玩家选择，选择人人，调Game(0);选择人机，调Game(1);
// 算法:      
// 参数:    无
// 返回:    无
// 注意事项:
////////////////////////////////////////////////////////////////////////////////
void ChooseGame(void)
{
	IMAGE img;
	loadimage(&img,"图片\\模式选择.jpg");//加载模式选择界面
	putimage(0,0,&img);//显示模式选择界面
	while(true)
	{
		MOUSEMSG msg;//定义鼠标信息
		msg = GetMouseMsg();
		if(msg.uMsg==WM_LBUTTONDOWN)//如果鼠标点左键
		{
			if((120<=msg.x&&msg.x<=404)&&(203<=msg.y&&msg.y<=266))//人人对战
				Game(0);
			if((566<=msg.x&&msg.x<=850)&&(203<=msg.y&&msg.y<=266))//人机对战
				Game(1);
			getch();//不立即跳出函数，等待用户按键，即按任意键退出游戏胜败提示结果界面，返回游戏菜单主界面
			break;
		}
	}
}