
// WavePacket_3DDlg.h: файл заголовка
//

#pragma once
#include "painter.h"
#include <complex>
#include "GridSweepMethod.h"
#include "Drawer.h"

// Диалоговое окно CWavePacket3DDlg
class CWavePacket3DDlg : public CDialogEx
{
// Создание
public:
	CWavePacket3DDlg(CWnd* pParent = nullptr);	// стандартный конструктор
	painter pictureControl;
	Drawer signalDrawer;
	GridSweepMethod experiment;
	vector < vector <complex<double>> > val_z;
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAVEPACKET_3D_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void startAnimationFlow();
	void finishAnimationFlow();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton7();
	int input_number_of_counts;
	double input_pit_width;
	double input_time_step;
	double input_amplituda_barrier;
	double input_sigma_barrier;
	double input_amplituda_distributions;
	double input_sigma_distributions;
	int input_animation_step;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnEnChangeEdit8();
	int input_time_stop;
};
