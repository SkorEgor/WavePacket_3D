
// WavePacket_3D.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CWavePacket3DApp:
// Сведения о реализации этого класса: WavePacket_3D.cpp
//

class CWavePacket3DApp : public CWinApp
{
public:
	CWavePacket3DApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CWavePacket3DApp theApp;
