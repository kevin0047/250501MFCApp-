#pragma once
#include <afxsock.h>

// C250501MFCAppDlg 대화 상자
class C250501MFCAppDlg : public CDialogEx
{
    // 생성입니다.
public:
    C250501MFCAppDlg(CWnd* pParent = nullptr);    // 표준 생성자입니다.

    // 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MY250501MFCAPP_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    // 구현입니다.
protected:
    HICON m_hIcon;

    // 생성된 메시지 맵 함수
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedButtonSend();

    // 소켓 통신을 위한 변수
    CSocket m_socket;

    // 컨트롤 변수
    CString m_strIP;
    int m_nPort;
    int m_nValue;

    // XGT 프로토콜 관련 함수
    BOOL ConnectToPlc();
    void DisconnectFromPlc();
    BOOL WriteWordToPlc(int nValue);

    // 메시지 로그 출력
    void AddLogMessage(LPCTSTR lpszMessage);
    CListBox m_listLog;
};