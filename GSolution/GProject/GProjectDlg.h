
// GProjectDlg.h: 헤더 파일
//

#pragma once
#include "afxwin.h"
#include <thread>
#include <atomic>
#include <random>


// CGProjectDlg 대화 상자
class CGProjectDlg : public CDialogEx
{
// 생성입니다.
public:
	CGProjectDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	CArray<CPoint, CPoint> m_circlePoints;
	int m_circleCount;
	int m_selectedCircle;
	bool m_isDragging;

	CStatic m_staticCircle1;
	CStatic m_staticCircle2;
	CStatic m_staticCircle3;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnResetButtonClick();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRandomMoveButtonClick();
	afx_msg void OnApplyRadiusButtonClick();
	afx_msg void OnApplyThicknessButtonClick();

	void UpdateCircleCoordinates();
	void RandomMoveCircles();

	std::atomic<bool> m_isRunning;
	std::thread m_randomMoveThread;

	DECLARE_MESSAGE_MAP()
public:
	double m_radius;
	double m_thickness;
};
