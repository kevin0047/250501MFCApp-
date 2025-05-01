#include "pch.h"
#include "framework.h"
#include "250501MFCApp.h"
#include "250501MFCAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    // 구현입니다.
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// C250501MFCAppDlg 대화 상자
C250501MFCAppDlg::C250501MFCAppDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MY250501MFCAPP_DIALOG, pParent)
    , m_strIP(_T(""))
    , m_nPort(2004)  // XGT 기본 포트 2004
    , m_nValue(0)
    , m_bConnected(FALSE)  // 연결 상태 초기화
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C250501MFCAppDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_IP, m_strIP);
    DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
    DDX_Text(pDX, IDC_EDIT_VALUE, m_nValue);
    DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
    DDX_Control(pDX, IDC_STATIC_CONNECTION_STATUS, m_staticConnectionStatus);
}

BEGIN_MESSAGE_MAP(C250501MFCAppDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_SEND, &C250501MFCAppDlg::OnBnClickedButtonSend)
    ON_BN_CLICKED(IDC_BUTTON_CONNECT, &C250501MFCAppDlg::OnBnClickedButtonConnect)
END_MESSAGE_MAP()

// C250501MFCAppDlg 메시지 처리기
// C250501MFCAppDlg 메시지 처리기
BOOL C250501MFCAppDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 이 대화 상자의 아이콘을 설정합니다.
    SetIcon(m_hIcon, TRUE);        // 큰 아이콘을 설정합니다.
    SetIcon(m_hIcon, FALSE);       // 작은 아이콘을 설정합니다.

    // 소켓 초기화
    AfxSocketInit();

    // 기본 IP 주소 설정
    m_strIP = _T("192.168.0.2");
    UpdateData(FALSE);

    // 연결 상태 초기화
    m_bConnected = FALSE;
    UpdateConnectionStatus();

    AddLogMessage(_T("프로그램이 시작되었습니다. IP 주소와 포트를 입력한 후 연결 버튼을 누르세요."));

    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void C250501MFCAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void C250501MFCAppDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 아이콘을 그립니다.
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR C250501MFCAppDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// 로그 메시지 추가
void C250501MFCAppDlg::AddLogMessage(LPCTSTR lpszMessage)
{
    CString strTime;
    SYSTEMTIME st;
    GetLocalTime(&st);
    strTime.Format(_T("[%02d:%02d:%02d] "), st.wHour, st.wMinute, st.wSecond);

    CString strMsg = strTime + lpszMessage;
    m_listLog.AddString(strMsg);
    m_listLog.SetTopIndex(m_listLog.GetCount() - 1);
}

// PLC 연결
BOOL C250501MFCAppDlg::ConnectToPlc()
{
    // 이미 연결되어 있으면 그대로 반환
    if (m_socket.m_hSocket != INVALID_SOCKET)
    {
        AddLogMessage(_T("이미 PLC에 연결되어 있습니다."));
        return TRUE;
    }

    // 소켓 생성
    if (!m_socket.Create())
    {
        AddLogMessage(_T("소켓 생성에 실패했습니다."));
        return FALSE;
    }

    // PLC 연결
    if (!m_socket.Connect(m_strIP, m_nPort))
    {
        int nError = GetLastError();
        CString strError;
        strError.Format(_T("PLC 연결 실패. 오류 코드: %d"), nError);
        AddLogMessage(strError);
        m_socket.Close();
        return FALSE;
    }

    AddLogMessage(_T("PLC에 연결되었습니다."));
    return TRUE;
}


// PLC 연결 해제
void C250501MFCAppDlg::DisconnectFromPlc()
{
    if (m_socket.m_hSocket != INVALID_SOCKET)
    {
        m_socket.Close();
        AddLogMessage(_T("PLC 연결이 해제되었습니다."));
    }
}


// PLC에 워드 값 쓰기 (XGT 프로토콜)
BOOL C250501MFCAppDlg::WriteWordToPlc(int nValue)
{
    // XGT 프로토콜 헤더 및 데이터 구성
    const int BUFFER_SIZE = 512;
    BYTE sendBuffer[BUFFER_SIZE] = { 0, };

    // Company ID (LSIS-XGT)
    sendBuffer[0] = 0x4C;  // 'L'
    sendBuffer[1] = 0x53;  // 'S'
    sendBuffer[2] = 0x49;  // 'I'
    sendBuffer[3] = 0x53;  // 'S'
    sendBuffer[4] = 0x2D;  // '-'
    sendBuffer[5] = 0x58;  // 'X'
    sendBuffer[6] = 0x47;  // 'G'
    sendBuffer[7] = 0x54;  // 'T'
    sendBuffer[8] = 0x00;  // '\0'
    sendBuffer[9] = 0x00;  // '\0'

    // PLC Info (Don't care)
    sendBuffer[10] = 0x00;
    sendBuffer[11] = 0x00;

    // CPU Info
    sendBuffer[12] = 0xA0;

    // Source of Frame (PC -> PLC)
    sendBuffer[13] = 0x33;

    // Invoke ID (임의 값)
    sendBuffer[14] = 0x01;
    sendBuffer[15] = 0x00;

    // Length (Application Instruction의 바이트 크기)
    int dataLength = 20;  // 명령어(2) + 데이터타입(2) + 예약영역(2) + 블록수(2) + 변수길이(2) + 변수(8) + 데이터개수(2) + 데이터(2)
    sendBuffer[16] = (BYTE)(dataLength & 0xFF);
    sendBuffer[17] = (BYTE)((dataLength >> 8) & 0xFF);

    // FEnet Position (슬롯 0, 베이스 0)
    sendBuffer[18] = 0x00;

    // Reserved2 (BCC)
    sendBuffer[19] = 0x00;

    // Command (쓰기 요청: 0x0058)
    sendBuffer[20] = 0x58;
    sendBuffer[21] = 0x00;

    // Data Type (워드: 0x0002)
    sendBuffer[22] = 0x02;
    sendBuffer[23] = 0x00;

    // 예약 영역
    sendBuffer[24] = 0x00;
    sendBuffer[25] = 0x00;

    // 블록 수 (1개의 블록)
    sendBuffer[26] = 0x01;
    sendBuffer[27] = 0x00;

    // 변수 길이 (%DW6000 - 7자)
    sendBuffer[28] = 0x07;
    sendBuffer[29] = 0x00;

    // 변수 (%DW6000)
    sendBuffer[30] = '%';
    sendBuffer[31] = 'D';
    sendBuffer[32] = 'W';
    sendBuffer[33] = '6';
    sendBuffer[34] = '0';
    sendBuffer[35] = '0';
    sendBuffer[36] = '0';

    // 데이터 크기 (2바이트)
    sendBuffer[37] = 0x02;
    sendBuffer[38] = 0x00;

    // 데이터 (nValue)
    sendBuffer[39] = (BYTE)(nValue & 0xFF);
    sendBuffer[40] = (BYTE)((nValue >> 8) & 0xFF);

    // 데이터 전송
    int totalSize = dataLength + 20;  // 헤더(20) + 데이터길이
    int sendSize = m_socket.Send(sendBuffer, totalSize);

    if (sendSize != totalSize)
    {
        AddLogMessage(_T("데이터 전송에 실패했습니다."));
        return FALSE;
    }

    // 응답 대기
    BYTE recvBuffer[BUFFER_SIZE] = { 0, };
    int recvSize = m_socket.Receive(recvBuffer, BUFFER_SIZE);

    if (recvSize <= 0)
    {
        AddLogMessage(_T("응답을 받지 못했습니다."));
        return FALSE;
    }

    // 응답 확인
    BOOL bSuccess = FALSE;

    // 응답이 충분한 길이인지 확인
    if (recvSize >= 24)
    {
        // 명령어 확인 (0x0059: 쓰기 응답)
        if (recvBuffer[20] == 0x59 && recvBuffer[21] == 0x00)
        {
            // 에러 상태 확인
            if (recvBuffer[24] == 0x00 && recvBuffer[25] == 0x00)
            {
                bSuccess = TRUE;
                AddLogMessage(_T("D6000에 값을 성공적으로 썼습니다."));
            }
            else
            {
                CString strError;
                strError.Format(_T("PLC 오류: 0x%02X%02X"), recvBuffer[25], recvBuffer[24]);
                AddLogMessage(strError);
            }
        }
        else
        {
            AddLogMessage(_T("명령어 응답이 잘못되었습니다."));
        }
    }
    else
    {
        AddLogMessage(_T("응답 길이가 너무 짧습니다."));
    }

    return bSuccess;
}

// 전송 버튼 클릭 처리
void C250501MFCAppDlg::OnBnClickedButtonSend()
{
    UpdateData(TRUE);  // 컨트롤 값 가져오기

    // 연결 상태 확인
    if (!m_bConnected)
    {
        MessageBox(_T("PLC에 연결되어 있지 않습니다. 먼저 연결 버튼을 클릭하여 연결하세요."), _T("연결 필요"), MB_ICONINFORMATION);
        return;
    }

    CString strMsg;
    strMsg.Format(_T("D6000에 값 %d을(를) 쓰려고 합니다."), m_nValue);
    AddLogMessage(strMsg);

    // 데이터 쓰기
    if (WriteWordToPlc(m_nValue))
    {
        strMsg.Format(_T("값 %d을(를) D6000에 성공적으로 썼습니다."), m_nValue);
        AddLogMessage(strMsg);
    }
    else
    {
        AddLogMessage(_T("데이터 쓰기에 실패했습니다."));
    }
}
// 연결 상태 업데이트 함수 추가
void C250501MFCAppDlg::UpdateConnectionStatus()
{
    if (m_bConnected)
    {
        m_staticConnectionStatus.SetWindowText(_T("연결됨"));
        GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(_T("연결 해제"));
    }
    else
    {
        m_staticConnectionStatus.SetWindowText(_T("연결 안됨"));
        GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(_T("연결"));
    }
}
// 연결 버튼 클릭 이벤트 핸들러 추가
void C250501MFCAppDlg::OnBnClickedButtonConnect()
{
    UpdateData(TRUE);  // 컨트롤 값 가져오기

    // 입력값 검증
    if (m_strIP.IsEmpty())
    {
        MessageBox(_T("IP 주소를 입력하세요."), _T("오류"), MB_ICONWARNING);
        return;
    }

    if (m_nPort <= 0 || m_nPort > 65535)
    {
        MessageBox(_T("올바른 포트 번호를 입력하세요."), _T("오류"), MB_ICONWARNING);
        return;
    }

    if (!m_bConnected)
    {
        // PLC 연결
        if (ConnectToPlc())
        {
            m_bConnected = TRUE;
            UpdateConnectionStatus();
            AddLogMessage(_T("PLC에 연결되었습니다."));
        }
    }
    else
    {
        // PLC 연결 해제
        DisconnectFromPlc();
        m_bConnected = FALSE;
        UpdateConnectionStatus();
        AddLogMessage(_T("PLC 연결이 해제되었습니다."));
    }
}