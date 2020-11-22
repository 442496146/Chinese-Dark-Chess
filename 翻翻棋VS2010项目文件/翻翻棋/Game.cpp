//  ��������Game.cpp
//  ���ܣ���Ϸ����
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <windows.h>

typedef struct chess
{	int num;//������ţ�Ҳ�����ӵ������ͼƬ�ļ���
	int point;//���ӽ�ɫ�Ĵ�С�������0������2����ɳԱ��������ܳ���
	char color;//���ӽ�ɫ����ɫ
}CHESS;

typedef struct click
{	int num;//������ţ�Ҳ�����ӵ������ͼƬ�ļ���
	int point;//���ӽ�ɫ�Ĵ�С�������0������2����ɳԱ��������ܳ���
	char color;//���ӽ�ɫ����ɫ
	int map;//�ṹ�������±��Ӧ������ͼԪ����ţ�Ҳ������ͼԪ�ص�ͼƬ�ļ���������ֵ�ǲ���ģ���Ϊ����ͼԪ�صĲ�����ȷ������ģ�
	bool overturn;//�Ƿ��Ѿ�����ת��
}MAP;

typedef struct choosed
{	int i;//������
	int j;//������
}CHOOSED;

int numofB=16;//����ʣ���ɫ������������ʼΪ16
int numofR=16;//����ʣ���ɫ������������ʼΪ16

IMAGE img[9],//����ͼԪ���飨9�ţ�
	  imgback,//��Ϸ����

	  //ʹ����Ԫ��դ����ʵ��͸����ͼ
	  //����ͼ����ԭͼ��Ӧ��ԭͼ��Ҫ͸���Ĳ��֣��ô���ɫ��ʾ��ԭͼ��Ҫ��ʾ�Ĳ��֣��ô���ɫ��ʾ��
	  chessblack,//�ף��ڵװ�ͼ��
	  //ԭͼ����Ҫ͸���Ĳ��֣��ô���ɫ��ʾ��
	  chessimg[32],//��������ͼƬ�飨32�ţ�
	  chessback,//���ӱ��棨�׵ײ�ͼ��

	  youwin,//��Ӯ�˵Ľ���
	  youlose,//�����˵Ľ���
	  player1win,//���1Ӯ�˵Ľ���
	  player2win,//���2Ӯ�˵Ľ���
	  draw,//ƽ�ֵĽ���

	  chessboard,//��Ϸ�������Ҳ�����ʾѡ�е����ӵ�����ĵ�ͼ
	  Player,//��Ϸ�������Ҳ�����ʾ����ҡ�����
	  name1,//��Ϸ�������Ҳ�����ʾ��Player1������
	  name2;//��Ϸ�������Ҳ�����ʾ��Player2������

CHESS Library[32]={{0,0,'B'},{1,0,'B'},{2,0,'B'},{3,0,'B'},{4,0,'B'},{5,1,'B'},{6,1,'B'},{7,2,'B'},
					{8,2,'B'},{9,3,'B'},{10,3,'B'},{11,4,'B'},{12,4,'B'},{13,5,'B'},{14,5,'B'},{15,6,'B'},
					{16,0,'R'},{17,0,'R'},{18,0,'R'},{19,0,'R'},{20,0,'R'},{21,1,'R'},{22,1,'R'},{23,2,'R'},
					{24,2,'R'},{25,3,'R'},{26,3,'R'},{27,4,'R'},{28,4,'R'},{29,5,'R'},{30,5,'R'},{31,6,'R'}};

void Loadback(void);//�������̽���������Ҫ��ͼƬ
void Print(void);//����ʼ����
void RandChess(MAP Location[32]);//���ϴ�Ӱ���
void DispChess(MAP Location[32],char *color);//��һ�ε�����ӣ����ӷ�����ȷ�����1��������ɫ
void Go(MAP Location[32]);//��ʼ���˶�ս
void AlphaGo(MAP Location[32]);//��ʼ�˻���ս
void dischoose(MAP Location[32],int i,int j);//�Ҳ�����ʾѡ������ͼƬ
void dipcancel();//�Ҳ�ѡ������������ʾ
void turnover(MAP Location[32],int i,int j);//��ת����
void move(MAP Location[32],CHOOSED iandj[2]);//�ƶ�����
bool cando(CHOOSED iandj[2]);//�ж��Ƿ�������
void location(MOUSEMSG msg,int *a,int *b);//ȷ�����겢����
void ailocation1(int *a,int *b, int numer);//ȷ���������ɵĹ����������겢����
void ailocation2(CHOOSED iandj[2],int number);//ȷ���������ɵı��������������Ŀ��λ�ò�����
void attack(MAP Location[32],CHOOSED iandj[2]);//����
void choose1(MAP Location[32],CHOOSED iandj[2],char color);//��������һ��������ѡ���Լ�������
void aichoose1(MAP Location[32],CHOOSED iandj[2],char color);//���������һ��������ѡ���Լ�������
int choose2(MAP Location[32],CHOOSED iandj[2]);//����ڶ���������ȷ������λ�ã�ִ�й�����ת���ƶ�����
void Rule(MAP Location[32],char color);//����һ��CHOOSED iandj[2]�������һλ�ú͵ڶ�λ�����ꣻ����choose1��choose2��choose2��choose1�е��ã�
void aiRule(MAP Location[32],char color);//����һ��CHOOSED iandj[2]�������һλ�ú͵ڶ�λ�����ꣻ����aichoose1��choose2��choose2��aichoose1�е��ã�
////////////////////////////////////////////////////////////////////////////////
//����: 	Game
// ����:	��ʼ��Ϸ
// �㷨:      
// ����: 	int i iΪ0ʱִ�����˶�ս��iΪ1ʱִ���˻���ս
// ����:	��
////////////////////////////////////////////////////////////////////////////////
void Game(int i)
{	
	//�ṹ�����ʼ�� num=-1;point=-1;color='A';map���ǽṹ�������±��Ӧ������ͼԪ����ţ�Ҳ������ͼԪ�ص�ͼƬ�ļ���;overturn=false;
	MAP Location[32]={{-1,-1,'A',8,false},{-1,-1,'A',8,false},{-1,-1,'A',8,false},{-1,-1,'A',0,false},{-1,-1,'A',1,false},{-1,-1,'A',8,false},{-1,-1,'A',8,false},{-1,-1,'A',8,false},
					  {-1,-1,'A',5,false},{-1,-1,'A',3,false},{-1,-1,'A',8,false},{-1,-1,'A',1,false},{-1,-1,'A',0,false},{-1,-1,'A',8,false},{-1,-1,'A',5,false},{-1,-1,'A',3,false},
					  {-1,-1,'A',7,false},{-1,-1,'A',6,false},{-1,-1,'A',3,false},{-1,-1,'A',5,false},{-1,-1,'A',3,false},{-1,-1,'A',5,false},{-1,-1,'A',7,false},{-1,-1,'A',6,false},
					  {-1,-1,'A',2,false},{-1,-1,'A',4,false},{-1,-1,'A',2,false},{-1,-1,'A',4,false},{-1,-1,'A',2,false},{-1,-1,'A',4,false},{-1,-1,'A',2,false},{-1,-1,'A',4,false}};

	Loadback();//�������̽���������Ҫ��ͼƬ
	Print();//����ʼ����
	RandChess(Location);//���ϴ�Ӱ���
	if(i==0)//���Game()�������Ϊ0��ִ��Go(),����ʼ���˶�ս
		Go(Location);
	if(i==1)//���Game()�������Ϊ1��ִ��Go(),����ʼ�˻���ս
		AlphaGo(Location);
}
////////////////////////////////////////////////////////////////////////////////
//����: 	Loadback
// ����:	�������̽���������Ҫ��ͼƬ
// �㷨:      
// ����: 	��
// ����:	��
////////////////////////////////////////////////////////////////////////////////
void Loadback()
{
	char imgfile[30]="";

	loadimage(&imgback,"ͼƬ\\��Ϸ����.jpg");

	loadimage(&img[0], "ͼƬ\\���̸���\\���Խ���.jpg");
	loadimage(&img[1], "ͼƬ\\���̸���\\�ζԽ���.jpg");
	loadimage(&img[2], "ͼƬ\\���̸���\\������.jpg");
	loadimage(&img[3], "ͼƬ\\���̸���\\������.jpg");
	loadimage(&img[4], "ͼƬ\\���̸���\\������.jpg");
	loadimage(&img[5], "ͼƬ\\���̸���\\������.jpg");
	loadimage(&img[6], "ͼƬ\\���̸���\\˫��.jpg");
	loadimage(&img[7], "ͼƬ\\���̸���\\˫��.jpg");
	loadimage(&img[8], "ͼƬ\\���̸���\\�հ�.jpg");

	loadimage(&chessback,"ͼƬ\\����\\����.jpg");

	loadimage(&chessblack,"ͼƬ\\����\\��.jpg");
	for(int i=0;i<32;i++)
	{
		sprintf(imgfile,"ͼƬ\\����\\%d.jpg",i);
		loadimage(&chessimg[i],imgfile);
	}
	loadimage(&youwin,"ͼƬ\\��Ӯ��.jpg");
	loadimage(&youlose,"ͼƬ\\������.jpg");
	loadimage(&player1win,"ͼƬ\\Player1Ӯ��.jpg");
	loadimage(&player2win,"ͼƬ\\Player2Ӯ��.jpg");
	loadimage(&draw,"ͼƬ\\ƽ��.jpg");

	loadimage(&chessboard,"ͼƬ\\chessboard.jpg");
	loadimage(&Player,"ͼƬ\\Player.jpg");
	loadimage(&name1,"ͼƬ\\name1.jpg");
	loadimage(&name2,"ͼƬ\\name2.jpg");
}
////////////////////////////////////////////////////////////////////////////////
// ����:    Print
// ����:    ͬʱ��ʾ�հ׵ĸ���ͼƬ�����Խ��߸���ͼƬ���ζԽ���ͼƬ�������ϸ���ͼ
//			Ƭ�������¸���ͼƬ�������ϸ���ͼƬ�������¸���ͼƬ��˫������ͼƬ��˫
//			�θ���ͼƬ������һ��������������Ϸ���档
// �㷨:      
// ����:    ��			
// ����:    ��
////////////////////////////////////////////////////////////////////////////////
void Print(void)	//����ʼ����
{
	putimage(0,0, &imgback);
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<8;j++)
		{
			if( (i==0&&j!=3&&j!=4) || (i==1&&j==2) || (i==1&&j==5) )//�����հ׵ĸ���
				putimage(120+90*j,64+90*i, &img[8]);
			if( (i==0&&j==3) || (i==1&&j==4) )//�������Խ��߸���
				putimage(120+90*j,64+90*i, &img[0]);
			if( (i==0&&j==4) || (i==1&&j==3) )//�����ζԽ��߸���
				putimage(120+90*j,64+90*i, &img[1]);
			if( i==3&&(j==0||j==2||j==4||j==6) )//���������ϸ���
				putimage(120+90*j,64+90*i, &img[2]);
			if( (i==1&&(j==1||j==7)) || (i==2&&(j==2||j==4)) )//���������¸���
				putimage(120+90*j,64+90*i, &img[3]);
			if( i==3&&(j==1||j==3||j==5||j==7) )//���������ϸ���
				putimage(120+90*j,64+90*i, &img[4]);
			if( (i==1&&(j==0||j==6)) || (i==2&&(j==3||j==5)) )//���������¸���
				putimage(120+90*j,64+90*i, &img[5]);
			if( i==2&&(j==1||j==7) )//����˫������
				putimage(120+90*j,64+90*i, &img[6]);
			if( i==2&&(j==0||j==6) )//����˫�θ���
				putimage(120+90*j,64+90*i, &img[7]);
			//����ͼ ͸����
			//NOTSRCERASE��ʹ�ò������͵�OR���򣩲��������Դ��Ŀ������������ɫֵ��Ȼ�󽫺ϳɵ���ɫȡ��
			//Ŀ��ͼ�� = NOT (Ŀ��ͼ�� OR Դͼ��)
			//SRCINVERT��ͨ��ʹ�ò�����XOR����򣩲�������Դ��Ŀ������������ɫ�ϲ�
			//Ŀ��ͼ�� = Ŀ��ͼ�� XOR Դͼ��
			putimage(120+90*j,64+90*i, &chessblack,NOTSRCERASE);//�ף��ڵװ�ͼ��
			putimage(120+90*j,64+90*i, &chessback,SRCINVERT);//���ӱ��棨�׵ײ�ͼ��
		}
	putimage(858,70,&Player);//	���ء���ҡ�ͼƬ
}
////////////////////////////////////////////////////////////////////////////////
// ����:    RandChess
// ����:    ���ϴ�Ӱ��ӣ�����32���������������õ������������Library���ѯ��
//			������Loaction��Ĳ�����
// �㷨:      
// ����:    MAP Location[32] ���̸�λ��״̬
// ����:    ��
////////////////////////////////////////////////////////////////////////////////
void RandChess (MAP Location[32]) //���ϴ��
{
	int i,j,tmp,num[32];

	//�õ�ǰʱ���������������������ʹÿ���������ɲ�ͬ�������
	srand( (unsigned)time( NULL ) );

	for(i=0;i<32;i++) //����32����0 1 ... 31
	{
		while(1) //����һ����
		{
			tmp=32*rand()/ RAND_MAX; //����һ�������
			for(j=0;j<i;j++) //�ж��Ƿ��ظ�
				if(tmp==num[j]) //�����ظ���
					break;
			if(j==i) //�ҵ�һ����
			{
				num[i]=tmp; //�����
				break; //��ʼ��һ��
			}
		}
	}
	//������õ���32�����������Library���ѯ��������Loaction��Ĳ���
	for(i=0;i<4;i++)
		for(j=0;j<8;j++)
		{
			Location[j+i*8].num=num[j+i*8];
			Location[j+i*8].point=Library[num[j+i*8]].point;
			Location[j+i*8].color=Library[num[j+i*8]].color;
		}
}
////////////////////////////////////////////////////////////////////////////////
// ����:    DispChess
// ����:    ��һ�ε�����ӣ����ӷ�����ȷ�����1��������ɫ
// �㷨:      
// ����:    MAP Location[32] ���̸�λ��״̬
//			char *color �C �������ɫ
// ����:    ��
////////////////////////////////////////////////////////////////////////////////
void DispChess(MAP Location[32],char *color)//�������
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
						putimage(120+90*j,64+90*i, &chessblack,NOTSRCERASE);//�ף��ڵװ�ͼ��
						putimage(120+90*j,64+90*i, &chessimg[Location[j+i*8].num],SRCINVERT);//�������棨�׵ײ�ͼ��
						*color=Location[j+i*8].color;
						Location[j+i*8].overturn=true;
						back=1;
					}
	}
}
////////////////////////////////////////////////////////////////////////////////
// ����:    Go
// ���ܣ�   ��ʼ���˶�ս
// �㷨:      
// ����:    MAP Location[32] ���̸�λ��״̬
// ����:    ��
////////////////////////////////////////////////////////////////////////////////
void Go(MAP Location[32])
{	
	char Player1,Player2;
	putimage(858,126,&name1);//��ʾ��Player1��ͼƬ
	DispChess(Location,&Player1);//�����ʾ����
	if(Player1=='B')
	{
		Player2='R';
		while(numofR>0&&numofB>0)//�������к���ɫ����������0ʱ����Ϸ��������ս���н��
		{
			putimage(858,126, &name2);//��ʾ��Player2��ͼƬ
			Rule(Location,Player2);
			if(numofR>0&&numofB>0)//�������к���ɫ����������0ʱ����Ϸ��������ս���н��
			{
				putimage(858,126, &name1);//���ء�Player1��ͼƬ
				Rule(Location,Player1);
			}
			else
				break;
		}
		if(numofR==0&&numofB!=0)
			putimage(0,0,&player1win);//BӮ��Player1Ӯ
		if(numofR!=0&&numofB==0)
			putimage(0,0,&player2win);//RӮ��Player2Ӯ
		if(numofR==0&&numofB==0)
			putimage(0,0,&draw);//ƽ��
		numofR=16;numofB=16;
	}
	else
	{
		Player2='B';
		while(numofR>0&&numofB>0)//�������к���ɫ����������0ʱ����Ϸ��������ս���н��
		{
			putimage(858,126, &name2);//��ʾ��Player2��ͼƬ
			Rule(Location,Player2);
			if(numofR>0&&numofB>0)//�������к���ɫ����������0ʱ����Ϸ��������ս���н��
			{
				putimage(858,126, &name1);//���ء�Player1��ͼƬ
				Rule(Location,Player1);
			}
			else
				break;
		}
		if(numofR==0&&numofB!=0)
			putimage(0,0,&player2win);//BӮ��Player2Ӯ
		if(numofR!=0&&numofB==0)
			putimage(0,0,&player1win);//RӮ��Player1Ӯ
		if(numofR==0&&numofB==0)
			putimage(0,0,&draw);//ƽ��
		numofR=16;numofB=16;
	}
}
////////////////////////////////////////////////////////////////////////////////
// ����:    AlphaGo
// ���ܣ�   ��ʼ�˻���ս
// �㷨:      
// ����:    MAP Location[32] ���̸�λ��״̬
// ����:    ��
////////////////////////////////////////////////////////////////////////////////
void AlphaGo(MAP Location[32])
{	
	char Player1,Player2;
	putimage(858,126,&name1);//��ʾ��Player1��ͼƬ
	DispChess(Location,&Player1);//�����ʾ����
	if(Player1=='B')
	{
		Player2='R';
		while(numofR>0&&numofB>0)//�������к���ɫ����������0ʱ����Ϸ��������ս���н��
		{
			putimage(858,126, &name2);//��ʾ��Player2��ͼƬ
			aiRule(Location,Player2);
			if(numofR>0&&numofB>0)//�������к���ɫ����������0ʱ����Ϸ��������ս���н��
			{
				putimage(858,126, &name1);//���ء�Player1��ͼƬ
				Rule(Location,Player1);
			}
			else
				break;
		}
		if(numofR==0&&numofB!=0)
			putimage(0,0,&youwin);//BӮ��Player1Ӯ�����Ӯ
		if(numofR!=0&&numofB==0)
			putimage(0,0,&youlose);//RӮ��Player2Ӯ������Ӯ���������
		if(numofR==0&&numofB==0)
			putimage(0,0,&draw);//ƽ��
		numofR=16;numofB=16;
	}
	else
	{
		Player2='B';
		while(numofR>0&&numofB>0)//�������к���ɫ����������0ʱ����Ϸ��������ս���н��
		{
			putimage(858,126, &name2);//��ʾ��Player2��ͼƬ
			aiRule(Location,Player2);
			if(numofR>0&&numofB>0)//�������к���ɫ����������0ʱ����Ϸ��������ս���н��
			{
				putimage(858,126, &name1);//���ء�Player1��ͼƬ
				Rule(Location,Player1);
			}
			else
				break;
		}
		if(numofR==0&&numofB!=0)
			putimage(0,0,&youlose);//BӮ��Player2Ӯ������Ӯ���������
		if(numofR!=0&&numofB==0)
			putimage(0,0,&youwin);//RӮ��Player1Ӯ�����Ӯ
		if(numofR==0&&numofB==0)
			putimage(0,0,&draw);//ƽ��
		numofR=16;numofB=16;
	}
}
////////////////////////////////////////////////////////////////////////////////
// ���ƣ�    dischoose
// ����:     �Ҳ�����ʾѡ������ͼƬ
// �㷨:      
// ����:    MAP Location[32] ���̸�λ��״̬
//          int i,j -��������λ��
// ����: 	��
////////////////////////////////////////////////////////////////////////////////
void dischoose(MAP Location[32],int i,int j)//�Ҳ�����ʾѡ������ͼƬ
{		
	putimage(858,182, &chessblack,NOTSRCERASE);//�ף��ڵװ�ͼ��
	putimage(858,182, &chessimg[Location[j+i*8].num],SRCINVERT);//�������棨�׵ײ�ͼ��
}
////////////////////////////////////////////////////////////////////////////////
// ����:    dipcancel
// ����:    �Ҳ�ѡ������������ʾ
// �㷨:    
// ����:    ��
// ����:    ��
////////////////////////////////////////////////////////////////////////////////
void dipcancel()//�Ҳ�ѡ������������ʾ
{
	putimage(858,182, &chessboard);
}
////////////////////////////////////////////////////////////////////////////////
// ����:    turnover
// ����:    ��ת����
// �㷨:    
// ����:    MAP Location[32] ���̸�λ��״̬
//          int i,j -��������λ��
// ����:    ��
////////////////////////////////////////////////////////////////////////////////
void turnover(MAP Location[32],int i,int j)//��ת����
{
	putimage(120+90*j,64+90*i, &chessblack,NOTSRCERASE);//�ף��ڵװ�ͼ��
	putimage(120+90*j,64+90*i, &chessimg[Location[j+i*8].num],SRCINVERT);//�������棨�׵ײ�ͼ��
	Location[j+i*8].overturn=true;//���ĵ�Location�ж�Ӧλ�õķ�ת״̬Ϊtrue
}
////////////////////////////////////////////////////////////////////////////////
// ����:    move
// ����:    �ƶ�����
// �㷨:      
// ����:    MAP Location[32] ���̸�λ��״̬
// 			CHOOSED iandj[2] 0Ϊ����λ�����꣬1Ϊ������λ������
// ����:      ��
// ע������:���Ҫ����ʾ��ʼλ�õ�����ͼ�����ó�ʼλ�õ�����ͼƬ��ʧ����Ŀ��λ��
//			��ʾ������ͼ���ٽ������ڳ�ʼλ�õ�Location�е���Ϣ��ʼ��
////////////////////////////////////////////////////////////////////////////////
void move(MAP Location[32],CHOOSED iandj[2])//�ƶ�����
{
	//�������ڳ�ʼλ�õ�Location�е���Ϣת�Ƶ�Ŀ��λ��
	Location[iandj[1].j + iandj[1].i*8].num=Location[iandj[0].j+iandj[0].i*8].num;
	Location[iandj[1].j + iandj[1].i*8].point=Location[iandj[0].j+iandj[0].i*8].point;
	Location[iandj[1].j + iandj[1].i*8].color=Location[iandj[0].j+iandj[0].i*8].color;
	//��ʾ��ʼλ�õ�����ͼ�����ó�ʼλ�õ�����ͼƬ��ʧ
	putimage(120+90*iandj[0].j,64+90*iandj[0].i, &img[Location[iandj[0].j+iandj[0].i*8].map]);
	//��Ŀ��λ����ʾ������ͼ
	putimage(120+90*iandj[1].j, 64+90*iandj[1].i, &chessblack,NOTSRCERASE);//�ף��ڵװ�ͼ��
	putimage(120+90*iandj[1].j, 64+90*iandj[1].i, &chessimg[Location[iandj[1].j+iandj[1].i*8].num],SRCINVERT);//�������棨�׵ײ�ͼ��
	//�������ڳ�ʼλ�õ�Location�е���Ϣ��ʼ��
	Location[iandj[0].j+iandj[0].i*8].num=-1;
	Location[iandj[0].j+iandj[0].i*8].point=-1;
	Location[iandj[0].j+iandj[0].i*8].color='A';
}
////////////////////////////////////////////////////////////////////////////////
// ����:    cando
// ����:    �ж��Ƿ�������
// �㷨:      
// ����:	CHOOSED iandj[2] iandj[0]�м�¼����λ�����꣬iandj[1]�м�¼������λ������
// ����:    ����ܣ�����true�����ܷ���false
// ע������:
////////////////////////////////////////////////////////////////////////////////
bool cando(CHOOSED iandj[2])//�ж��Ƿ�������
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
// ����:     location
// ����:     ȷ�����������겢����
// �㷨:      
// ����:     MOUSEMSG msg -�����ȡ�������Ϣ 
//           int *a,int *b  -�洢��������ӵ�λ����Ϣ
// ����:     ��
// ע������:
////////////////////////////////////////////////////////////////////////////////
void location(MOUSEMSG msg,int *a,int *b)//ȷ�����겢����
{	
	int out=0;//��out=1ʱֹͣѭ��
	for(int i=0;i<4&&out==0;i++)
		for(int j=0;j<8&&out==0;j++)
			if((msg.x>=120+90*j)&&(msg.x<=120+90*j+90)&&(msg.y>=64+90*i)&&(msg.y<=64+90*i+90))
			{
				*a=i;
				*b=j;
				out=1;//�˳�ѭ��
			}
}
////////////////////////////////////////////////////////////////////////////////
// ����:     ailocation1
// ����:     ȷ���������ɵĹ����������겢����
// �㷨:      
// ����:     int *a,int *b  -�洢��������ӵ�λ����Ϣ
//           int number ȡ��������еĵڼ��������
// ����:     ��
// ע������: ����������ٶȿ죬���ÿ��ֱ������һ���������ֵ���أ����ֵ��������
//			 һ���ģ�����������
////////////////////////////////////////////////////////////////////////////////
void ailocation1(int *a,int *b, int number)
{	
	int i,j,tmp,num[32],out=0;;
	//�õ�ǰʱ���������������������ʹÿ���������ɲ�ͬ�������
	srand( (unsigned)time( NULL ) );
	for(i=0;i<32;i++) //����32����0 1 ... 31
	{
		while(1) //����һ����
		{
			tmp=32*rand()/ RAND_MAX; //����һ�������
			for(j=0;j<i;j++) //�ж��Ƿ��ظ�
				if(tmp==num[j]) //�����ظ���
					break;
			if(j==i) //�ҵ�һ����
			{
				num[i]=tmp; //�����
				break; //��ʼ��һ��
			}
		}
	}
	for(int i=0;i<4&&out==0;i++)
		for(int j=0;j<8&&out==0;j++)
		{
			if(num[number]==(j+i*8))//�������ת�ɶ�Ӧ���꣬�������긳ֵ����
			{
				*a=i;
				*b=j;
				out=1;
			}
		}
}
////////////////////////////////////////////////////////////////////////////////
// ����:     ailocation2
// ����:     ȷ���������ɵı��������������Ŀ��λ�ò�����
// �㷨:      
// ����:     CHOOSED iandj[2]  ��¼����λ������
//           int number ȡ��������еĵڼ��������
// ����:     ��
// ע������: ����������ٶȿ죬���ÿ��ֱ������һ���������ֵ���أ����ֵ������
//			 ��һ���ģ�����������
//			 �������ɵı��������������Ŀ��λ�ñ����ڵ�һ��ȷ���Ĺ�����λ�õ�ǰ
//			 ������һ������
////////////////////////////////////////////////////////////////////////////////
void ailocation2(CHOOSED iandj[2], int number)
{
	int i,j,tmp,num[4];
	//�õ�ǰʱ���������������������ʹÿ���������ɲ�ͬ�������
	srand( (unsigned)time( NULL ) );
	for(i=0;i<4;i++) //����4����0 1 2 3
	{
		while(1) //����һ����
		{
			tmp=4*rand()/ RAND_MAX; //����һ�������
			for(j=0;j<i;j++) //�ж��Ƿ��ظ�
				if(tmp==num[j]) //�����ظ���
					break;
			if(j==i) //�ҵ�һ����
			{
				num[i]=tmp; //�����
				break; //��ʼ��һ��
			}
		}
	}
	switch(num[number])
	{
		case 0://���Ͻ������ƶ�һ��
			iandj[1].i=iandj[0].i-1;
			iandj[1].j=iandj[0].j;
			break;
		case 1://���ҽ������ƶ�һ��
			iandj[1].i=iandj[0].i;
			iandj[1].j=iandj[0].j+1;
			break;
		case 2://���½������ƶ�һ��
			iandj[1].i=iandj[0].i+1;
			iandj[1].j=iandj[0].j;
			break;
		case 3://����������ƶ�һ��
			iandj[1].i=iandj[0].i;
			iandj[1].j=iandj[0].j-1;
			break;
	}
}
////////////////////////////////////////////////////////////////////////////////
// ����:    attack
// ����:    �������Ӵ�С��ͬ���������������̱����ǣ����������������̱����ǣ�˫��
//			����ȥһ���������������������Ӵ��ڱ�����������ʱ���ý��������Ӹ��Ǳ�
//			���������ӣ�����������ȥһ����������������������С���ڱ�����������ʱ
//			�����������̱����ǣ������������Ӳ����䶯��
// �㷨:      
// ����:    MAP Location[32] ���̸�λ��״̬
// 			CHOOSED iandj[2] 0Ϊ����λ�����꣬1Ϊ������λ������
// ����: 	��
// ע������:
////////////////////////////////////////////////////////////////////////////////
void attack(MAP Location[32],CHOOSED iandj[2]){
	//�������Ӵ�С��ͬ
	if(Location[iandj[0].j+iandj[0].i*8].point==Location[iandj[1].j+iandj[1].i*8].point)
	{
		putimage(120+90*iandj[0].j,64+90*iandj[0].i, &img[Location[iandj[0].j+iandj[0].i*8].map]);//�������������̱�����
		putimage(120+90*iandj[1].j,64+90*iandj[1].i, &img[Location[iandj[1].j+iandj[1].i*8].map]);//���������������̱�����
		//��������������λ�ó�ʼ��
		Location[iandj[0].j+iandj[0].i*8].num=-1;
		Location[iandj[0].j+iandj[0].i*8].point=-1;
		Location[iandj[0].j+iandj[0].i*8].color='A';
		//����������������λ�ó�ʼ��
		Location[iandj[1].j+iandj[1].i*8].num=-1;
		Location[iandj[1].j+iandj[1].i*8].point=-1;
		Location[iandj[1].j+iandj[1].i*8].color='A';
		//˫������ȥһ��������
		 numofR--;
		 numofB--;
	}
	//�����������Ӵ��ڱ�����������ʱ
	else 
		if(Location[iandj[0].j+iandj[0].i*8].point>Location[iandj[1].j+iandj[1].i*8].point && !(Location[iandj[0].j+iandj[0].i*8].point==6 && Location[iandj[1].j+iandj[1].i*8].point==0)
			|| Location[iandj[0].j+iandj[0].i*8].point==0&&Location[iandj[1].j+iandj[1].i*8].point==6)
		{
			//����������������λ��״̬ת�Ƶ�����������
			Location[iandj[1].j+iandj[1].i*8].num=Location[iandj[0].j+iandj[0].i*8].num;
			Location[iandj[1].j+iandj[1].i*8].point=Location[iandj[0].j+iandj[0].i*8].point;
			Location[iandj[1].j+iandj[1].i*8].color=Location[iandj[0].j+iandj[0].i*8].color;
			//���ɽ��������Ӵ����̿�ͼ
			putimage(120+90*iandj[0].j,64+90*iandj[0].i, &img[Location[iandj[0].j+iandj[0].i*8].map]);
			//�ý��������Ӹ��Ǳ�����������
			putimage(120+90*iandj[1].j,64+90*iandj[1].i, &chessblack,NOTSRCERASE);//�ף��ڵװ�ͼ��
			putimage(120+90*iandj[1].j,64+90*iandj[1].i, &chessimg[Location[iandj[0].j+iandj[0].i*8].num],SRCINVERT);//�������棨�׵ײ�ͼ��
			//����������ȥһ��������
			if(Location[iandj[0].j+iandj[0].i*8].color=='B')//ȷ��������������ɫ
				numofR--;
			else
				numofB--;
			//������������ԭʼλ��״̬��ʼ��
			Location[iandj[0].j+iandj[0].i*8].num=-1;
			Location[iandj[0].j+iandj[0].i*8].point=-1;
			Location[iandj[0].j+iandj[0].i*8].color='A';
		}
			//������������С�ڱ�����������ʱ
		else 
			if(Location[iandj[0].j+iandj[0].i*8].point<Location[iandj[1].j+iandj[1].i*8].point && !(Location[iandj[0].j+iandj[0].i*8].point==6 && Location[iandj[1].j+iandj[1].i*8].point==0)
				|| Location[iandj[0].j+iandj[0].i*8].point==6&&Location[iandj[1].j+iandj[1].i*8].point==0)
			{
				//���������̱����ǣ������������Ӳ����䶯
				putimage(120+90*iandj[0].j,64+90*iandj[0].i, &img[Location[iandj[0].j+iandj[0].i*8].map]);
				//��������ȥһ��������
				if(Location[iandj[0].j+iandj[0].i*8].color=='B')//ȷ��������������ɫ
					numofB--;
				else
					numofR--;
				//�������������̳�ʼ��
				Location[iandj[0].j+iandj[0].i*8].num=-1;
				Location[iandj[0].j+iandj[0].i*8].point=-1;
				Location[iandj[0].j+iandj[0].i*8].color='A';
			}
}
////////////////////////////////////////////////////////////////////////////////
// ����:   choose1
// ����:   ��ҵ�һ��ѡ�������һ��ѭ������ȡ��һ�������Ϣ��ȷ����������Ϣ������
//		   ѭ������ȡ�ڶ��������Ϣ��ȷ������������Ϣ��
// �㷨:      
// ����:    MAP Location[32] ���̸�λ��״̬
// 			CHOOSED iandj[2] 0Ϊ����λ�����꣬1Ϊ������λ������
// 			char color -������Ҷ�Ӧ��������ɫ
// ����: 	��
// ע������:
////////////////////////////////////////////////////////////////////////////////
void choose1(MAP Location[32],CHOOSED iandj[2],char color)//�����һ��������ѡ���Լ�������
{
	int out=0;
	while(out==0)//out==1ʱ����һ��ѭ��
	{
		MOUSEMSG msg;
		msg = GetMouseMsg();//��ȡ��һ�������Ϣ
		FlushMouseMsgBuffer();
		if(msg.uMsg==WM_LBUTTONDOWN)
		{

			location(msg,&(iandj[0].i),&(iandj[0].j));//ȷ�������湥����λ��������iandj[0]
			if(Location[iandj[0].j+iandj[0].i*8].num==-1)//�˴�û������
				continue;
			if(!Location[iandj[0].j+iandj[0].i*8].overturn)//û������
			{
				turnover(Location,iandj[0].i,iandj[0].j);//����
				out=1;//�˳�һ��ѭ��
			}
			else//������
			{
				if(Location[iandj[0].j+iandj[0].i*8].color==color)//���ѡ�е������ҷ����壬��������
				{
					dischoose(Location,iandj[0].i,iandj[0].j);//�Ҳ�����ʾѡ������ͼƬ					
					while(true)//����ѭ��
					{
						MOUSEMSG msge;		
						msge = GetMouseMsg();//��ȡ�ڶ��������Ϣ
						if(msge.uMsg==WM_RBUTTONDOWN)//����һ�
						{
							dipcancel();//��ȡ����һ���Ľ���������ѡ��
							break;//�˳�����ѭ��������һ��ѭ�����»�ȡһ���µĵ�һ����������λ����Ϣ
						}
						if(msge.uMsg==WM_LBUTTONDOWN)//������
						{
							location(msge,&(iandj[1].i),&(iandj[1].j));//ȷ�������湥����λ��������iandj[1]
							if(choose2(Location,iandj))//���choose2����Ϊ1
							{
								continue;//��ֹͣ����ѭ�������»�ȡһ���µĵڶ���λ����Ϣ
							}
							else//���choose2����Ϊ0
							{
								out=1;//׼���˳�һ��ѭ��
								break;//�˳�����ѭ��
							}
						}
					}
				}
				else//���ѡ�е��岻���ҷ����壬����ѡ��
					continue;
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
// ����:   aichoose1
// ����:   ���Ե�һ��ѡ�������һ��ѭ����ȷ����������Ϣ������ѭ����ȷ����������
//		   ��Ϣ��
// �㷨:      
// ����:    MAP Location[32] ���̸�λ��״̬
// 			CHOOSED iandj[2] 0Ϊ����λ�����꣬1Ϊ������λ������
// 			char color -������Ҷ�Ӧ��������ɫ
// ����: 	��
// ע������:����ȷ������ܿ죬����������̺ܿ죬ע�����ʵ��ط���sleep()
////////////////////////////////////////////////////////////////////////////////
void aichoose1(MAP Location[32],CHOOSED iandj[2],char color)//�����һ��������ѡ���Լ�������
{
	int out=0;
	int num1=0,num2;//num1ȷ��ȡ��һ�����λ��������ĵ�num1+1������,num2ȷ��ȡ�ڶ������λ��������ĵ�num2+1������
	Sleep(500);//ȷ����һ��λ��ǰ˯��0.5s
	while(out==0)//out==1ʱ����һ��ѭ��
	{
		ailocation1(&(iandj[0].i),&(iandj[0].j),num1);//ȷ�������湥����λ��������iandj[0]
		num1++;//��һ�����λ��������ĵ�num1+1���������ã��´��õ�num1+2������
		num2=0;//��ʼ����˵���ڶ������λ��������������δ��ʹ�ù�
		if(Location[iandj[0].j+iandj[0].i*8].num==-1)//�˴�û������
				continue;
		if(!Location[iandj[0].j+iandj[0].i*8].overturn)//û������
		{
			turnover(Location,iandj[0].i,iandj[0].j);//����
			out=1;//�˳�һ��ѭ��
		}
		else//������
		{
			if(Location[iandj[0].j+iandj[0].i*8].color==color)//���ѡ�е������ҷ����壬��������
			{
				dischoose(Location,iandj[0].i,iandj[0].j);//�Ҳ�����ʾѡ������ͼƬ
				Sleep(500);//ȷ���ڶ���λ��ǰ˯��0.5s
				while(true)//����ѭ��
				{
					if(num2==4)//���num2һֱ�ӵ�����4������ִ�ж���ѭ������˵����һ��λ��������Χ�ĸ����򶼲���������������
						break; //��Ϊ��ֹailocation2�����е�һ����¼λ�õĳ���Ϊ4��������������ֹͣ����ѭ��������һ��ѭ�����»�ȡһ���µĵ�һ����������λ����Ϣ��
					ailocation2(iandj,num2);//ȷ�������湥����λ��������iandj[1]
						num2++;//�ڶ������λ��������ĵ�num2+1���������ã��´��õ�num2+2������
					if(choose2(Location,iandj))//���choose2����Ϊ1
					{
						continue;//��ֹͣ����ѭ�������»�ȡһ���µĵڶ���λ����Ϣ
					}
					else//���choose2����Ϊ0
					{
						out=1;//׼���˳�һ��ѭ��
						break;//�˳�����ѭ��
					}
				}
			}
			else//���ѡ�е��岻���ҷ����壬����ѡ��
				continue;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
// ����:    choose2
// ����:    ��ҵڶ���ѡ�������ȷ�����ƶ����ǽ���
// �㷨:      
// ����:    MAP Location[32] ���̸�λ��״̬
// 			CHOOSED iandj[2] 0Ϊ����λ�����꣬1Ϊ������λ������
// ����:    ��������ʱ����һ��λ��û����return 0����û������return 0���屻������
//			�岻ͬɫreturn 0����ͬɫreturn 1������������return 1
//			����1�����»�ȡ�ڶ��������Ϣ����ֹͣchoose1�еĶ���ѭ��
//			����0��������ȡ���������Ϣ��������ֹͣchoose1�е�һ������ѭ��
// ע������:
////////////////////////////////////////////////////////////////////////////////
int choose2(MAP Location[32],CHOOSED iandj[2])//����ڶ���������ȷ������λ�ã�ִ�й�����ת���ƶ�����
{
	if(cando(iandj))//�ж��Ƿ�������
		//��������
		if(Location[iandj[1].j+iandj[1].i*8].num==-1)//�жϱ��������Ƿ������ӣ����������ƶ�����
		{	
			move(Location,iandj);
			return 0;//������ȡ���������Ϣ��������ֹͣchoose1�е�һ������ѭ��
		}
		else//������
			if(Location[iandj[1].j+iandj[1].i*8].overturn)//�ж��Ƿ񱻷���
				if(Location[iandj[0].j+iandj[0].i*8].color==Location[iandj[1].j+iandj[1].i*8].color)//�ж��Ƿ�ͬɫ
					return 1;//���»�ȡ�ڶ��������Ϣ����ֹͣchoose1�еĶ���ѭ��
				else//��ͬɫ
				{
					attack(Location,iandj);//����
					return 0;
				}
			else//û������
				if(Location[iandj[0].j+iandj[0].i*8].color==Location[iandj[1].j+iandj[1].i*8].color)//�ж��Ƿ�ͬɫ
				{
					turnover(Location,iandj[1].i,iandj[1].j);//��ת
					return 0;
				}
				else//��ͬɫ
				{
					attack(Location,iandj);//����
					return 0;
				}
	else//����������
		return 1;
}
////////////////////////////////////////////////////////////////////////////////
// ����:    Rule
// ����:    ����һ��CHOOSED iandj[2]�������һλ�ú͵ڶ�λ�����ꣻ����choose1��
//			choose2��choose2��choose1�е��ã�
// �㷨:      
// ����:    MAP Location[32] ���̸�λ��״̬
//			char color -  ���������ɫ
// ����:    ��
////////////////////////////////////////////////////////////////////////////////
void Rule(MAP Location[32],char color)
{	
	CHOOSED iandj[2]={{-1,-1},{-1,-1}};//��ʼ��
	choose1(Location,iandj,color);
}
////////////////////////////////////////////////////////////////////////////////
// ����:    aiRule
// ����:    ����һ��CHOOSED iandj[2]�������һλ�ú͵ڶ�λ�����ꣻ����aichoose1��
//			choose2��choose2��aichoose1�е��ã�
// �㷨:      
// ����:    MAP Location[32] ���̸�λ��״̬
//			char color -  ���������ɫ
// ����:    ��
////////////////////////////////////////////////////////////////////////////////
void aiRule(MAP Location[32],char color)
{	
	CHOOSED iandj[2]={{-1,-1},{-1,-1}};//��ʼ��
	aichoose1(Location,iandj,color);
}