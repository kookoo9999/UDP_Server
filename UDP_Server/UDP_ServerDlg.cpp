
// UDP_ServerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "UDP_Server.h"
#include "UDP_ServerDlg.h"
#include "afxdialogex.h"

#include <cmath>




#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedExit();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_EXIT, &CAboutDlg::OnBnClickedExit)
END_MESSAGE_MAP()


// CUDP_ServerDlg ��ȭ ����



CUDP_ServerDlg::CUDP_ServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UDP_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUDP_ServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_pic);
}

BEGIN_MESSAGE_MAP(CUDP_ServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND_BTN, &CUDP_ServerDlg::OnBnClickedSendBtn)
	ON_BN_CLICKED(IDC_BUTTON1, &CUDP_ServerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

static void DrawPicture(HWND hWnd)
{
	
}

/*static CString ConvertToHex(CString data)
{
	CString returnvalue;
	for (int x = 0; x < data.GetLength(); x++)
	{
		CString temporary;
		int value = (int)(data[x]);
		returnvalue.Format(_T("%02X ", value));
		returnvalue += temporary;
	}
	return returnvalue;
}*/

//���ϴ� ��ġ�� Idx �� �޾ƿ���
static void GetValueByIndex(void *pPacket, int nPacketSize, int nByteIndex, void *pReturn, int nReturnSize)
{
	ASSERT(nPacketSize > nByteIndex + nReturnSize);

	BYTE *pbyPacket = (BYTE *)pPacket;
	memcpy(pReturn, &pbyPacket[nByteIndex], nReturnSize);
}
/*static unsigned char HexToByte(char hex)
{
	switch (hex)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'A':
	case 'a':
		return 10;
	case 'B':
	case 'b':
		return 11;
	case 'C':
	case 'c':
		return 12;
	case 'D':
	case 'd':
		return 13;
	case 'E':
	case 'e':
		return 14;
	case 'F':
	case 'f':
		return 15;
	}
	return 32;
}*/

/*static unsigned int hextoi(CString strHex)
{
	unsigned char bin[8] = { 0, };
	unsigned int m_nDEC = 0;
	int nLen = strHex.GetLength();
	if (nLen > 8)
		return -1;
	for (int i = 0; i < nLen; i++) {
		bin[i] = HexToByte(strHex[(nLen - 1) - i]);
		if (bin[i] >= 0 && bin[i] <= 15)
		{
			m_nDEC += bin[i] << (4 * i);
		}
		else if (bin[i] == 32)
		{
			return -1;
		}
	}
	return m_nDEC;
}*/



// CUDP_ServerDlg �޽��� ó����
static UINT RecvFunc(LPVOID pVoid)
{
	//namedWindow("point");
	//imshow("point",Mat::zeros(100,100,CV_8UC3));
	DWORD dwWFSObj = 0;
	CUDP_ServerDlg *dlg = (CUDP_ServerDlg *)pVoid;
	
	WSADATA wsa;   // Winsock ������ ����ü
	int retVal = 0;
	int nRecvLen = 0;
	char Buffer[2000] = { 0, };
	
	int arr[100] = { 0, };
	double size = sizeof(Buffer);
	
	int count = 0;
	ST_HEADER st1; //header data �޾ƿͼ� �з�
	ST_HDATA st_oHeader; // data output header �з�
	ST_CONFIG st_config; //config output
	ST_MDATA st_mData; //measurement data output
	
	int pCount = 0; //packet count
	
	//double dX[NOBD] = { 0, };
	//double dY[NOBD] = { 0, };

	
	//WS2_32.DLL �ʱ�ȭ 
	if (WSAStartup(MAKEWORD(2, 2), &wsa) == SOCKET_ERROR)
	{
		WSACleanup();
		return -1;
	}

	dlg->m_ServerSocket = socket(AF_INET, SOCK_DGRAM, 0);  // UDP Socket ���� (SOCK_DGRAM : UDP)

	dlg->m_ServerInfo.sin_family = AF_INET;  // IPv4 
	dlg->m_ServerInfo.sin_addr.s_addr = htonl(INADDR_ANY);  // Local
	dlg->m_ServerInfo.sin_port = htons(6060);
	

	retVal = bind(dlg->m_ServerSocket, (SOCKADDR*)&dlg->m_ServerInfo, sizeof(dlg->m_ServerInfo));

	if (retVal == SOCKET_ERROR)
		return -1;

	int nClient_Size = sizeof(dlg->m_ClientInfo);
	char mData[totalLen] = { 0, }; //��ģ ��Ŷ ����

	while (1)
	{
		//dlg->img = Mat::zeros(1000, 1000, CV_8UC3);
		
		
		// wait for receive . save Client info 
		nRecvLen = recvfrom(dlg->m_ServerSocket, reinterpret_cast<char*>(Buffer), 2000, 0, (SOCKADDR*)&dlg->m_ClientInfo, &nClient_Size);
		//TRACE("type=",typeid(nRecvLen));
		

		int nOffset = 0;
		int mOffset = 24; //without header 24byte

		//read the each packet header 
		memcpy(&st1, &Buffer[nOffset], sizeof(st1));
		nOffset += sizeof(st1);
		
		//thorugh the Fragment Offset , make the total_data_packet

		switch (st1.Fragment_offset)
		{
		case pfirst: //1��° ��Ŷ
			if (pCount != 0) 
			{
				memset(mData, 0, sizeof(mData));
				pCount = 0;
			}
			else
			{
				memcpy(&mData[pfirst], &Buffer[mOffset], datalen);
				pCount++;
			}
			//mOffset += sizeof(Buffer);
			//pCount+=1;
			break;
		case psecond: //2��° ��Ŷ 2:1436 3:2872 4:4308 5:5744 6:7180
			if (pCount != 1)
			{
				memset(mData, 0, sizeof(mData));
				pCount = 0;
			}
			else
			{
				memcpy(&mData[psecond], &Buffer[mOffset], datalen);
				pCount++;
			}
			//mOffset += sizeof(Buffer);
			//pCount += 1;
			break;
		case pthird: //3��° ��Ŷ
			if (pCount != 2)
			{
				memset(mData, 0, sizeof(mData));
				pCount = 0;
			}
			else
			{
				memcpy(&mData[pthird], &Buffer[mOffset], datalen);
				pCount++;
			}
			//mOffset += sizeof(Buffer);
			//pCount += 1;
			break;
		case pfour: //4��° ��Ŷ
			if (pCount != 3)
			{
				memset(mData, 0, sizeof(mData));
				pCount = 0;
			}
			else
			{
				memcpy(&mData[pfour], &Buffer[mOffset], datalen);
				pCount++;
			}
			//mOffset += sizeof(Buffer);
			//pCount += 1;
			break;
		case pfive: //5��° ��Ŷ
			if (pCount != 4)
			{
				memset(mData, 0, sizeof(mData));
				pCount = 0;
			}
			else
			{
				memcpy(&mData[pfive], &Buffer[mOffset], datalen);
				pCount++;
			}
			//mOffset += sizeof(Buffer);
			//pCount += 1;
			break;
		case plast: //������ 6��° ��Ŷ 
			if (pCount != 5)
			{
				memset(mData, 0, sizeof(mData));
				pCount = 0;
			}
			else
			{
				memcpy(&mData[plast], &Buffer[mOffset], lastlen);
				pCount++;
			}
			//pCount += 1;
		default:
			break;
		}
		
		//if complete packet, Put in data to each Structure
		if (pCount == 6)
		{
			dlg->img = Mat::zeros(1000, 1000, CV_8UC3);
			line(dlg->img, Point(500, 0), Point(500, 1000), Scalar(0, 0, 255));
			line(dlg->img, Point(0, 500), Point(1000, 500), Scalar(0, 0, 255));
			circle(dlg->img, Point(500, 500), 10, Scalar(0, 0, 255), 1, 8, 0);
			circle(dlg->img, Point(500, 500), 25, Scalar(0, 0, 255), 1, 8, 0);
			circle(dlg->img, Point(500, 500), 50, Scalar(0, 0, 255), 1, 8, 0);
			circle(dlg->img, Point(500, 500), 100, Scalar(0, 0, 255), 1, 8, 0);
			circle(dlg->img, Point(500, 500), 200, Scalar(0, 0, 255), 1, 8, 0);
			circle(dlg->img, Point(500, 500), 300, Scalar(0, 0, 255), 1, 8, 0);
			circle(dlg->img, Point(500, 500), 400, Scalar(0, 0, 255), 1, 8, 0);
			circle(dlg->img, Point(500, 500), 500, Scalar(0, 0, 255), 1, 8, 0);
			imshow("lidarPoint", dlg->img);
			memcpy(&st_oHeader, &mData, sizeof(st_oHeader));
			//unsigned long time=0;
			//memcpy(&time, &mData[28], 4);

			memcpy(&st_config, &mData[configOff], sizeof(st_config));
			memcpy(&st_mData, &mData[dataOff], 6608);
			//unsigned long d0;
			//memcpy(&d0, &mData[124], 4);			
			//wchar_t d1 = 0;
			//memcpy(&d1, &mData[132], 2);
			wchar_t d_arr[NOBD] = { 0, }; //distance array for each 1651 beams 
					
			//CUDP_ServerDlg dlg;			
			memcpy(&d_arr, &mData[128], sizeof(d_arr));
			double angle = sAngle;
			int Cx = 500;
			int Cy = 500;
			//int r[NOBD] = {0,};
			for (int i = 0; i < NOBD; i++) {
				//r[i] = d_arr[i];
				if (angle >= 360) { angle -= 360; }
				//int Cx=0, Cy=0;
				dlg->dX[i] = cos(angle * opi / 180) * d_arr[i];
				dlg->dY[i] = sin(angle * opi / 180) * d_arr[i];
				/*
				if (angle >= 0 && angle <= 90)  //1��и�
				{
					dlg->dX[i] = abs(cos(angle * opi / 180) * r);
					dlg->dY[i] = abs(sin(angle * opi / 180) * r);
					//circle(dlg->img, Point(Cx + dlg->dX[i] , Cy + dlg->dY[i]), 1, Scalar(255, 255, 0), 1, 8, 0);
					
				}
				else if (angle > 90 && angle < 180) //2��и�
				{
					dlg->dX[i] = -1*abs(cos((angle)*opi / 180) * r);
					dlg->dY[i] = abs(sin((180-angle)*opi / 180) * r);
					//circle(dlg->img, Point(Cx + dlg->dX[i], Cy + dlg->dY[i]), 1, Scalar(255, 255, 0), 1, 8, 0);
				}
				else if (angle > 180 && angle <= eAngle) //3��и�
				{
					dlg->dX[i] = -1*abs(cos((angle)*opi / 180)*r);
					dlg->dY[i] = -1*abs(sin((angle)*opi / 180)*r);
					//circle(dlg->img, Point(Cx + dlg->dX[i], Cy + dlg->dY[i]), 1, Scalar(255, 255, 0), 1, 8, 0);
				}
				else if(angle >= sAngle && angle < 360) //4��и�
				{
					dlg->dX[i] = abs(cos((angle)*opi / 180)*r);
					dlg->dY[i] = -1*abs(sin((angle)*opi / 180)*r);
					//circle(dlg->img, Point(Cx + dlg->dX[i], Cy + dlg->dY[i]), 1, Scalar(255, 255, 0), 1, 8, 0);
				}*/
				circle(dlg->img, Point(Cx + dlg->dX[i], Cy - dlg->dY[i]), 1, Scalar(255, 255, 0), 1, 8, 0);
				imshow("lidarPoint", dlg->img);


				//-------------------���̾�α׿� �Ѹ��°͵�-------------------------------
				//dlg.Draw_Point(i);
				
				

				//TRACE("x[%d]=%d,y[%d]=%d\n", i,(int)dlg->dX[i],i, (int)dlg->dY[i]);

				/*
				HWND hwnd = ::GetDlgItem(dlg->GetSafeHwnd(), IDC_STATIC_PIC);
				CRect rect;
				::GetWindowRect(hwnd, rect);	// �׸� �׸� ������ ũ�� ���ϱ�

				//CStatic *staticSize = (CStatic *)GetDlgItem(hwnd, IDC_STATIC_PIC);
				//staticSize->GetClientRect(rect);

				
				dlg->pWidth = rect.Width() / 2;
				dlg->pHegith = rect.Height() / 2;

				HDC hdc = ::GetDC(hwnd);	// �׸� �׸� �������� HDC ���ϱ�
				HBITMAP hbmp = ::CreateCompatibleBitmap(hdc, rect.Width(), rect.Height());	// �׸� �׸� �������� HBITMAP ���ϱ�
				HBITMAP hbmpOld = (HBITMAP)::SelectObject(hdc, hbmp);	// �׸� �׸� �������� HBITMAP �����ϱ�

				MoveToEx(hdc, dlg->pWidth, dlg->pHegith, NULL);*/
				/*
				if (dlg->dY[i] < 0)
				{
					//SetPixel(hdc, dlg->pWidth + (dlg->dX[i] / 200), dlg->pHegith + (dlg->dY[i] / 200), RGB(0, 0, 0));

					circle(dlg->img, Point( 500 + (int)(dlg->dX[i]/2 ), 500 - (int)(dlg->dY[i]/2 )), 1, Scalar(255, 255, 0), 1, 8, 0);
					
					//LineTo(hdc, dlg->pWidth + (dlg->dX[i]/200), dlg->pHegith + (dlg->dY[i]/200));
				}
				else if(dlg->dY[i]>0)
				{
					//SetPixel(hdc, dlg->pWidth + (dlg->dX[i] / 200), dlg->pHegith + (dlg->dY[i] / 200), RGB(0, 0, 0));

					circle(dlg->img, Point(500 + (int)(dlg->dX[i]/2 ),500- (int)(dlg->dY[i]/2 )), 1, Scalar(255, 255, 0), 1, 8, 0);

					//LineTo(hdc, dlg->pWidth + (dlg->dX[i] / 200), dlg->pHegith - (dlg->dY[i] / 200));
				}*/
				//circle(dlg->img, Point(500+50, 500+50), 2, Scalar(255, 255, 0), 1, 8, 0);

				//imshow("lidarPoint", dlg->img);

				//dlg->m_pic.InvalidateRect(rect, TRUE);
				//dlg->Invalidate(FALSE);
				//dlg->InvalidateRect(rect, TRUE);
				
				//LineTo(hdc, ((int)dlg->dX[i])/100, ((int)dlg->dY[i])/100);
				//::SelectObject(hdc, hbmpOld);	// ���� HBITMAP���� ������
				//::DeleteObject(hbmp);			// ����� HBITMAP �����
				//::ReleaseDC(hwnd, hdc);		// HDC ����
				//::DeleteDC(hdc);				// HDC �����
				
				//find x,y for next beam's 
				angle += rAngular; 

				//imshow("lidarPoint", dlg->img);
				
			}
			
			//After last packet, next packet is 1st
			pCount = 0;
			
			
		}
		//wchar_t offT;
		//memcpy(&offT, &mData[100], 2);
		
		//nOffset += sizeof(st_data1);
		

		/*
		if (nRecvLen > 0)
		{
			// RECV ���� �� MSG ó��.. 
			//TRACE("Type = ", typeid(nRecvLen));
			//TRACE("Length = %d\n", nRecvLen);
			dlg->SetDlgItemInt(IDC_RECV_LENGTH, nRecvLen);
			CString strTemp, strMsg; //���ڿ� ����
			CString HexTemp, HexMsg; //hex ����
			CString TextTemp, TestMsg; //test CString
			CString lengthTemp, lengthMsg; //total length
			unsigned long int T_Length;
			

			for (int i = 0; i < nRecvLen; i++)
			{

				
				

				int anb = 0;

				int short aaa = 0;
				
				strTemp.Format(_T("%c"), Buffer[i]);
				strMsg += strTemp; //abc~~

				//HexTemp.Format(_T("%02x ", Buffer[i]));
				HexTemp.Format(_T("%02x "), Buffer[i]);
				//HexTemp.Format(_T("%02d ", strTemp));
				HexMsg += HexTemp; //02 0d~~

				
				
				/*
				memcpy(&st1.datagram_marker, &Buffer[0], 4);
				//st1.datagram_marker[0] = Buffer[0];
				//TRACE2("data gram marker [%d] = %d\n", st1.datagram_marker[i], Buffer[i]);

				memcpy(&st1.protocol, &Buffer[4], 2); //protocol
				memcpy(&st1.Version_maj, &Buffer[6], 1); //ver maj
				memcpy(&st1.Version_min, &Buffer[7], 1); //ver min

				memcpy(&st1.Total_Length, &Buffer[8], 4); //total length
				memcpy(&st_data1.total_length, &st1.Total_Length, 4); //total length -> data ����ü ����

				//Identification
				memcpy(&st1.Identification, &Buffer[12], 4);
				memcpy(&st_data1.Identification, &st1.Identification, 4);

				//Fragment offset
				memcpy(&st1.Fragment_offset, &Buffer[16], 4);
				memcpy(&st_data1.Fragment_offset, &st1.Fragment_offset, 4);


				//version
				memcpy(&st_data1.Major_Version, &Buffer[25], 1);
				memcpy(&st_data1.Minor_Version, &Buffer[26], 1);
				//serial number
				memcpy(&st_data1.Serial_Num, &Buffer[28], 4);
				memcpy(&st_data1.Serial_Num2, &Buffer[32], 4);
				//channel number
				memcpy(&st_data1.channel_num, &Buffer[36], 1);
				//sequence number
				memcpy(&st_data1.Sequence_num, &Buffer[40], 4);
				//scan number
				memcpy(&st_data1.Scan_num, &Buffer[44], 4);
				
				int nBeams = 0;
				memcpy(&nBeams, &Buffer[120], 4);

				short ndata_size = 0;
				memcpy(&ndata_size, &Buffer[112], 2);

				short ndata_size2 = 0;
				memcpy(&ndata_size2, &Buffer[120], 2);

				short ndata_size3 = 0;
				memcpy(&ndata_size3, &Buffer[122], 2);


				int abc = 0;







				//memcpy(&test_lengh, &st1.Total_Length[0], 2);

				//unsigned short _byteswap_ushort(unsigned short test_lengh);


				//lengthMsg.Format(_T("%02x%02x"), st1.Total_Length[1],st1.Total_Length[0]);

				T_Length = hextoi(lengthMsg);



				//ConvertToHex(lengthMsg);

				//TRACE("Total Legnth = %d\n", total_length);

			}



			int nTotalLength = 0;
			GetValueByIndex(Buffer, nRecvLen, 16, &nTotalLength, sizeof(nTotalLength));
			//TRACE("%d\n", nTotalLength);

			dlg->SetDlgItemText(IDC_RECV_HEX, HexMsg);
			
			dlg->SetDlgItemText(IDC_RECV_EDIT, strMsg);
			


			HexMsg = _T(""); //�ʱ�ȭ
			strMsg = _T("");



		}*/
		
	}

	closesocket(dlg->m_ServerSocket);
	WSACleanup();

	return 0;

}

BOOL CUDP_ServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.
	img = Mat::zeros(1000, 1000, CV_8UC3);
	//circle(img, Point(100, 100), 1, Scalar(255, 255, 0), 1, 8, 0);
	imshow("lidarPoint", img);
	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	pThread = AfxBeginThread(RecvFunc, this);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CUDP_ServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CUDP_ServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		
		CDialogEx::OnPaint();
		/*Mat aaa;
		int cvpoint = 100;
		aaa = Mat::zeros(1000, 1000, CV_8UC3);
		circle(aaa, Point(cvpoint, 100), 0.5, Scalar(0, 0, 255), 5, 8);
		circle(aaa, Point(101, 100), 0.5, Scalar(0, 0, 255), 5, 8);
		imshow("image", aaa);*/
		/*CClientDC dc(GetDlgItem(IDC_STATIC_PIC));
		dc.MoveTo(100, 100);
		dc.LineTo(200, 200);*/
		
		/*CDC* p = m_pic.GetWindowDC();
		CStatic *staticSize = (CStatic *)GetDlgItem(IDC_STATIC_PIC);
		CRect rect;

		staticSize->GetClientRect(rect);
		pWidth = rect.Width()/2;
		pHegith = rect.Height()/2;

		
		
		
		for (int i = 0; i < NOBD; i++) {
			
			p->SetPixel(pWidth + (int)dX[i], pHegith + (int)dY[i], RGB(0, 0, 0));
			TRACE2("x = %d y = %d\n", dX[i], dY[i]);
		}

		p->MoveTo(pWidth, pHegith);
		p->LineTo(pWidth+100, pHegith+100);

		p->MoveTo(0, 0);
		p->LineTo(0+100, 0 + 100);
		
		
		p->SetPixel(pWidth, pHegith, RGB(0, 0, 0));
		p->SetPixel(100, 100, RGB(0, 0, 0));
		m_pic.ReleaseDC(p);
		Invalidate(FALSE);*/
	}
}

void CUDP_ServerDlg::RecvPKC()
{
	RecvFunc(this);
}

void CUDP_ServerDlg::Draw_Point(int i)
{
	CClientDC dc(this);
	
	

	//CUDP_ServerDlg dlg;
	//Mat img;
	

	

	//circle(img, Point((int)dX[i], (int)dY[i]), 0.5, Scalar(0, 0, 255), 5, 8);
	//circle(img, Point((int)333, (int)333), 0.5, Scalar(0, 0, 255), 5, 8);
	//imshow("image", img);
	

	//CClientDC dc(this);
	//dc.SetPixel(dX[i], dY[i], RGB(0, 0, 0));
	/*CWnd *pWnd = AfxGetMainWnd();
	//HWND hWnd = pWnd->m_hWnd;
	HWND hWnd = ::GetDlgItem(m_hWnd, IDC_STATIC_PIC);
	this -> m_hWnd;
	this -> GetSafeHwnd();*/

	/*CClientDC dc(GetDlgItem(IDC_STATIC_PIC));
	CRect rect;
	GetDlgItem(IDC_STATIC_PIC)->GetClientRect(&rect);
	/*CDC* cdc;
	cdc = m_pic.GetDC();
	
	
	pWidth = rect.Width() / 2;
	pHegith = rect.Height() / 2;	*/

	//cdc->SetPixel(100, 100, RGB(0, 0, 0));
	//ReleaseDC(cdc);
	//cdc.SetPixel(100, 100, RGB(0, 0, 0));

	/*for (int i = 0; i < NOBD; i++) {

		p->SetPixel(pWidth + (int)dX[i], pHegith + (int)dY[i], RGB(0, 0, 0));
		TRACE2("x = %d y = %d\n", dX[i], dY[i]);
		ReleaseDC(p);
	}
	
	Invalidate(FALSE);
	*/
}



// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CUDP_ServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUDP_ServerDlg::OnBnClickedSendBtn()
{
	CString strTemp;
	int nSize = 0;
	
	GetDlgItemText(IDC_SEND_EDIT, strTemp);

	for (int i = 0; i < strTemp.GetLength(); i++)
	{
		if ((strTemp.GetAt(i) <= 0) || (strTemp.GetAt(i) > 127))
			nSize += 2;
		else
			nSize += 1;
	}

	char* Buffer = new char[nSize + 1];

	size_t CharactersConverted = 0;

	// �����ڵ� -> �ƽ�Ű�ڵ� (��Ƽ����Ʈ -> �̱۹���Ʈ)
	wcstombs_s(&CharactersConverted, Buffer, nSize + 1, strTemp, _TRUNCATE);


	// Client �ʿ��� 1�� Send�� �� ��� Client ������ ���� Send/Recv ���� 
	int sendSize = sendto(m_ServerSocket, reinterpret_cast<char*>(Buffer), nSize, 0, (struct sockaddr*)&m_ClientInfo, sizeof(m_ClientInfo));

	if (sendSize < 0)
		SetDlgItemText(IDC_SEND_EDIT, _T("���� ����"));

	delete[] Buffer;
}


void CAboutDlg::OnBnClickedExit()
{
	//SendMessage(WM_CLOSE, 0, 0);
	AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_APP_EXIT,NULL);
}


void CUDP_ServerDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	HWND hwnd = ::GetDlgItem(this->GetSafeHwnd(), IDC_STATIC_PIC);	// �׸� �׸� ������ �ڵ��� ���ؼ�
	CRect rect;
	::GetWindowRect(hwnd, rect);	// �׸� �׸� ������ ũ�� ���ϱ�
	
	HDC hdc = ::GetDC(hwnd);	// �׸� �׸� �������� HDC ���ϱ�
	HBITMAP hbmp = ::CreateCompatibleBitmap(hdc, rect.Width(), rect.Height());	// �׸� �׸� �������� HBITMAP ���ϱ�
	HBITMAP hbmpOld = (HBITMAP)::SelectObject(hdc, hbmp);	// �׸� �׸� �������� HBITMAP �����ϱ�

	// todo .. : �簢�� �׸���

	// todo .. : �� �׸���

	// todo .. : �ٰ��� �׸���

	// todo .. : �� �׸���
	//MoveToEx(hdc, 0, 0,NULL);
	//LineTo(hdc, 100, 100);
	//for (int i = 0; i < NOBD; i++) {
	//	SetPixel(hdc, dX[i], dY[i], RGB(0, 0, 0));
	//}
	//::SelectObject(hdc, hbmpOld);	// ���� HBITMAP���� ������

	SetPixel(hdc, pWidth, pHegith,NULL);
	MoveToEx(hdc, pWidth, pHegith, NULL);
	LineTo(hdc, pWidth+30,pHegith+60);
	LineTo(hdc, pWidth + 60, pHegith - 60);
	::DeleteObject(hbmp);			// ����� HBITMAP �����
	::ReleaseDC(hwnd, hdc);		// HDC ����
	::DeleteDC(hdc);				// HDC �����
	//Invalidate(true);

}
