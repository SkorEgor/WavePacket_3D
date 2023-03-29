
// WavePacket_3DDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "WavePacket_3D.h"
#include "WavePacket_3DDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CWavePacket3DDlg



CWavePacket3DDlg::CWavePacket3DDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WAVEPACKET_3D_DIALOG, pParent)
	, input_number_of_counts(32)
	, input_pit_width(20)
	, input_time_step(0.01)
	, input_amplituda_barrier(2.0)
	, input_sigma_barrier(0.1)
	, input_amplituda_distributions(5)
	, input_sigma_distributions(2)
	, input_animation_step(20)
	, input_time_stop(200)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWavePacket3DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_3D, pictureControl);
	DDX_Text(pDX, IDC_EDIT1, input_number_of_counts);
	DDX_Text(pDX, IDC_EDIT2, input_pit_width);
	DDX_Text(pDX, IDC_EDIT3, input_time_step);
	DDX_Text(pDX, IDC_EDIT4, input_amplituda_barrier);
	DDX_Text(pDX, IDC_EDIT5, input_sigma_barrier);
	DDX_Text(pDX, IDC_EDIT6, input_amplituda_distributions);
	DDX_Text(pDX, IDC_EDIT7, input_sigma_distributions);
	DDX_Text(pDX, IDC_EDIT8, input_animation_step);
	DDX_Text(pDX, IDC_EDIT9, input_time_stop);
}

BEGIN_MESSAGE_MAP(CWavePacket3DDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_WM_KEYDOWN()
ON_BN_CLICKED(IDC_BUTTON2, &CWavePacket3DDlg::OnBnClickedButton2)
ON_BN_CLICKED(IDC_BUTTON5, &CWavePacket3DDlg::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON4, &CWavePacket3DDlg::OnBnClickedButton4)
ON_BN_CLICKED(IDC_BUTTON3, &CWavePacket3DDlg::OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON6, &CWavePacket3DDlg::OnBnClickedButton6)
ON_BN_CLICKED(IDC_BUTTON1, &CWavePacket3DDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON7, &CWavePacket3DDlg::OnBnClickedButton7)
ON_EN_CHANGE(IDC_EDIT1, &CWavePacket3DDlg::OnEnChangeEdit1)
ON_EN_CHANGE(IDC_EDIT2, &CWavePacket3DDlg::OnEnChangeEdit2)
ON_EN_CHANGE(IDC_EDIT3, &CWavePacket3DDlg::OnEnChangeEdit3)
ON_EN_CHANGE(IDC_EDIT4, &CWavePacket3DDlg::OnEnChangeEdit4)
ON_EN_CHANGE(IDC_EDIT5, &CWavePacket3DDlg::OnEnChangeEdit5)
ON_EN_CHANGE(IDC_EDIT6, &CWavePacket3DDlg::OnEnChangeEdit6)
ON_EN_CHANGE(IDC_EDIT7, &CWavePacket3DDlg::OnEnChangeEdit7)
ON_EN_CHANGE(IDC_EDIT8, &CWavePacket3DDlg::OnEnChangeEdit8)
END_MESSAGE_MAP()


// Обработчики сообщений CWavePacket3DDlg

BOOL CWavePacket3DDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	signalDrawer.Create(GetDlgItem(IDC_STATIC2)->GetSafeHwnd());
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CWavePacket3DDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CWavePacket3DDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CWavePacket3DDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	double angelY = 0, angelZ = 0;
//	bool persp = false;
//	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
//	if (nChar == VK_UP) angelY += 10;
//	else if (nChar == VK_DOWN) angelY -= 10;
//	else if (nChar == VK_RIGHT)angelZ += 10;
//	else if (nChar == VK_LEFT) angelZ -= 10;
//	else if (nChar == VK_SHIFT) persp = !persp;
//	pictureControl.OnKeyDown(angelY, angelZ, persp);
//
//	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
//}

//лево
void CWavePacket3DDlg::OnBnClickedButton2() { pictureControl.OnKeyDown(0, -10, false, false); }
//право
void CWavePacket3DDlg::OnBnClickedButton4() { pictureControl.OnKeyDown(0, 10, false, false); }
//Верх
void CWavePacket3DDlg::OnBnClickedButton5() { pictureControl.OnKeyDown(10, 0, false, false); }
//низ
void CWavePacket3DDlg::OnBnClickedButton3() { pictureControl.OnKeyDown(-10, 0, false, false); }
//Перспектива
void CWavePacket3DDlg::OnBnClickedButton6() { pictureControl.OnKeyDown(0, 0, true, false); }
//Заливка
void CWavePacket3DDlg::OnBnClickedButton1() { pictureControl.OnKeyDown(0, 0, false, true); }


void CWavePacket3DDlg::OnBnClickedButton7()
{
	/*
	int N_size = 20;
	val_z.resize(N_size);
	for (int j = 0; j < N_size; j++) {
		val_z[j].resize(N_size);
		for (int i = 0; i < N_size; i++) {
			val_z[j][i] = j * (1000000. / N_size) - 1.;
		}
	}
	pictureControl.updateData(val_z);
	*/
	startAnimationFlow();
}


void CWavePacket3DDlg::OnEnChangeEdit1() { UpdateData(TRUE); }
void CWavePacket3DDlg::OnEnChangeEdit2() { UpdateData(TRUE); }
void CWavePacket3DDlg::OnEnChangeEdit3() { UpdateData(TRUE); }
void CWavePacket3DDlg::OnEnChangeEdit4() { UpdateData(TRUE); }
void CWavePacket3DDlg::OnEnChangeEdit5() { UpdateData(TRUE); }
void CWavePacket3DDlg::OnEnChangeEdit6() { UpdateData(TRUE); }
void CWavePacket3DDlg::OnEnChangeEdit7() { UpdateData(TRUE); }
void CWavePacket3DDlg::OnEnChangeEdit8() { UpdateData(TRUE); }
