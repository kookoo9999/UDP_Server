
// UDP_Server.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CUDP_ServerApp:
// �� Ŭ������ ������ ���ؼ��� UDP_Server.cpp�� �����Ͻʽÿ�.
//

class CUDP_ServerApp : public CWinApp
{
public:
	CUDP_ServerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CUDP_ServerApp theApp;