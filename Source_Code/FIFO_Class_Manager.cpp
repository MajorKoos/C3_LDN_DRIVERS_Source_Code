//---------------------------------------------------------------------------

//#pragma hdrstop

#include "FIFO_Class_Manager.h"

//---------------------------------------------------------------------------

//#pragma package(smart_init)
MyFifoBuff::MyFifoBuff(DWORD BlockCount, DWORD BlockSize)//���캯��
{
	DWORD memsize;
	memset(&hHandle, 0x00, sizeof(hHandle)); //����ڴ�
	hHandle.SET_UserBuffBlockCount = BlockCount; //���ݿ�����
	hHandle.SET_UserDataBlockSize = BlockSize; //ÿ�����ݿ�Ĵ�С

	if(((BlockCount * BlockSize) % 4) == 0)//�����ܷ�4����
	{
		memsize = (BlockCount * BlockSize) / 4; //��ô������ڴ����4�ı���
	}
	else
	{
		memsize = ((BlockCount * BlockSize) / 4) + 1;
		//������ܱ�4��������ô����4���ֽڣ������ڴ����
	}
	hHandle.pData = new DWORD[memsize]; //�����ڴ�
	hHandle.pSET_UserDataBuff = (byte *)hHandle.pData;
	InitializeCriticalSection(&hCs);
}

//------------------------------------------------------------------------------
MyFifoBuff::~MyFifoBuff(void)//��������
{
	delete[]hHandle.pData; //�ͷ��ڴ�
	DeleteCriticalSection(&hCs);
}

//------------------------------------------------------------------------------
DWORD MyFifoBuff::GetDataLen(void)//ȡ���Ѵ�������ݿ�����
{
	DWORD temp;
	EnterCriticalSection(&hCs); //�̻߳���
	temp = hHandle.DataManager.DataLen;
	LeaveCriticalSection(&hCs);
	return (temp);
}

//------------------------------------------------------------------------------
void MyFifoBuff::InserData(void *Data)//��������
{
	DWORD index;
	byte *pData = (byte *)Data;

	EnterCriticalSection(&hCs); //�̻߳���
	if(hHandle.DataManager.DataLen >= hHandle.SET_UserBuffBlockCount)
		//���������������������
	{
		LeaveCriticalSection(&hCs);
		return;
	}

	if((hHandle.DataManager.End + 1) > hHandle.SET_UserBuffBlockCount)
		//�����������ݴﵽ������β
	{
		hHandle.DataManager.End = 0; //��ô���ݴ�ͷ��ʼ����
	}

	//����Ҫ��������ݵ�������
	for(index = 0; index < hHandle.SET_UserDataBlockSize; index++)
	{
		hHandle.pSET_UserDataBuff[(hHandle.DataManager.End * hHandle.SET_UserDataBlockSize) + index] = *pData++ ;
	}
	hHandle.DataManager.End++ ;
	hHandle.DataManager.DataLen++ ;
	LeaveCriticalSection(&hCs);
}

//------------------------------------------------------------------------------
void MyFifoBuff::GetData(void *Data)//ȡ������
{
	DWORD index;
	byte *pData = (byte *)Data;

	EnterCriticalSection(&hCs); //�̻߳���
	if(hHandle.DataManager.DataLen == 0)//���ݳ���Ϊ�գ��򷵻ش�����Ϣ
	{
		LeaveCriticalSection(&hCs); //�뿪
		return;
	}

	for(index = 0; index < hHandle.SET_UserDataBlockSize; index++)//ȡ���û�����
	{
		*pData++ = hHandle.pSET_UserDataBuff[(hHandle.DataManager.Head * hHandle.SET_UserDataBlockSize) + index];
	}

	if(hHandle.DataManager.Head < (hHandle.SET_UserBuffBlockCount - 1))
		//�����������β
	{
		hHandle.DataManager.Head++ ; //����ָ��ָ����һ���߼���
	}
	else
	{
		hHandle.DataManager.Head = 0; //��ô��ͷ��ʼ
	}
	hHandle.DataManager.DataLen-- ;
	LeaveCriticalSection(&hCs);
}

//------------------------------------------------------------------------------
void MyFifoBuff::ClearData(void)//��մ��������
{
	memset(&hHandle.DataManager, 0x00, sizeof(hHandle.DataManager));
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
