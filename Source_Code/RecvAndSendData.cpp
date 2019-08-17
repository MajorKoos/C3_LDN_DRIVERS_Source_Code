//---------------------------------------------------------------------------

#pragma hdrstop

#include "RecvAndSendData.h"

extern "C"
{
#include "setupapi.h"
#include "hiddll.h"
}
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "hid.lib")

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
MyRwHid::MyRwHid(HWND hFormHandle)//���캯��
{
	memset(&RecvDataBuff, 0x00, sizeof(RecvDataBuff)); //��ջ�����
	memset(&SendDataBuff, 0x00, sizeof(SendDataBuff));
	memset(&ConnectType, 0x00, sizeof(ConnectType));
	hMainFormHandle = hFormHandle; //�����ڵľ��

	//�����̻߳�������
	InitializeCriticalSection(&RecvDataBuff.hCs); //��ʼ��������
	RecvDataBuff.hOl.hEvent = CreateEvent(0, false, false, NULL); //����һ���¼�
	RecvDataBuff.hMainFormHandle = hMainFormHandle;
	//����һ�����ն��л�����
	RecvDataBuff.RecvFifoBuff = new MyFifoBuff(_DEF_USB_RECV_DATA_FIFO_BUFF_BLOCK_COUNT, _DEF_RECV_DATA_PACK_SIZE);

	//�����̻߳�������
	InitializeCriticalSection(&SendDataBuff.hCs); //��ʼ��������
	SendDataBuff.hOl.hEvent = CreateEvent(0, false, false, NULL); //����һ���¼�
	SendDataBuff.hMainFormHandle = hMainFormHandle;
	SendDataBuff.SendFifoBuff = new MyFifoBuff(_DEF_USB_RECV_DATA_FIFO_BUFF_BLOCK_COUNT, _DEF_RECV_DATA_PACK_SIZE);
}

//---------------------------------------------------------------------------
MyRwHid::~MyRwHid(void)//��������
{
	/*
	 if (WaitForSingleObject(hRecvThread, 200) != WAIT_FAILED) // �ȴ����������߳��˳� INFINITE
	 {
	 CloseHandle(hRecvThread);
	 }
	 if (WaitForSingleObject(hSendThread, 200) != WAIT_FAILED) // �ȴ����������߳��˳�
	 {
	 CloseHandle(hSendThread);
	 }
	 */
	DeleteCriticalSection(&SendDataBuff.hCs);
	DeleteCriticalSection(&RecvDataBuff.hCs);
	delete RecvDataBuff.RecvFifoBuff;
}

//---------------------------------------------------------------------------
void MyRwHid::StartRecvThread(void)//���������߳�
{

	if(ConnectType.ConnectType == _DEF_USB_CONNECT_TYPE_NONE)
	{
		return;
	}//���δ���ӣ��򲻻������߳�

	if(WaitForSingleObject(hRecvThread, INFINITE) != WAIT_FAILED)
	{
		CloseHandle(hRecvThread);
	}
	RecvDataBuff.hHidHandle = ConnectType.hHidHandle; //���
	hRecvThread = CreateThread(NULL, 0, USB_Thread_RecvData, (VOID *) &RecvDataBuff, 0, NULL);

}

//---------------------------------------------------------------------------
void MyRwHid::GetRecvDataFromFifoBuff(byte *ReportID, void *Data)
	//��ȡ�յ������ݣ���FIFO������
{
	struct HID_DEF_DATA_PACK_65BYTE Pack;

	RecvDataBuff.RecvFifoBuff->GetData(&Pack); //�ӻ�������ȡ������

	*ReportID = Pack.ReportID; //���ر���ID
	memcpy(Data, Pack.PackData, sizeof(Pack.PackData)); //�������ݳ���
}

//---------------------------------------------------------------------------
DWORD MyRwHid::GetRecvDataLen(void)//��ȡ�������е�����������0=������Ϊ��
{
	DWORD DataLen;
	DataLen = RecvDataBuff.RecvFifoBuff->GetDataLen();
	return (DataLen);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void MyRwHid::StartSendThread(void)//���������߳�
{

	if(ConnectType.ConnectType == _DEF_USB_CONNECT_TYPE_NONE)
	{
		return;
	}//���δ���ӣ��򲻻������߳�


	if(WaitForSingleObject(hSendThread, 500) != WAIT_FAILED)		//�ȴ��ϴη��͵��߳̽��� INFINITE
	{
		CloseHandle(hSendThread); //�ر��̵߳ľ��
	}

	SendDataBuff.hHidHandle = ConnectType.hHidHandle; //����HID_Device�ľ��
	SendDataBuff.IsSend = 1; //����Ϊ���ڷ���״̬
	hSendThread = CreateThread(NULL, 0, USB_Thread_SendData, (VOID *) &SendDataBuff, 0, NULL);

}

//---------------------------------------------------------------------------
void MyRwHid::SendDataByFifo(byte ReportID, void *Data)//����Ҫ���͵����ݣ�������64�ֽ�
{
	struct HID_DEF_DATA_PACK_65BYTE Pack;

	if(ConnectType.ConnectType == _DEF_USB_CONNECT_TYPE_NONE)
	{
		return;
	}//���δ���ӣ��򲻻�����ݷ��뻺���������ݱ���ʧ

	Pack.ReportID = ReportID; //���ñ���ID
	memcpy(&Pack.PackData, Data, sizeof(Pack.PackData)); //����Ҫ���͵����ݵ�������
	SendDataBuff.SendFifoBuff->InserData(&Pack); //�����ݲ��뻺����
}

//---------------------------------------------------------------------------
byte MyRwHid::GetSendStatus(void)//��ȡ����״̬1=���ڷ��ͣ�0=�������
{
	byte status;
	EnterCriticalSection(&SendDataBuff.hCs); //�����̻߳���
	status = SendDataBuff.IsSend;
	LeaveCriticalSection(&SendDataBuff.hCs); //�뿪�̻߳���
	return (status);
}

//---------------------------------------------------------------------------
void MyRwHid::StopRecvThread(void)//ֹͣ��д�߳�
{
	EnterCriticalSection(&RecvDataBuff.hCs); //���뻥��
	RecvDataBuff.IsEndRun = 1; //���������̵߳�����
	LeaveCriticalSection(&RecvDataBuff.hCs); //�뿪����
	SetEvent(RecvDataBuff.hOl.hEvent); //�˹�����Ϊ���źţ���ʹ��������ֹͣ����
	Sleep(50);

}

//---------------------------------------------------------------------------
void MyRwHid::StopSendThread(void)//ֹͣд�߳�
{
	EnterCriticalSection(&SendDataBuff.hCs); //���뻥��
	SendDataBuff.IsEndRun = 1; //���������̵߳�����
	LeaveCriticalSection(&SendDataBuff.hCs); //�뿪����
	SetEvent(SendDataBuff.hOl.hEvent); //�˹�����Ϊ���źţ���ʹ��������ֹͣ����
	Sleep(50);

}

//---------------------------------------------------------------------------
DWORD WINAPI USB_Thread_RecvData(void *lpParam)//��ȡ�����߳�
{
	struct _USB_THREAD_RECV_DATA_STRUCT *RecvBuff;
	struct HID_DEF_DATA_PACK_65BYTE Pack;
	DWORD ReadFileRecvSize;
	DWORD LASTERROR;
	DWORD errs;

	RecvBuff = (struct _USB_THREAD_RECV_DATA_STRUCT *)lpParam; //����

	while(1)
	{
		memset(&Pack, 0x00, sizeof(Pack)); //��ս��ջ�����

		//ʹ���ص�IO��ȡ����
		ReadFile(RecvBuff->hHidHandle, (byte *) &Pack, sizeof(Pack), &ReadFileRecvSize, &RecvBuff->hOl);
		//�ȴ��������
		LASTERROR = GetLastError();

		if(LASTERROR == ERROR_IO_PENDING)
		{
			WaitForSingleObject(RecvBuff->hOl.hEvent, INFINITE); //�ȴ��������
		}
		else //����Ѿ�ʧЧ,Ӧ���˳��߳�
		{
			CloseHandle(RecvBuff->hHidHandle); //�ر�HID���
			//Ӧ��֪ͨ���̣߳����ӶϿ���
			PostMessage(RecvBuff->hMainFormHandle, ON_HID_DISCONNECT, 0, 0);
			//֪ͨ���̣߳������Ѿ��Ͽ�
			break;
		}
		//���յ������ݲ��뻺����
		EnterCriticalSection(&RecvBuff->hCs); //���뻥��
		if(RecvBuff->IsEndRun == 1)//���Ҫ���������߳�
		{
			LeaveCriticalSection(&RecvBuff->hCs); //�뿪����
			break;
		}
		LeaveCriticalSection(&RecvBuff->hCs); //�뿪����
		RecvBuff->RecvFifoBuff->InserData(&Pack);
		PostMessage(RecvBuff->hMainFormHandle, ON_GET_RECV_DATA, 0, 0);
	}
	return (0);
}

//---------------------------------------------------------------------------
DWORD WINAPI USB_Thread_SendData(void *lpParam)//д�������߳�
{

	struct _USB_THREAD_SEND_DATA_STRUCT *SendBuff = (struct _USB_THREAD_SEND_DATA_STRUCT *)lpParam;
	DWORD WriteStatus; //WriteFile�������ص�����
	DWORD WriteFileSize; //WriteFile����д����ֽ���
	DWORD SendPack = 0; //�Ѿ����͵����ݴ�С
	struct HID_DEF_DATA_PACK_65BYTE Pack; //Ҫ���͵����ݰ�

	//�������ݰ�
	while(1)//ֱ���������
	{

		if(SendBuff->IsEndRun == 1)//���Ҫ�����̵߳ķ���
		{
			EnterCriticalSection(&SendBuff->hCs); //���뻥��
			SendBuff->IsSend = 0; //�������
			LeaveCriticalSection(&SendBuff->hCs); //�뿪����
			break;
		}

		if(SendBuff->SendFifoBuff->GetDataLen() == 0)//������ݷ������
		{
			EnterCriticalSection(&SendBuff->hCs); //���뻥��
			SendBuff->IsSend = 0; //�������
			LeaveCriticalSection(&SendBuff->hCs); //�뿪����
			break;
		}

		SendBuff->SendFifoBuff->GetData(&Pack); //ȡ������
		//�������ݣ�ͨ���ص�IO

		WriteStatus = WriteFile(SendBuff->hHidHandle, //���
			(byte *) &Pack, //���ݻ�����
			sizeof(Pack), //���ݴ�С
			&WriteFileSize, //�ѷ��ʹ�С
			&SendBuff->hOl); //ʹ���ص�IO

		if((WriteStatus == false) && (GetLastError() == ERROR_IO_PENDING))
			//����Ƿ����
		{
			WaitForSingleObject(SendBuff->hOl.hEvent, INFINITE); //�ȴ����ݷ������
		}
		else //�������Ѿ���Ĳ����ã��������˳��߳�
		{
			EnterCriticalSection(&SendBuff->hCs); //���뻥��
			SendBuff->IsSend = 0; //�������
			LeaveCriticalSection(&SendBuff->hCs); //�뿪����
			CloseHandle(SendBuff->hHidHandle); //�ر�HID���
			//Ӧ��֪ͨ���̣߳����ӶϿ���
			PostMessage(SendBuff->hMainFormHandle, ON_HID_DISCONNECT, 0, 0);
			//֪ͨ���̣߳������Ѿ��Ͽ�
			PostMessage(SendBuff->hMainFormHandle, ON_SEND_DATA_SUCCESS, ON_SEND_DATA_PARA_ERROR, 0); //���ݷ���ʧ��֪ͨ
			return (0); //�����߳�
		}
		//���͵�ǰ�Ľ��ȸ����߳�
		PostMessage(SendBuff->hMainFormHandle, ON_SEND_DATA_SUCCESS, ON_SEND_DATA_PARA_DATA_PROGRESS, ++SendPack);
	}

	PostMessage(SendBuff->hMainFormHandle, ON_SEND_DATA_SUCCESS, ON_SEND_DATA_PARA_DATA_SEND_SUCCESS, 0); //���ݷ������֪ͨ
	EnterCriticalSection(&SendBuff->hCs); //���뻥��
	SendBuff->IsSend = 0; //�������
	LeaveCriticalSection(&SendBuff->hCs); //�뿪����
	return (0);
}

//---------------------------------------------------------------------------
bool MyRwHid::QueryMyHidDevice(void)//��ѯHID�豸
{
	GUID hidGuid;
	String DeviceString;
	HDEVINFO hDevInfo;
	ULONG requiredLength = 0;
	int deviceNo = 0;

	if(ConnectType.ConnectType != 0)//����Ѿ����ӣ���������
	{
		return (false);
	}
	HidD_GetHidGuid(&hidGuid);
	hDevInfo = SetupDiGetClassDevs(&hidGuid, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));

	if(hDevInfo == INVALID_HANDLE_VALUE)//��ѯʧ��
	{
		return (false);
	}
	SP_DEVICE_INTERFACE_DATA devInfoData;
	devInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	deviceNo = 0;
	SetLastError(0);
	while(GetLastError() != ERROR_NO_MORE_ITEMS)//���û�и����HID�豸�����˳�ѭ��
	{
		if(SetupDiEnumInterfaceDevice(hDevInfo, 0, &hidGuid, deviceNo, &devInfoData))//ö��ϵͳ�ڵ�USB HID�豸
		{
			requiredLength = 0;
			SetupDiGetInterfaceDeviceDetail(hDevInfo, &devInfoData, NULL, 0, &requiredLength, NULL); //��ȡ��Ҫ�����ݿռ��С
			PSP_INTERFACE_DEVICE_DETAIL_DATA devDetail = (SP_INTERFACE_DEVICE_DETAIL_DATA *)malloc(requiredLength);
			//��̬�����ڴ�
			devDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
			if(!SetupDiGetInterfaceDeviceDetail(hDevInfo, &devInfoData, devDetail, requiredLength, NULL, NULL))//�ٴε��ã��Ի�ȡ���õ�����
			{
				free(devDetail); //ʧ���˾��ͷŷ�����ڴ�
				SetupDiDestroyDeviceInfoList(hDevInfo);
				return (false); //����
			}
			DeviceString = devDetail->DevicePath; //�ɹ��ˣ���ȡ�豸�ľ���·��
			if((DeviceString.Pos(_DEF_USB_DEVICE_KEYBOARD_FW_STRING) != 0) || (DeviceString.Pos(_DEF_USB_DEVICE_BOOTLOADER_FW_STRING) != 0))
				//�����豸ID��������ԣ��ͼ�����һ��������
			{
				ConnectType.hHidHandle = CreateFile(devDetail->DevicePath, //�ص�IOģʽ���豸
					GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
			}
			else
			{
				++deviceNo; //���û����Ҫ���豸���������һ��
				continue;
			}

			free(devDetail); //�ͷ��豸��Ϣ������ڴ�
			if(ConnectType.hHidHandle == INVALID_HANDLE_VALUE)//����豸��ʧ��
			{
				SetupDiDestroyDeviceInfoList(hDevInfo);
				free(devDetail);
				return (false);
			}
			_HIDD_ATTRIBUTES hidAttributes;
			if(!HidD_GetAttributes(ConnectType.hHidHandle, &hidAttributes))
				//��ȡ�豸������
			{
				CloseHandle(ConnectType.hHidHandle);
				SetupDiDestroyDeviceInfoList(hDevInfo);
				return (false);
			}
			if((_DEF_USB_DEVICE_VID == hidAttributes.VendorID) && (_DEF_USB_DEVICE_PID_KEYBOARD_FW == hidAttributes.ProductID))//�ж���KB_FW
			{
				ConnectType.ConnectType = _DEF_USB_CONNECT_TYPE_KEYBOARD_FW;
				//�����Ӽ��̹̼�
				SetupDiDestroyDeviceInfoList(hDevInfo);
				//�������Ҫ�ģ���ô�ͷű����豸��Ϣ��������ڴ�
				return true; //������ȷ
			}
			if((_DEF_USB_DEVICE_VID == hidAttributes.VendorID) && (_DEF_USB_DEVICE_PID_BOOTLOADER_FW == hidAttributes.ProductID))//�ж���BootLoader_FW
			{
				ConnectType.ConnectType = _DEF_USB_CONNECT_TYPE_BOOTLOADER;
				//�����Ӽ��̹̼�
				SetupDiDestroyDeviceInfoList(hDevInfo);
				//�������Ҫ�ģ���ô�ͷű����豸��Ϣ��������ڴ�
				return true; //������ȷ
			}

			//�����ǣ��������һ��
			CloseHandle(ConnectType.hHidHandle);
			//����رմ򿪺�ľ����������ѯ��һ��USB HID�豸����Ϣ
			++deviceNo;
		}
	}
	SetupDiDestroyDeviceInfoList(hDevInfo); //���û���ҵ��κ���Ҫ��USB HID�豸��������ͷŷ�����ڴ�
	return false; //������Ч�ľ��
}

//---------------------------------------------------------------------------
byte MyRwHid::GetConnectType(void)//��ȡ��������
{
	return (ConnectType.ConnectType);
}

//---------------------------------------------------------------------------
void MyRwHid::SetDisConnect(void)//����Ϊ�Ͽ�����
{
	ConnectType.ConnectType = _DEF_USB_CONNECT_TYPE_NONE;
	ConnectType.hHidHandle = NULL;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
