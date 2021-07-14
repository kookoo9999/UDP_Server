
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����


#include <afxsock.h>            // MFC ���� Ȯ��

#include <typeinfo>

#include <intrin.h>

typedef struct _ST_HEADER
{
	unsigned char datagram_marker[4];
	unsigned char protocol[2];
	//wchar_t protocol;
	unsigned char Version_maj;
	unsigned char Version_min;
	unsigned int Total_Length;
	unsigned int Identification;
	unsigned int Fragment_offset;
	unsigned char Reserve[4];
	 

} ST_HEADER;

//Data_output_Header
typedef struct _ST_DATA
{
	//unsigned short total_length = 0;
	//unsigned short Identification = 0;
	//unsigned short Fragment_offset = 0;
	//------------------------------------- st_header���� Ȯ�ΰ���..
	// Version info
	unsigned char Version; 
	unsigned char Major_Version;
	unsigned char Minor_Version;
	unsigned char Release;
	//device serial num
	unsigned long Serial_Num; 
	//device serial num2
	unsigned long Serial_Num2; //serial num for system plug 
	//channel number
	unsigned char channel_num;
	unsigned char Reserve1[3];
	//Sequence number
	unsigned long Sequence_num;
	//Scan number
	unsigned long Scan_num;
	//Time data
	wchar_t tDate;      //time data date
	unsigned char tReserve[2]; //titme data reserve
	unsigned long tTime; //time data time

	//Block device status
	wchar_t Status_offset; 
	wchar_t Status_size;
	//Block configuration of data output
	wchar_t Config_offset;
	wchar_t Config_size;
	//Block measurement data
	wchar_t mData_offset;
	wchar_t mData_size;
	//Block field interruption
	wchar_t fInter_offset;
	wchar_t fInter_size;
	//Blcok application data
	wchar_t AppData_offset;
	wchar_t AppData_size;
	//Block Local_I/O
	wchar_t LIO_offset;
	wchar_t LIO_size;
	
	
	
	
}ST_HDATA;

typedef struct _ST_BLOCK_STATUS
{
	//Data from assembly 113 device status
	unsigned char dStatus[16];

}ST_STATUS;

typedef struct _ST_BLOCK_CONFIG
{
	//Configuration of the data output
	wchar_t Factor;
	wchar_t cNumberOfBeams; //config NOB
	wchar_t Scan_Cycle_Time; //ms
	unsigned char Reserved_config[2];
	long Start_Angle; //devided by 4194304
	long Angular_Resolution; //devided by 4194304
	unsigned long Beam_Interval; //us
	unsigned char Reserved_confing2[4];

}ST_CONFIG;
/*
typedef struct _ST_BLOCK_DATA
{
	unsigned long dNumberOfBeams; 
	wchar_t Distance; //mm
	unsigned char RSSI;

}ST_MDATA;*/

typedef struct _ST_1
{
	wchar_t Distance; //mm
	unsigned char RSSI;
	unsigned char Status;

}ST_1;

typedef struct _ST_BLOCK_DATA
{
	unsigned long dNumberOfBeams;
	ST_1 *pData;

}ST_MDATA;

#include "opencv2/opencv.hpp"
#include <afxwin.h>
using namespace cv;

/*typedef struct _ST_DATA
{

}ST_DATA;*/



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


