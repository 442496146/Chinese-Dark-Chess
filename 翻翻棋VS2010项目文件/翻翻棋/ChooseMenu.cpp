//  ��������ChooseMenu.cpp
//  ���ܣ������û�����꽻��ѡ���ģ�飨1��ʼ��Ϸ��2�˳���Ϸ��3��Ϸ˵����
#include<stdio.h>
#include<conio.h>
#include<graphics.h>
////////////////////////////////////////////////////////////////////////////////
// ����:    ChooseMenu(void)
// ����:    ��ȡ�����Ϣ���ж����ѡ��
// �㷨:      
// ����:    ��
// ����:    ��ʼ��Ϸ����1��������Ϸ����2��ʹ��˵������3	
// ע������:
////////////////////////////////////////////////////////////////////////////////
char ChooseMenu(void)
{
	char choose=0;//Ĭ��Ϊ��Чѡ��
	MOUSEMSG msg;//���������Ϣ
	msg = GetMouseMsg();
	FlushMouseMsgBuffer();
	if(msg.uMsg==WM_LBUTTONDOWN)//����������
	{
		if((340<=msg.x&&msg.x<=620)&&(215<=msg.y&&msg.y<=275))//�����ʼ��Ϸ
			choose='1';
		if((340<=msg.x&&msg.x<=620)&&(305<=msg.y&&msg.y<=370))//���������Ϸ
			choose='2';
		if((340<=msg.x&&msg.x<=620)&&(400<=msg.y&&msg.y<=460))//�����Ϸ˵��
			choose='3';
	}
	return choose;
}