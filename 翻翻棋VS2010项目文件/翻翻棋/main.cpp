//  ��������main.cpp
//  ���ܣ�������������ͼ���ڣ���ʾ�����棬�����û���������꽻����Ϣѡ��ģ�飨1��ʼ��Ϸ��2�˳���Ϸ��3��Ϸ˵����
#include<stdio.h>
#include<conio.h>
#include<graphics.h>

extern void DispMenu(void);//��ʾ������
extern char ChooseMenu(void);//�����û�����꽻����ѡ��1��ʼ��Ϸ��2�˳���Ϸ��3��Ϸ˵����
extern void ChooseGame(void);//ѡ����Ϸģʽ��0���˶�ս��1�˻���ս��
extern void Closegraph(void);//�ر�ͼ�λ���
extern void Introduce(void);//��Ϸ����˵�������벢��ʾ����ͼƬ�������������

void main(void)
{
	int done=0;//��־Ϊ1ʱ�˳�ϵͳ
	initgraph(960,488);//��ʼ����ͼ����
	do
	{
		//ÿ��ѭ����������ʾ���桢�����û�ѡ�񡢸����û�ѡ��ִ����Ӧ��֧
		DispMenu();//��ʾ������
		switch( ChooseMenu() )//�����û�����꽻����ѡ��
		{
		case '1': 
			ChooseGame();//��ʼ��Ϸ
			break;
		case '2':
			Closegraph();//�ر�ͼ�λ���
			done=1;
			break;
		case '3':
			Introduce();//�鿴����
			break;
		}
	}while(!done);
}