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
    afx_msg void OnBnClickedButtonConnect();  // 연결 버튼 클릭 이벤트 핸들러

    // 소켓 통신을 위한 변수
    CSocket m_socket;
    BOOL m_bConnected;  // 연결 상태를 저장하는 변수

    // 컨트롤 변수
    CString m_strIP;
    int m_nPort;
    int m_nValue;
    CStatic m_staticConnectionStatus;  // 연결 상태 표시 Static 컨트롤
    CString m_strMemoryAddress; // 메모리 주소를 저장할 변수 추가

    // XGT 프로토콜 관련 함수
    BOOL ConnectToPlc();
    void DisconnectFromPlc();
    BOOL WriteWordToPlc(int nValue);
    void UpdateConnectionStatus();  // 연결 상태 업데이트 함수

    // 메시지 로그 출력
    void AddLogMessage(LPCTSTR lpszMessage);
    CListBox m_listLog;
public:
    afx_msg void OnBnClickedButtonRead();  // 읽기 버튼 클릭 이벤트 핸들러

    // 메모리 읽기 관련 변수 추가
    int m_nReadValue;       // 읽은 값을 저장할 변수
    CString m_strReadMemoryAddress;  // 읽을 메모리 주소

    // XGT 프로토콜 읽기 함수 추가
    BOOL ReadWordFromPlc(LPCTSTR lpszMemAddress, int& nReadValue);
};