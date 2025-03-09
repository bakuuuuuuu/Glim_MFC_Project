// GProjectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "GProject.h"
#include "GProjectDlg.h"
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


// CGProjectDlg 대화 상자



CGProjectDlg::CGProjectDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_GPROJECT_DIALOG, pParent), m_selectedCircle(-1), m_isDragging(false), m_isRunning(false)
    , m_radius(0)
    , m_thickness(0)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGProjectDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_CIRCLE1, m_staticCircle1);
    DDX_Control(pDX, IDC_STATIC_CIRCLE2, m_staticCircle2);
    DDX_Control(pDX, IDC_STATIC_CIRCLE3, m_staticCircle3);
    DDX_Text(pDX, IDC_EDIT_RADIUS, m_radius);
    DDX_Text(pDX, IDC_EDIT_THICKNESS, m_thickness);
}

BEGIN_MESSAGE_MAP(CGProjectDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_BN_CLICKED(IDC_BTN_RESET, &CGProjectDlg::OnResetButtonClick)
    ON_BN_CLICKED(IDC_BTN_RANDOM, &CGProjectDlg::OnRandomMoveButtonClick)
    ON_BN_CLICKED(IDC_BTN_APPLY_RADIUS, &CGProjectDlg::OnApplyRadiusButtonClick)
    ON_BN_CLICKED(IDC_BTN_APPLY_THICKNESS, &CGProjectDlg::OnApplyThicknessButtonClick)
END_MESSAGE_MAP()


// CGProjectDlg 메시지 처리기

BOOL CGProjectDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

    // IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
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

    // 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
    //  프레임워크가 이 작업을 자동으로 수행합니다.
    SetIcon(m_hIcon, TRUE);         // 큰 아이콘을 설정합니다.
    SetIcon(m_hIcon, FALSE);      // 작은 아이콘을 설정합니다.

    // TODO: 여기에 추가 초기화 작업을 추가합니다.
    UpdateData(FALSE);
    m_radius = 10;

    m_staticCircle1.SetWindowTextW(L"Circle 1: (N/A, N/A)");
    m_staticCircle2.SetWindowTextW(L"Circle 2: (N/A, N/A)");
    m_staticCircle3.SetWindowTextW(L"Circle 3: (N/A, N/A)");

    m_circleCount = 0;

    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CGProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CGProjectDlg::OnPaint()
{
    if (IsIconic())
    {
        CDialogEx::OnPaint();
    }
    else
    {
        CDialogEx::OnPaint();

        CClientDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

        for (int i = 0; i < m_circleCount; i++)
        {
            CPoint point = m_circlePoints[i];
            CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
            CBrush brush(RGB(0, 0, 0));
            CPen* oldPen = dc.SelectObject(&pen);
            CBrush* oldBrush = dc.SelectObject(&brush);

            dc.Ellipse(point.x - m_radius, point.y - m_radius, point.x + m_radius, point.y + m_radius);
            dc.SelectObject(oldPen);
            dc.SelectObject(oldBrush);
        }

        if (m_circleCount == 3)
        {
            CPoint p1 = m_circlePoints[0];
            CPoint p2 = m_circlePoints[1];
            CPoint p3 = m_circlePoints[2];

            double D = 2 * (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y));
            double Dx = (p1.x * p1.x + p1.y * p1.y) * (p2.y - p3.y) + (p2.x * p2.x + p2.y * p2.y) * (p3.y - p1.y) + (p3.x * p3.x + p3.y * p3.y) * (p1.y - p2.y);
            double Dy = (p1.x * p1.x + p1.y * p1.y) * (p3.x - p2.x) + (p2.x * p2.x + p2.y * p2.y) * (p1.x - p3.x) + (p3.x * p3.x + p3.y * p3.y) * (p2.x - p1.x);
            double cx = Dx / D;
            double cy = Dy / D;
            double r = sqrt((p1.x - cx) * (p1.x - cx) + (p1.y - cy) * (p1.y - cy));

            CPen pen(PS_SOLID, (int)m_thickness, RGB(0, 0, 0));
            CPen* oldPen = dc.SelectObject(&pen);
            dc.SelectStockObject(NULL_BRUSH);
            dc.Ellipse((int)(cx - r), (int)(cy - r), (int)(cx + r), (int)(cy + r));
            dc.SelectObject(oldPen);
        }
    }
}

void CGProjectDlg::OnResetButtonClick()
{
    m_circleCount = 0;
    m_circlePoints.RemoveAll();
    m_radius = 10;

    GetDlgItem(IDC_EDIT_RADIUS)->SetWindowTextW(L"0");

    UpdateCircleCoordinates();
    Invalidate();
}

void CGProjectDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CDialogEx::OnLButtonDown(nFlags, point);

    m_selectedCircle = -1;

    for (int i = 0; i < m_circleCount; i++)
    {
        CPoint p = m_circlePoints[i];
        int dx = point.x - p.x;
        int dy = point.y - p.y;
        if (dx * dx + dy * dy <= m_radius * m_radius)
        {
            m_selectedCircle = i;
            m_isDragging = true;
            return;
        }
    }

    if (m_circleCount < 3)
    {
        m_circlePoints.Add(point);
        m_circleCount++;
        UpdateCircleCoordinates();
        Invalidate();
    }
}

void CGProjectDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_isDragging && m_selectedCircle != -1)
    {
        m_circlePoints[m_selectedCircle] = point;
        UpdateCircleCoordinates();
        Invalidate();
    }

    CDialogEx::OnMouseMove(nFlags, point);
}

void CGProjectDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_isDragging = false;
    m_selectedCircle = -1;

    CDialogEx::OnLButtonUp(nFlags, point);
}


void CGProjectDlg::UpdateCircleCoordinates()
{
    CString text;
    if (m_circleCount > 0)
    {
        text.Format(L"Circle 1: (%d, %d)", m_circlePoints[0].x, m_circlePoints[0].y);
        m_staticCircle1.SetWindowTextW(text);
    }
    else
    {
        m_staticCircle1.SetWindowTextW(L"Circle 1: (N/A, N/A)");
    }

    if (m_circleCount > 1)
    {
        text.Format(L"Circle 2: (%d, %d)", m_circlePoints[1].x, m_circlePoints[1].y);
        m_staticCircle2.SetWindowTextW(text);
    }
    else
    {
        m_staticCircle2.SetWindowTextW(L"Circle 2: (N/A, N/A)");
    }

    if (m_circleCount > 2)
    {
        text.Format(L"Circle 3: (%d, %d)", m_circlePoints[2].x, m_circlePoints[2].y);
        m_staticCircle3.SetWindowTextW(text);
    }
    else
    {
        m_staticCircle3.SetWindowTextW(L"Circle 3: (N/A, N/A)");
    }
}


void CGProjectDlg::OnRandomMoveButtonClick()
{
    if (m_circleCount < 3) return;
    if (m_isRunning.load()) return;

    m_isRunning.store(true);
    m_randomMoveThread = std::thread(&CGProjectDlg::RandomMoveCircles, this);
    m_randomMoveThread.detach();
}

void CGProjectDlg::RandomMoveCircles()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    CRect clientRect;
    GetClientRect(&clientRect);

    std::uniform_int_distribution<int> distX(20, clientRect.Width() - 20);
    std::uniform_int_distribution<int> distY(20, clientRect.Height() - 20);

    for (int i = 0; i < 10; ++i)
    {
        if (!m_isRunning.load()) return;

        for (int j = 0; j < 3; ++j)
        {
            m_circlePoints[j] = CPoint(distX(gen), distY(gen));
        }

        UpdateCircleCoordinates();
        Invalidate();
        Sleep(500);
    }
    m_isRunning.store(false);
}

void CGProjectDlg::OnApplyRadiusButtonClick()
{
    UpdateData(TRUE);
    if (m_radius < 1)
    {
        AfxMessageBox(_T("반지름은 1 이상이어야 합니다."));
        return;
    }
    UpdateCircleCoordinates();
    Invalidate();
}

void CGProjectDlg::OnApplyThicknessButtonClick()
{
    UpdateData(TRUE);
    if (m_thickness < 1.0)
    {
        AfxMessageBox(L"선 두께는 1 이상이어야 합니다.");
        return;
    }
    Invalidate();
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CGProjectDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}
