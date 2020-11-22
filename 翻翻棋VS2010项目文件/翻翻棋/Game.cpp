//  程序名：Game.cpp
//  功能：游戏进行
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <windows.h>

typedef struct chess
{	int num;//棋子序号，也是棋子的正面的图片文件名
	int point;//棋子角色的大小，如兵是0，马是2，马可吃兵，兵不能吃马
	char color;//棋子角色的颜色
}CHESS;

typedef struct click
{	int num;//棋子序号，也是棋子的正面的图片文件名
	int point;//棋子角色的大小，如兵是0，马是2，马可吃兵，兵不能吃马
	char color;//棋子角色的颜色
	int map;//结构体数组下标对应的棋盘图元素序号，也是棋盘图元素的图片文件名（它的值是不变的，因为棋盘图元素的布局是确定不变的）
	bool overturn;//是否已经被翻转过
}MAP;

typedef struct choosed
{	int i;//行坐标
	int j;//列坐标
}CHOOSED;

int numofB=16;//棋盘剩余黑色棋子总数，初始为16
int numofR=16;//棋盘剩余红色棋子总数，初始为16

IMAGE img[9],//棋盘图元素组（9张）
	  imgback,//游戏界面

	  //使用三元光栅操作实现透明贴图
	  //掩码图：与原图对应。原图需要透明的部分，用纯黑色表示；原图需要显示的部分，用纯白色表示。
	  chessblack,//底（黑底白图）
	  //原图：需要透明的部分，用纯白色表示。
	  chessimg[32],//棋子正面图片组（32张）
	  chessback,//棋子背面（白底彩图）

	  youwin,//你赢了的界面
	  youlose,//你输了的界面
	  player1win,//玩家1赢了的界面
	  player2win,//玩家2赢了的界面
	  draw,//平局的界面

	  chessboard,//游戏界面中右侧栏显示选中的棋子的区域的底图
	  Player,//游戏界面中右侧栏显示“玩家”两字
	  name1,//游戏界面中右侧栏显示“Player1”两字
	  name2;//游戏界面中右侧栏显示“Player2”两字

CHESS Library[32]={{0,0,'B'},{1,0,'B'},{2,0,'B'},{3,0,'B'},{4,0,'B'},{5,1,'B'},{6,1,'B'},{7,2,'B'},
					{8,2,'B'},{9,3,'B'},{10,3,'B'},{11,4,'B'},{12,4,'B'},{13,5,'B'},{14,5,'B'},{15,6,'B'},
					{16,0,'R'},{17,0,'R'},{18,0,'R'},{19,0,'R'},{20,0,'R'},{21,1,'R'},{22,1,'R'},{23,2,'R'},
					{24,2,'R'},{25,3,'R'},{26,3,'R'},{27,4,'R'},{28,4,'R'},{29,5,'R'},{30,5,'R'},{31,6,'R'}};

void Loadback(void);//加载棋盘界面所有需要的图片
void Print(void);//画初始棋盘
void RandChess(MAP Location[32]);//随机洗子摆子
void DispChess(MAP Location[32],char *color);//第一次点击棋子，棋子翻开，确定玩家1的棋子颜色
void Go(MAP Location[32]);//开始人人对战
void AlphaGo(MAP Location[32]);//开始人机对战
void dischoose(MAP Location[32],int i,int j);//右侧栏显示选中棋子图片
void dipcancel();//右侧选中区无棋子显示
void turnover(MAP Location[32],int i,int j);//翻转棋子
void move(MAP Location[32],CHOOSED iandj[2]);//移动棋子
bool cando(CHOOSED iandj[2]);//判断是否能行棋
void location(MOUSEMSG msg,int *a,int *b);//确定坐标并保存
void ailocation1(int *a,int *b, int numer);//确定电脑生成的攻击棋子坐标并保存
void ailocation2(CHOOSED iandj[2],int number);//确定电脑生成的被攻击棋子坐标或目的位置并保存
void attack(MAP Location[32],CHOOSED iandj[2]);//攻击
void choose1(MAP Location[32],CHOOSED iandj[2],char color);//玩家行棋第一步操作，选中自己的棋子
void aichoose1(MAP Location[32],CHOOSED iandj[2],char color);//电脑行棋第一步操作，选中自己的棋子
int choose2(MAP Location[32],CHOOSED iandj[2]);//行棋第二步操作，确定落子位置，执行攻击翻转或移动操作
void Rule(MAP Location[32],char color);//创建一个CHOOSED iandj[2]用来存第一位置和第二位置坐标；运行choose1和choose2（choose2在choose1中调用）
void aiRule(MAP Location[32],char color);//创建一个CHOOSED iandj[2]用来存第一位置和第二位置坐标；运行aichoose1和choose2（choose2在aichoose1中调用）
////////////////////////////////////////////////////////////////////////////////
//名称: 	Game
// 功能:	开始游戏
// 算法:      
// 参数: 	int i i为0时执行人人对战，i为1时执行人机对战
// 返回:	无
////////////////////////////////////////////////////////////////////////////////
void Game(int i)
{	
	//结构数组初始化 num=-1;point=-1;color='A';map就是结构体数组下标对应的棋盘图元素序号，也是棋盘图元素的图片文件名;overturn=false;
	MAP Location[32]={{-1,-1,'A',8,false},{-1,-1,'A',8,false},{-1,-1,'A',8,false},{-1,-1,'A',0,false},{-1,-1,'A',1,false},{-1,-1,'A',8,false},{-1,-1,'A',8,false},{-1,-1,'A',8,false},
					  {-1,-1,'A',5,false},{-1,-1,'A',3,false},{-1,-1,'A',8,false},{-1,-1,'A',1,false},{-1,-1,'A',0,false},{-1,-1,'A',8,false},{-1,-1,'A',5,false},{-1,-1,'A',3,false},
					  {-1,-1,'A',7,false},{-1,-1,'A',6,false},{-1,-1,'A',3,false},{-1,-1,'A',5,false},{-1,-1,'A',3,false},{-1,-1,'A',5,false},{-1,-1,'A',7,false},{-1,-1,'A',6,false},
					  {-1,-1,'A',2,false},{-1,-1,'A',4,false},{-1,-1,'A',2,false},{-1,-1,'A',4,false},{-1,-1,'A',2,false},{-1,-1,'A',4,false},{-1,-1,'A',2,false},{-1,-1,'A',4,false}};

	Loadback();//加载棋盘界面所有需要的图片
	Print();//画初始棋盘
	RandChess(Location);//随机洗子摆子
	if(i==0)//如果Game()输入参数为0，执行Go(),即开始人人对战
		Go(Location);
	if(i==1)//如果Game()输入参数为1，执行Go(),即开始人机对战
		AlphaGo(Location);
}
////////////////////////////////////////////////////////////////////////////////
//名称: 	Loadback
// 功能:	加载棋盘界面所有需要的图片
// 算法:      
// 参数: 	无
// 返回:	无
////////////////////////////////////////////////////////////////////////////////
void Loadback()
{
	char imgfile[30]="";

	loadimage(&imgback,"图片\\游戏界面.jpg");

	loadimage(&img[0], "图片\\棋盘格子\\主对角线.jpg");
	loadimage(&img[1], "图片\\棋盘格子\\次对角线.jpg");
	loadimage(&img[2], "图片\\棋盘格子\\单左上.jpg");
	loadimage(&img[3], "图片\\棋盘格子\\单左下.jpg");
	loadimage(&img[4], "图片\\棋盘格子\\单右上.jpg");
	loadimage(&img[5], "图片\\棋盘格子\\单右下.jpg");
	loadimage(&img[6], "图片\\棋盘格子\\双主.jpg");
	loadimage(&img[7], "图片\\棋盘格子\\双次.jpg");
	loadimage(&img[8], "图片\\棋盘格子\\空白.jpg");

	loadimage(&chessback,"图片\\棋子\\背面.jpg");

	loadimage(&chessblack,"图片\\棋子\\底.jpg");
	for(int i=0;i<32;i++)
	{
		sprintf(imgfile,"图片\\棋子\\%d.jpg",i);
		loadimage(&chessimg[i],imgfile);
	}
	loadimage(&youwin,"图片\\你赢了.jpg");
	loadimage(&youlose,"图片\\你输了.jpg");
	loadimage(&player1win,"图片\\Player1赢了.jpg");
	loadimage(&player2win,"图片\\Player2赢了.jpg");
	loadimage(&draw,"图片\\平局.jpg");

	loadimage(&chessboard,"图片\\chessboard.jpg");
	loadimage(&Player,"图片\\Player.jpg");
	loadimage(&name1,"图片\\name1.jpg");
	loadimage(&name2,"图片\\name2.jpg");
}
////////////////////////////////////////////////////////////////////////////////
// 名称:    Print
// 功能:    同时显示空白的格子图片，主对角线格子图片，次对角线图片，单左上格子图
//			片，单左下格子图片，单右上格子图片，单右下格子图片，双主格子图片，双
//			次格子图片等生成一个完整的棋盘游戏界面。
// 算法:      
// 参数:    无			
// 返回:    无
////////////////////////////////////////////////////////////////////////////////
void Print(void)	//画初始棋盘
{
	putimage(0,0, &imgback);
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<8;j++)
		{
			if( (i==0&&j!=3&&j!=4) || (i==1&&j==2) || (i==1&&j==5) )//画出空白的格子
				putimage(120+90*j,64+90*i, &img[8]);
			if( (i==0&&j==3) || (i==1&&j==4) )//画出主对角线格子
				putimage(120+90*j,64+90*i, &img[0]);
			if( (i==0&&j==4) || (i==1&&j==3) )//画出次对角线格子
				putimage(120+90*j,64+90*i, &img[1]);
			if( i==3&&(j==0||j==2||j==4||j==6) )//画出单左上格子
				putimage(120+90*j,64+90*i, &img[2]);
			if( (i==1&&(j==1||j==7)) || (i==2&&(j==2||j==4)) )//画出单左下格子
				putimage(120+90*j,64+90*i, &img[3]);
			if( i==3&&(j==1||j==3||j==5||j==7) )//画出单右上格子
				putimage(120+90*j,64+90*i, &img[4]);
			if( (i==1&&(j==0||j==6)) || (i==2&&(j==3||j==5)) )//画出单右下格子
				putimage(120+90*j,64+90*i, &img[5]);
			if( i==2&&(j==1||j==7) )//画出双主格子
				putimage(120+90*j,64+90*i, &img[6]);
			if( i==2&&(j==0||j==6) )//画出双次格子
				putimage(120+90*j,64+90*i, &img[7]);
			//掩码图 透明化
			//NOTSRCERASE，使用布尔类型的OR（或）操作符组合源和目标矩形区域的颜色值，然后将合成的颜色取反
			//目标图像 = NOT (目标图像 OR 源图像)
			//SRCINVERT，通过使用布尔型XOR（异或）操作符将源和目标矩形区域的颜色合并
			//目标图像 = 目标图像 XOR 源图像
			putimage(120+90*j,64+90*i, &chessblack,NOTSRCERASE);//底（黑底白图）
			putimage(120+90*j,64+90*i, &chessback,SRCINVERT);//棋子背面（白底彩图）
		}
	putimage(858,70,&Player);//	加载“玩家”图片
}
////////////////////////////////////////////////////////////////////////////////
// 名称:    RandChess
// 功能:    随机洗子摆子（生成32个随机数，用随机得到的序号排序，在Library里查询，
//			并更新Loaction里的参数）
// 算法:      
// 参数:    MAP Location[32] 棋盘各位置状态
// 返回:    无
////////////////////////////////////////////////////////////////////////////////
void RandChess (MAP Location[32]) //随机洗子
{
	int i,j,tmp,num[32];

	//用当前时间种子重置随机数生成器使每次运行生成不同的随机数
	srand( (unsigned)time( NULL ) );

	for(i=0;i<32;i++) //生成32个数0 1 ... 31
	{
		while(1) //生成一个数
		{
			tmp=32*rand()/ RAND_MAX; //产生一个随机数
			for(j=0;j<i;j++) //判断是否重复
				if(tmp==num[j]) //跳过重复数
					break;
			if(j==i) //找到一个数
			{
				num[i]=tmp; //入队列
				break; //开始下一轮
			}
		}
	}
	//用随机得到的32个序号排序，在Library里查询，并更新Loaction里的参数
	for(i=0;i<4;i++)
		for(j=0;j<8;j++)
		{
			Location[j+i*8].num=num[j+i*8];
			Location[j+i*8].point=Library[num[j+i*8]].point;
			Location[j+i*8].color=Library[num[j+i*8]].color;
		}
}
////////////////////////////////////////////////////////////////////////////////
// 名称:    DispChess
// 功能:    第一次点击棋子，棋子翻开，确定玩家1的棋子颜色
// 算法:      
// 参数:    MAP Location[32] 棋盘各位置状态
//			char *color – 存玩家颜色
// 返回:    无
////////////////////////////////////////////////////////////////////////////////
void DispChess(MAP Location[32],char *color)//点击翻棋
{	
	int back=0;
	while(back==0)
	{
		MOUSEMSG msg;
		msg = GetMouseMsg();
		FlushMouseMsgBuffer();
		if(msg.uMsg==WM_LBUTTONDOWN)
			for(int i=0;i<4&&back==0;i++)
				for(int j=0;j<8&&back==0;j++)
					if((msg.x>=120+90*j)&&(msg.x<=120+90*j+90)&&(msg.y>=64+90*i)&&(msg.y<=64+90*i+90))
					{
						putimage(120+90*j,64+90*i, &chessblack,NOTSRCERASE);//底（黑底白图）
						putimage(120+90*j,64+90*i, &chessimg[Location[j+i*8].num],SRCINVERT);//棋子正面（白底彩图）
						*color=Location[j+i*8].color;
						Location[j+i*8].overturn=true;
						back=1;
					}
	}
}
////////////////////////////////////////////////////////////////////////////////
// 名称:    Go
// 功能：   开始人人对战
// 算法:      
// 参数:    MAP Location[32] 棋盘各位置状态
// 返回:    无
////////////////////////////////////////////////////////////////////////////////
void Go(MAP Location[32])
{	
	char Player1,Player2;
	putimage(858,126,&name1);//显示“Player1”图片
	DispChess(Location,&Player1);//点击显示棋子
	if(Player1=='B')
	{
		Player2='R';
		while(numofR>0&&numofB>0)//当棋盘中红或黑色棋子数等于0时，游戏结束，对战已有结果
		{
			putimage(858,126, &name2);//显示“Player2”图片
			Rule(Location,Player2);
			if(numofR>0&&numofB>0)//当棋盘中红或黑色棋子数等于0时，游戏结束，对战已有结果
			{
				putimage(858,126, &name1);//加载“Player1”图片
				Rule(Location,Player1);
			}
			else
				break;
		}
		if(numofR==0&&numofB!=0)
			putimage(0,0,&player1win);//B赢即Player1赢
		if(numofR!=0&&numofB==0)
			putimage(0,0,&player2win);//R赢即Player2赢
		if(numofR==0&&numofB==0)
			putimage(0,0,&draw);//平局
		numofR=16;numofB=16;
	}
	else
	{
		Player2='B';
		while(numofR>0&&numofB>0)//当棋盘中红或黑色棋子数等于0时，游戏结束，对战已有结果
		{
			putimage(858,126, &name2);//显示“Player2”图片
			Rule(Location,Player2);
			if(numofR>0&&numofB>0)//当棋盘中红或黑色棋子数等于0时，游戏结束，对战已有结果
			{
				putimage(858,126, &name1);//加载“Player1”图片
				Rule(Location,Player1);
			}
			else
				break;
		}
		if(numofR==0&&numofB!=0)
			putimage(0,0,&player2win);//B赢即Player2赢
		if(numofR!=0&&numofB==0)
			putimage(0,0,&player1win);//R赢即Player1赢
		if(numofR==0&&numofB==0)
			putimage(0,0,&draw);//平局
		numofR=16;numofB=16;
	}
}
////////////////////////////////////////////////////////////////////////////////
// 名称:    AlphaGo
// 功能：   开始人机对战
// 算法:      
// 参数:    MAP Location[32] 棋盘各位置状态
// 返回:    无
////////////////////////////////////////////////////////////////////////////////
void AlphaGo(MAP Location[32])
{	
	char Player1,Player2;
	putimage(858,126,&name1);//显示“Player1”图片
	DispChess(Location,&Player1);//点击显示棋子
	if(Player1=='B')
	{
		Player2='R';
		while(numofR>0&&numofB>0)//当棋盘中红或黑色棋子数等于0时，游戏结束，对战已有结果
		{
			putimage(858,126, &name2);//显示“Player2”图片
			aiRule(Location,Player2);
			if(numofR>0&&numofB>0)//当棋盘中红或黑色棋子数等于0时，游戏结束，对战已有结果
			{
				putimage(858,126, &name1);//加载“Player1”图片
				Rule(Location,Player1);
			}
			else
				break;
		}
		if(numofR==0&&numofB!=0)
			putimage(0,0,&youwin);//B赢即Player1赢即玩家赢
		if(numofR!=0&&numofB==0)
			putimage(0,0,&youlose);//R赢即Player2赢即电脑赢，玩家输了
		if(numofR==0&&numofB==0)
			putimage(0,0,&draw);//平局
		numofR=16;numofB=16;
	}
	else
	{
		Player2='B';
		while(numofR>0&&numofB>0)//当棋盘中红或黑色棋子数等于0时，游戏结束，对战已有结果
		{
			putimage(858,126, &name2);//显示“Player2”图片
			aiRule(Location,Player2);
			if(numofR>0&&numofB>0)//当棋盘中红或黑色棋子数等于0时，游戏结束，对战已有结果
			{
				putimage(858,126, &name1);//加载“Player1”图片
				Rule(Location,Player1);
			}
			else
				break;
		}
		if(numofR==0&&numofB!=0)
			putimage(0,0,&youlose);//B赢即Player2赢即电脑赢，玩家输了
		if(numofR!=0&&numofB==0)
			putimage(0,0,&youwin);//R赢即Player1赢即玩家赢
		if(numofR==0&&numofB==0)
			putimage(0,0,&draw);//平局
		numofR=16;numofB=16;
	}
}
////////////////////////////////////////////////////////////////////////////////
// 名称：    dischoose
// 功能:     右侧栏显示选中棋子图片
// 算法:      
// 参数:    MAP Location[32] 棋盘各位置状态
//          int i,j -传入棋子位置
// 返回: 	无
////////////////////////////////////////////////////////////////////////////////
void dischoose(MAP Location[32],int i,int j)//右侧栏显示选中棋子图片
{		
	putimage(858,182, &chessblack,NOTSRCERASE);//底（黑底白图）
	putimage(858,182, &chessimg[Location[j+i*8].num],SRCINVERT);//棋子正面（白底彩图）
}
////////////////////////////////////////////////////////////////////////////////
// 名称:    dipcancel
// 功能:    右侧选中区无棋子显示
// 算法:    
// 参数:    无
// 返回:    无
////////////////////////////////////////////////////////////////////////////////
void dipcancel()//右侧选中区无棋子显示
{
	putimage(858,182, &chessboard);
}
////////////////////////////////////////////////////////////////////////////////
// 名称:    turnover
// 功能:    翻转棋子
// 算法:    
// 参数:    MAP Location[32] 棋盘各位置状态
//          int i,j -传入棋子位置
// 返回:    无
////////////////////////////////////////////////////////////////////////////////
void turnover(MAP Location[32],int i,int j)//翻转棋子
{
	putimage(120+90*j,64+90*i, &chessblack,NOTSRCERASE);//底（黑底白图）
	putimage(120+90*j,64+90*i, &chessimg[Location[j+i*8].num],SRCINVERT);//棋子正面（白底彩图）
	Location[j+i*8].overturn=true;//更改的Location中对应位置的翻转状态为true
}
////////////////////////////////////////////////////////////////////////////////
// 名称:    move
// 功能:    移动棋子
// 算法:      
// 参数:    MAP Location[32] 棋盘各位置状态
// 			CHOOSED iandj[2] 0为进攻位置坐标，1为被进攻位置坐标
// 返回:      无
// 注意事项:务必要先显示初始位置的棋盘图，即让初始位置的棋子图片消失，在目的位置
//			显示其棋子图，再将棋子在初始位置的Location中的信息初始化
////////////////////////////////////////////////////////////////////////////////
void move(MAP Location[32],CHOOSED iandj[2])//移动棋子
{
	//将棋子在初始位置的Location中的信息转移到目标位置
	Location[iandj[1].j + iandj[1].i*8].num=Location[iandj[0].j+iandj[0].i*8].num;
	Location[iandj[1].j + iandj[1].i*8].point=Location[iandj[0].j+iandj[0].i*8].point;
	Location[iandj[1].j + iandj[1].i*8].color=Location[iandj[0].j+iandj[0].i*8].color;
	//显示初始位置的棋盘图，即让初始位置的棋子图片消失
	putimage(120+90*iandj[0].j,64+90*iandj[0].i, &img[Location[iandj[0].j+iandj[0].i*8].map]);
	//在目的位置显示其棋子图
	putimage(120+90*iandj[1].j, 64+90*iandj[1].i, &chessblack,NOTSRCERASE);//底（黑底白图）
	putimage(120+90*iandj[1].j, 64+90*iandj[1].i, &chessimg[Location[iandj[1].j+iandj[1].i*8].num],SRCINVERT);//棋子正面（白底彩图）
	//将棋子在初始位置的Location中的信息初始化
	Location[iandj[0].j+iandj[0].i*8].num=-1;
	Location[iandj[0].j+iandj[0].i*8].point=-1;
	Location[iandj[0].j+iandj[0].i*8].color='A';
}
////////////////////////////////////////////////////////////////////////////////
// 名称:    cando
// 功能:    判断是否能行棋
// 算法:      
// 参数:	CHOOSED iandj[2] iandj[0]中记录进攻位置坐标，iandj[1]中记录被进攻位置坐标
// 返回:    如果能，返回true，不能返回false
// 注意事项:
////////////////////////////////////////////////////////////////////////////////
bool cando(CHOOSED iandj[2])//判断是否能行棋
{
	if(abs(iandj[0].i-iandj[1].i)==1&&iandj[0].j==iandj[1].j
	||iandj[0].i==iandj[1].i&&abs(iandj[0].j-iandj[1].j)==1)
		if(iandj[1].i>=0&&iandj[1].i<=3&&iandj[1].j>=0&&iandj[1].j<=7)
			return true;
		else
			return false;
	else
		return false;
}
////////////////////////////////////////////////////////////////////////////////
// 名称:     location
// 功能:     确定按键的坐标并保存
// 算法:      
// 参数:     MOUSEMSG msg -输入获取的鼠标信息 
//           int *a,int *b  -存储鼠标点击棋子的位置信息
// 返回:     无
// 注意事项:
////////////////////////////////////////////////////////////////////////////////
void location(MOUSEMSG msg,int *a,int *b)//确定坐标并保存
{	
	int out=0;//当out=1时停止循环
	for(int i=0;i<4&&out==0;i++)
		for(int j=0;j<8&&out==0;j++)
			if((msg.x>=120+90*j)&&(msg.x<=120+90*j+90)&&(msg.y>=64+90*i)&&(msg.y<=64+90*i+90))
			{
				*a=i;
				*b=j;
				out=1;//退出循环
			}
}
////////////////////////////////////////////////////////////////////////////////
// 名称:     ailocation1
// 功能:     确定电脑生成的攻击棋子坐标并保存
// 算法:      
// 参数:     int *a,int *b  -存储鼠标点击棋子的位置信息
//           int number 取随机数组中的第几个随机数
// 返回:     无
// 注意事项: 因电脑运行速度快，如果每次直接生成一个随机数赋值返回，出现的随机数是
//			 一样的，不满足需求
////////////////////////////////////////////////////////////////////////////////
void ailocation1(int *a,int *b, int number)
{	
	int i,j,tmp,num[32],out=0;;
	//用当前时间种子重置随机数生成器使每次运行生成不同的随机数
	srand( (unsigned)time( NULL ) );
	for(i=0;i<32;i++) //生成32个数0 1 ... 31
	{
		while(1) //生成一个数
		{
			tmp=32*rand()/ RAND_MAX; //产生一个随机数
			for(j=0;j<i;j++) //判断是否重复
				if(tmp==num[j]) //跳过重复数
					break;
			if(j==i) //找到一个数
			{
				num[i]=tmp; //入队列
				break; //开始下一轮
			}
		}
	}
	for(int i=0;i<4&&out==0;i++)
		for(int j=0;j<8&&out==0;j++)
		{
			if(num[number]==(j+i*8))//将随机数转成对应坐标，并将坐标赋值保存
			{
				*a=i;
				*b=j;
				out=1;
			}
		}
}
////////////////////////////////////////////////////////////////////////////////
// 名称:     ailocation2
// 功能:     确定电脑生成的被攻击棋子坐标或目的位置并保存
// 算法:      
// 参数:     CHOOSED iandj[2]  记录两次位置坐标
//           int number 取随机数组中的第几个随机数
// 返回:     无
// 注意事项: 因电脑运行速度快，如果每次直接生成一个随机数赋值返回，出现的随机数
//			 是一样的，不满足需求。
//			 电脑生成的被攻击棋子坐标或目的位置必须在第一次确定的攻击子位置的前
//			 后左右一个格当中
////////////////////////////////////////////////////////////////////////////////
void ailocation2(CHOOSED iandj[2], int number)
{
	int i,j,tmp,num[4];
	//用当前时间种子重置随机数生成器使每次运行生成不同的随机数
	srand( (unsigned)time( NULL ) );
	for(i=0;i<4;i++) //生成4个数0 1 2 3
	{
		while(1) //生成一个数
		{
			tmp=4*rand()/ RAND_MAX; //产生一个随机数
			for(j=0;j<i;j++) //判断是否重复
				if(tmp==num[j]) //跳过重复数
					break;
			if(j==i) //找到一个数
			{
				num[i]=tmp; //入队列
				break; //开始下一轮
			}
		}
	}
	switch(num[number])
	{
		case 0://向上进攻或移动一格
			iandj[1].i=iandj[0].i-1;
			iandj[1].j=iandj[0].j;
			break;
		case 1://向右进攻或移动一格
			iandj[1].i=iandj[0].i;
			iandj[1].j=iandj[0].j+1;
			break;
		case 2://向下进攻或移动一格
			iandj[1].i=iandj[0].i+1;
			iandj[1].j=iandj[0].j;
			break;
		case 3://向左进攻或移动一格
			iandj[1].i=iandj[0].i;
			iandj[1].j=iandj[0].j-1;
			break;
	}
}
////////////////////////////////////////////////////////////////////////////////
// 名称:    attack
// 功能:    当两棋子大小相同，进攻方棋子棋盘被覆盖，被进攻方棋子棋盘被覆盖，双方
//			各减去一个棋子数；当进攻方棋子大于被进攻方棋子时，用进攻方棋子覆盖被
//			进攻方棋子，被进攻方减去一个棋子数；当进攻方棋子小于于被进攻方棋子时
//			，进攻方棋盘被覆盖，被进攻方棋子不做变动。
// 算法:      
// 参数:    MAP Location[32] 棋盘各位置状态
// 			CHOOSED iandj[2] 0为进攻位置坐标，1为被进攻位置坐标
// 返回: 	无
// 注意事项:
////////////////////////////////////////////////////////////////////////////////
void attack(MAP Location[32],CHOOSED iandj[2]){
	//当两棋子大小相同
	if(Location[iandj[0].j+iandj[0].i*8].point==Location[iandj[1].j+iandj[1].i*8].point)
	{
		putimage(120+90*iandj[0].j,64+90*iandj[0].i, &img[Location[iandj[0].j+iandj[0].i*8].map]);//进攻方棋子棋盘被覆盖
		putimage(120+90*iandj[1].j,64+90*iandj[1].i, &img[Location[iandj[1].j+iandj[1].i*8].map]);//被进攻方棋子棋盘被覆盖
		//进攻方棋子棋盘位置初始化
		Location[iandj[0].j+iandj[0].i*8].num=-1;
		Location[iandj[0].j+iandj[0].i*8].point=-1;
		Location[iandj[0].j+iandj[0].i*8].color='A';
		//被进攻方棋子棋盘位置初始化
		Location[iandj[1].j+iandj[1].i*8].num=-1;
		Location[iandj[1].j+iandj[1].i*8].point=-1;
		Location[iandj[1].j+iandj[1].i*8].color='A';
		//双方各减去一个棋子数
		 numofR--;
		 numofB--;
	}
	//当进攻方棋子大于被进攻方棋子时
	else 
		if(Location[iandj[0].j+iandj[0].i*8].point>Location[iandj[1].j+iandj[1].i*8].point && !(Location[iandj[0].j+iandj[0].i*8].point==6 && Location[iandj[1].j+iandj[1].i*8].point==0)
			|| Location[iandj[0].j+iandj[0].i*8].point==0&&Location[iandj[1].j+iandj[1].i*8].point==6)
		{
			//将进攻方棋子棋盘位置状态转移到被攻击方处
			Location[iandj[1].j+iandj[1].i*8].num=Location[iandj[0].j+iandj[0].i*8].num;
			Location[iandj[1].j+iandj[1].i*8].point=Location[iandj[0].j+iandj[0].i*8].point;
			Location[iandj[1].j+iandj[1].i*8].color=Location[iandj[0].j+iandj[0].i*8].color;
			//生成进攻方棋子处棋盘块图
			putimage(120+90*iandj[0].j,64+90*iandj[0].i, &img[Location[iandj[0].j+iandj[0].i*8].map]);
			//用进攻方棋子覆盖被进攻方棋子
			putimage(120+90*iandj[1].j,64+90*iandj[1].i, &chessblack,NOTSRCERASE);//底（黑底白图）
			putimage(120+90*iandj[1].j,64+90*iandj[1].i, &chessimg[Location[iandj[0].j+iandj[0].i*8].num],SRCINVERT);//棋子正面（白底彩图）
			//被进攻方减去一个棋子数
			if(Location[iandj[0].j+iandj[0].i*8].color=='B')//确定进攻方棋子颜色
				numofR--;
			else
				numofB--;
			//进攻方处棋子原始位置状态初始化
			Location[iandj[0].j+iandj[0].i*8].num=-1;
			Location[iandj[0].j+iandj[0].i*8].point=-1;
			Location[iandj[0].j+iandj[0].i*8].color='A';
		}
			//当进攻方棋子小于被进攻方棋子时
		else 
			if(Location[iandj[0].j+iandj[0].i*8].point<Location[iandj[1].j+iandj[1].i*8].point && !(Location[iandj[0].j+iandj[0].i*8].point==6 && Location[iandj[1].j+iandj[1].i*8].point==0)
				|| Location[iandj[0].j+iandj[0].i*8].point==6&&Location[iandj[1].j+iandj[1].i*8].point==0)
			{
				//进攻方棋盘被覆盖，被进攻方棋子不做变动
				putimage(120+90*iandj[0].j,64+90*iandj[0].i, &img[Location[iandj[0].j+iandj[0].i*8].map]);
				//进攻方减去一个棋子数
				if(Location[iandj[0].j+iandj[0].i*8].color=='B')//确定进攻方棋子颜色
					numofB--;
				else
					numofR--;
				//进攻方棋子棋盘初始化
				Location[iandj[0].j+iandj[0].i*8].num=-1;
				Location[iandj[0].j+iandj[0].i*8].point=-1;
				Location[iandj[0].j+iandj[0].i*8].color='A';
			}
}
////////////////////////////////////////////////////////////////////////////////
// 名称:   choose1
// 功能:   玩家第一次选择操作，一级循环，获取第一个鼠标信息，确定攻击方信息；二级
//		   循环，获取第二个鼠标信息，确定被攻击方信息。
// 算法:      
// 参数:    MAP Location[32] 棋盘各位置状态
// 			CHOOSED iandj[2] 0为进攻位置坐标，1为被进攻位置坐标
// 			char color -输入玩家对应的棋子颜色
// 返回: 	无
// 注意事项:
////////////////////////////////////////////////////////////////////////////////
void choose1(MAP Location[32],CHOOSED iandj[2],char color)//行棋第一步操作，选中自己的棋子
{
	int out=0;
	while(out==0)//out==1时继续一级循环
	{
		MOUSEMSG msg;
		msg = GetMouseMsg();//获取第一个鼠标信息
		FlushMouseMsgBuffer();
		if(msg.uMsg==WM_LBUTTONDOWN)
		{

			location(msg,&(iandj[0].i),&(iandj[0].j));//确定并保存攻击方位置坐标至iandj[0]
			if(Location[iandj[0].j+iandj[0].i*8].num==-1)//此处没有棋子
				continue;
			if(!Location[iandj[0].j+iandj[0].i*8].overturn)//没翻过棋
			{
				turnover(Location,iandj[0].i,iandj[0].j);//翻棋
				out=1;//退出一级循环
			}
			else//翻过棋
			{
				if(Location[iandj[0].j+iandj[0].i*8].color==color)//如果选中的棋是我方的棋，继续操作
				{
					dischoose(Location,iandj[0].i,iandj[0].j);//右侧栏显示选中棋子图片					
					while(true)//二级循环
					{
						MOUSEMSG msge;		
						msge = GetMouseMsg();//获取第二次鼠标信息
						if(msge.uMsg==WM_RBUTTONDOWN)//如果右击
						{
							dipcancel();//则取消第一个的进攻方棋子选中
							break;//退出二级循环，继续一级循环重新获取一个新的第一个即攻击方位置信息
						}
						if(msge.uMsg==WM_LBUTTONDOWN)//如果左击
						{
							location(msge,&(iandj[1].i),&(iandj[1].j));//确定并保存攻击方位置坐标至iandj[1]
							if(choose2(Location,iandj))//如果choose2返回为1
							{
								continue;//不停止二级循环，重新获取一个新的第二个位置信息
							}
							else//如果choose2返回为0
							{
								out=1;//准备退出一级循环
								break;//退出二级循环
							}
						}
					}
				}
				else//如果选中的棋不是我方的棋，重新选择
					continue;
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
// 名称:   aichoose1
// 功能:   电脑第一次选择操作，一级循环，确定攻击方信息；二级循环，确定被攻击方
//		   信息。
// 算法:      
// 参数:    MAP Location[32] 棋盘各位置状态
// 			CHOOSED iandj[2] 0为进攻位置坐标，1为被进攻位置坐标
// 			char color -输入玩家对应的棋子颜色
// 返回: 	无
// 注意事项:电脑确定坐标很快，所以行棋过程很快，注意在适当地方加sleep()
////////////////////////////////////////////////////////////////////////////////
void aichoose1(MAP Location[32],CHOOSED iandj[2],char color)//行棋第一步操作，选中自己的棋子
{
	int out=0;
	int num1=0,num2;//num1确定取第一个随机位置坐标组的第num1+1个坐标,num2确定取第二个随机位置坐标组的第num2+1个坐标
	Sleep(500);//确定第一个位置前睡眠0.5s
	while(out==0)//out==1时继续一级循环
	{
		ailocation1(&(iandj[0].i),&(iandj[0].j),num1);//确定并保存攻击方位置坐标至iandj[0]
		num1++;//第一个随机位置坐标组的第num1+1个坐标已用，下次用第num1+2个坐标
		num2=0;//初始化，说明第二个随机位置坐标组中坐标未被使用过
		if(Location[iandj[0].j+iandj[0].i*8].num==-1)//此处没有棋子
				continue;
		if(!Location[iandj[0].j+iandj[0].i*8].overturn)//没翻过棋
		{
			turnover(Location,iandj[0].i,iandj[0].j);//翻棋
			out=1;//退出一级循环
		}
		else//翻过棋
		{
			if(Location[iandj[0].j+iandj[0].i*8].color==color)//如果选中的棋是我方的棋，继续操作
			{
				dischoose(Location,iandj[0].i,iandj[0].j);//右侧栏显示选中棋子图片
				Sleep(500);//确定第二个位置前睡眠0.5s
				while(true)//二级循环
				{
					if(num2==4)//如果num2一直加到等于4，还在执行二级循环，则说明第一个位置棋子周围四个方向都不满足行棋条件，
						break; //且为防止ailocation2函数中的一个记录位置的长度为4的数组访问溢出，停止二级循环，继续一级循环重新获取一个新的第一个即攻击方位置信息。
					ailocation2(iandj,num2);//确定并保存攻击方位置坐标至iandj[1]
						num2++;//第二个随机位置坐标组的第num2+1个坐标已用，下次用第num2+2个坐标
					if(choose2(Location,iandj))//如果choose2返回为1
					{
						continue;//不停止二级循环，重新获取一个新的第二个位置信息
					}
					else//如果choose2返回为0
					{
						out=1;//准备退出一级循环
						break;//退出二级循环
					}
				}
			}
			else//如果选中的棋不是我方的棋，重新选择
				continue;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
// 名称:    choose2
// 功能:    玩家第二次选择操作，确定是移动还是进攻
// 算法:      
// 参数:    MAP Location[32] 棋盘各位置状态
// 			CHOOSED iandj[2] 0为进攻位置坐标，1为被进攻位置坐标
// 返回:    可以行棋时，下一个位置没有棋return 0，棋没被翻过return 0，棋被翻过且
//			棋不同色return 0，棋同色return 1；不可以行棋return 1
//			返回1：重新获取第二次鼠标信息，不停止choose1中的二级循环
//			返回0：结束获取两次鼠标信息操作，即停止choose1中的一级二级循环
// 注意事项:
////////////////////////////////////////////////////////////////////////////////
int choose2(MAP Location[32],CHOOSED iandj[2])//行棋第二步操作，确定落子位置，执行攻击翻转或移动操作
{
	if(cando(iandj))//判断是否能行棋
		//可以行棋
		if(Location[iandj[1].j+iandj[1].i*8].num==-1)//判断被攻击区是否无棋子，无棋子则移动棋子
		{	
			move(Location,iandj);
			return 0;//结束获取两次鼠标信息操作，即停止choose1中的一级二级循环
		}
		else//有棋子
			if(Location[iandj[1].j+iandj[1].i*8].overturn)//判断是否被翻过
				if(Location[iandj[0].j+iandj[0].i*8].color==Location[iandj[1].j+iandj[1].i*8].color)//判断是否同色
					return 1;//重新获取第二次鼠标信息，不停止choose1中的二级循环
				else//不同色
				{
					attack(Location,iandj);//进攻
					return 0;
				}
			else//没被翻过
				if(Location[iandj[0].j+iandj[0].i*8].color==Location[iandj[1].j+iandj[1].i*8].color)//判断是否同色
				{
					turnover(Location,iandj[1].i,iandj[1].j);//翻转
					return 0;
				}
				else//不同色
				{
					attack(Location,iandj);//进攻
					return 0;
				}
	else//不可以行棋
		return 1;
}
////////////////////////////////////////////////////////////////////////////////
// 名称:    Rule
// 功能:    创建一个CHOOSED iandj[2]用来存第一位置和第二位置坐标；运行choose1和
//			choose2（choose2在choose1中调用）
// 算法:      
// 参数:    MAP Location[32] 棋盘各位置状态
//			char color -  传入玩家颜色
// 返回:    无
////////////////////////////////////////////////////////////////////////////////
void Rule(MAP Location[32],char color)
{	
	CHOOSED iandj[2]={{-1,-1},{-1,-1}};//初始化
	choose1(Location,iandj,color);
}
////////////////////////////////////////////////////////////////////////////////
// 名称:    aiRule
// 功能:    创建一个CHOOSED iandj[2]用来存第一位置和第二位置坐标；运行aichoose1和
//			choose2（choose2在aichoose1中调用）
// 算法:      
// 参数:    MAP Location[32] 棋盘各位置状态
//			char color -  传入玩家颜色
// 返回:    无
////////////////////////////////////////////////////////////////////////////////
void aiRule(MAP Location[32],char color)
{	
	CHOOSED iandj[2]={{-1,-1},{-1,-1}};//初始化
	aichoose1(Location,iandj,color);
}