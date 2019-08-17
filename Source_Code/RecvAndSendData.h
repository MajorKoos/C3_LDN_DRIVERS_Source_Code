// ---------------------------------------------------------------------------

#ifndef RecvAndSendDataH
#define RecvAndSendDataH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>

#include "FIFO_Class_Manager.h"

// USB�豸�Ŷ���
#define		_DEF_USB_DEVICE_VID													0x0483		//�豸ID
#define		_DEF_USB_DEVICE_PID_KEYBOARD_FW										0x8601		//���̵�PID
#define		_DEF_USB_DEVICE_PID_BOOTLOADER_FW									0x8602		//BootLoader��PID
#define		_DEF_USB_DEVICE_KEYBOARD_FW_STRING									"vid_0483&pid_8601&mi_02"//���̵��ַ���
#define     _DEF_USB_DEVICE_BOOTLOADER_FW_STRING                                "vid_0483&pid_8602"  //BootLoader���ַ���

// �豸���Ͷ���
#define		_DEF_USB_CONNECT_TYPE_NONE											0x00		//δ����
#define		_DEF_USB_CONNECT_TYPE_BOOTLOADER									0x01		//���ӵ���BootLoader
#define		_DEF_USB_CONNECT_TYPE_KEYBOARD_FW									0x02		//�����˼��̹̼�

// ��Ϣ
#define  	ON_GET_RECV_DATA  (WM_USER+103) 												//�վ��յ���Ϣ
#define  	ON_HID_DISCONNECT (WM_USER+104) 												//hid���ӱ��Ͽ�
#define		ON_SEND_DATA_SUCCESS (WM_USER+105)												//�߳����ݷ������

// ��Ϣ���صĴ���WParam
#define		ON_SEND_DATA_PARA_SUCCESS											0			//�������ݳɹ�
#define		ON_SEND_DATA_PARA_ERROR												1			//��������ʧ��
#define		ON_SEND_DATA_PARA_DATA_SEND_SUCCESS									2			//���ݷ������
#define		ON_SEND_DATA_PARA_DATA_PROGRESS										3			//���ݷ��͵Ľ���

// �궨��
#define   	_DEF_RECV_DATA_PACK_SIZE                                            65      	//ÿ�����ݰ����64+1�ֽ�
#define		_DEF_RECV_DATA_SIZE													64			//��Ч����
#define   	_DEF_RW_DATA_PACK_SIZE 					                            56      	//ÿ�����ݰ������Ч���ݴ�����

// FIFO��������С
#define		_DEF_USB_RECV_DATA_FIFO_BUFF_BLOCK_COUNT							2048	//���ݽ����̵߳�FIFO��������С
#define		_DEF_USB_SEND_DATA_FIFO_BUFF_BLOCK_COUNT							2048	//���ݷ����̵߳�FIFO��������С

// ʹ�õ����ݽṹ
#pragma pack(1)           //1�ֽڶ���

struct HID_DEF_DATA_PACK_65BYTE
{
	uint8_t ReportID; // USB����ID
	uint8_t PackData[_DEF_RECV_DATA_SIZE]; // ���ݰ�������
};

// �շ�����ʹ�õ����ݽṹ
struct _USB_THREAD_RECV_DATA_STRUCT // ���������߳�ʹ�õ�
{
	HANDLE hHidHandle; // HID�豸���
	HWND hMainFormHandle; // �����ڵľ��
	bool IsEndRun; // �߳��Ƿ���Ҫ������1=�ǣ�0=��
	CRITICAL_SECTION hCs; // �̻߳���
	OVERLAPPED hOl; // �첽�¼�
	MyFifoBuff *RecvFifoBuff; // FIFO���ݹ�����
};

struct _USB_THREAD_SEND_DATA_STRUCT // ���������߳�ʹ�õ�
{
	HANDLE hHidHandle; // HID�豸���
	HWND hMainFormHandle; // �����ھ��
	bool IsEndRun; // �߳��Ƿ���Ҫ������1=�ǣ�0=��
	CRITICAL_SECTION hCs; // �̻߳���
	OVERLAPPED hOl; // �ص�IO�¼�
	bool IsSend; // ���ڷ���=true���������=false
	MyFifoBuff *SendFifoBuff; // FIFO���ݹ�����
};

#pragma pack()

// �̺߳���
// ������
DWORD WINAPI USB_Thread_RecvData(void*lpParam); // ��ȡ�����߳�
DWORD WINAPI USB_Thread_SendData(void*lpParam); // д�������߳�

class MyRwHid
{
private:
	// �����շ�������
	HWND hMainFormHandle; // �����ھ��
	HANDLE hRecvThread; // ���߳̾��
	HANDLE hSendThread; // ���������߳�

	// д�߳̾��
	struct
	{
		// ������صĽṹ��
		byte ConnectType; // ��������0=δ���ӣ�1=Bootloader��2=KeyBoard_Fw
		HANDLE hHidHandle; // ���Ӿ��
	}

	ConnectType;

	struct _USB_THREAD_RECV_DATA_STRUCT RecvDataBuff;
	struct _USB_THREAD_SEND_DATA_STRUCT SendDataBuff;

public:

	MyRwHid(HWND hFormHandle); // ���캯��
	~MyRwHid(void); // ��������

	// ���ݽ��չ���
	void StartRecvThread(void); // ���������߳�
	void GetRecvDataFromFifoBuff(byte *ReportID, void *Data);
	// ��ȡ�յ������ݣ���FIFO������
	DWORD GetRecvDataLen(void); // ��ȡ�������е�����������0=������Ϊ��

	// ���͹���
	void StartSendThread(void); // ���������߳�
	void SendDataByFifo(byte ReportID, void *Data); // ����Ҫ���͵����ݣ�������64�ֽ�
	byte GetSendStatus(void); // ��ȡ����״̬1=���ڷ��ͣ�0=�������

	// �̹߳���
	void StopRecvThread(void); // ֹͣ���߳�
	void StopSendThread(void); // ֹͣд�߳�

	// ��ѯ�Լ�����HID�豸
	bool QueryMyHidDevice(void); // ��ѯHID�豸
	byte GetConnectType(void); // ��ȡ��������
	void SetDisConnect(void); // ����Ϊ�Ͽ�����

private:

};

#endif
