
// UDP_ServerDlg.h : 헤더 파일
//

#pragma once
//--------length
#define bufLen 1460
#define datalen 1436
#define lastlen 199
#define totalLen 7379
//packet oreder 1st=0
#define pfirst 0
#define psecond 1436 
#define pthird 2872 
#define pfour 4308 
#define pfive 5744
#define plast 7180
//packet block data offset
#define configOff 96
#define dataOff 124 

//number of beam
#define NOBD 1651

//Angular Range
#define rAngular 0.1665657177468201

//Start Angle
#define sAngle 312.5 //-47.5 
#define eAngle 227.5
#define tAngle 275

//pi
#define opi 3.1415926

// CUDP_ServerDlg 대화 상자
class CUDP_ServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CUDP_ServerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UDP_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSendBtn();
	afx_msg void OnPaint();
	SOCKET m_ServerSocket;
	
	SOCKADDR_IN m_ServerInfo;
	SOCKADDR_IN m_ClientInfo;
	
	void RecvPKC();
	void Draw_Point(int i);
	CWinThread* pThread;
	CStatic m_pic;
	
	Mat img;

	double dX[NOBD];
	double dY[NOBD];

	int pWidth;// = rect.Width() / 2;
	int pHegith;// = rect.Height() / 2;

	int abc;
	int ddd;
	
	

	afx_msg void OnBnClickedButton1();
};
