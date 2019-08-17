// ---------------------------------------------------------------------------

#ifndef FIFO_Class_ManagerH
#define FIFO_Class_ManagerH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
// #include <Controls.hpp>
// #include <StdCtrls.hpp>
// #include <Forms.hpp>
// #include <ExtCtrls.hpp>
// #include <Buttons.hpp>
// #include <ComCtrls.hpp>

#define		MY_FIFO_DEF_SUCCESS						                           	0x00			//�����ɹ�
#define		MY_FIFO_DEF_BUFF_NULL						                        0x01			//������Ϊ��
#define		MY_FIFO_DEF_BUFF_FULL						                        0x02			//����������
#define		MY_FIFO_DEF_BUFF_ERROR						                        0x03			//���������ô���

struct MY_FIFO_DATA_MANAGER_STRUCT
{
	DWORD SET_UserBuffBlockCount; // �����û��Ļ������ж������ݿ�
	DWORD SET_UserDataBlockSize; // �����û������ݿ�Ĵ�С(�ֽ�)
	byte *pSET_UserDataBuff; // ָ�룬ָ���û������ݻ�����
	DWORD *pData; // ��ʱʹ�õ�
	DWORD ErrorCode; // ������һ�β�����Ĵ������

	struct
	{
		DWORD Head; // ����ͷ��ָ��
		DWORD End; // ����β��ָ��
		DWORD DataLen; // ��������������ĸ������������*�û�ÿһ�����ݵĴ�С�������������ֽ���
	}

	DataManager; // ���ݹ�����
};

class MyFifoBuff
{
private:

	struct MY_FIFO_DATA_MANAGER_STRUCT hHandle;
	CRITICAL_SECTION hCs; // �̻߳���

public:
	MyFifoBuff::MyFifoBuff(DWORD BlockCount, DWORD BlockSize); // ���캯��
	MyFifoBuff::~MyFifoBuff(void); // ��������

	void MyFifoBuff::InserData(void *Data); // ��������
	void MyFifoBuff::GetData(void *Data); // ȡ������
	DWORD MyFifoBuff::GetDataLen(void); // ȡ���Ѵ�������ݿ�����
	void MyFifoBuff::ClearData(void); // ��մ��������
};

#endif
