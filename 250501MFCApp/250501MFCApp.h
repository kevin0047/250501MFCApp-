// 250501MFCApp.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"        // 주 기호입니다.

// CMy250501MFCAppApp:
// 이 클래스의 구현에 대해서는 250501MFCApp.cpp을(를) 참조하세요.
//

class CMy250501MFCAppApp : public CWinApp
{
public:
    CMy250501MFCAppApp();

    // 재정의입니다.
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance(); // 이 부분 추가

    // 구현입니다.
protected:
    CShellManager* m_pShellManager = nullptr; // 이 부분 추가

    DECLARE_MESSAGE_MAP()
};

extern CMy250501MFCAppApp theApp;