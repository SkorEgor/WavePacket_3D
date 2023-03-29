//1) Подключение файла диалога
#pragma once
#include "pch.h"
#include "framework.h"
#include "WavePacket_3D.h"
#include "WavePacket_3DDlg.h"
#include "afxdialogex.h"

#include <complex>
#include <vector>
#include "GridSweepMethod.h"

using namespace std;

//x->i y->j

complex <double> fi_t0(double x, double x0, complex <double> fi_0, double sigma) {
	return fi_0 * exp(-(x - x0) * (x - x0) / (4 * sigma * sigma));
}

double barrier_Ux(double x, double x0, double a, double sigma) {
	return a * exp(-(x - x0) * (x - x0) / (4 * sigma * sigma));
}

vector <double> complex_to_double(vector <complex<double>>& complex_vector) {
	vector <double> rez;
	for (int i = 0; i < complex_vector.size(); i++) {
		rez.push_back(abs(complex_vector[i]));
	}
	return rez;
}

double gaussian_distribution_3d(double x, double x0, double y, double y0, double a, double sigma) {
	return a * exp(
		-(
		(x - x0) * (x - x0) +
		(y - y0) * (y - y0)
		) / 
		(4 * sigma * sigma)
	);
}


void gaussian_distribution_3d_fill(
	double x_min, double x_max,
	double y_min, double y_max,
	double a, double sigma, int N,
	vector < vector <complex<double>> >* in_val_z)
{
	in_val_z->clear();
	in_val_z->resize(N);

	double x_step = (x_max - x_min) / N;
	double y_step = (y_max - y_min) / N;

	double x0 = (x_max - x_min) / 2 + x_min;
	double y0 = (y_max - y_min) / 2 + y_min;

	for (int j = 0; j < N; j++) {
		in_val_z->at(j).resize(N);
		for (int i = 0; i < N; i++) {
			in_val_z->at(j).at(i) = { gaussian_distribution_3d(
				x_min + x_step * i,
				x0,
				y_min + y_step * j,
				y0,
				a,
				sigma
			), 0};
		}
	}
}

//Определяет работать или нет потоку
bool status_th = false;

//4)Задаем действия в потоке
DWORD WINAPI animationThread(LPVOID t)
{
	//4.1)Создаем объект нашего окна
	CWavePacket3DDlg* thisWind = (CWavePacket3DDlg*)t;
	
	//Получаем кол-во отсчетов и ширину ямы
	int N = thisWind->input_number_of_counts;
	double d = thisWind->input_pit_width;
	double step_x = d / N;

	//Задаем барьер
	double a_barrier = thisWind->input_amplituda_barrier;
	double sigma_barrier = thisWind->input_sigma_barrier;

	//Задаем начальное распределение
	double fi_packet=thisWind->input_amplituda_distributions;
	double sigma_packet = thisWind->input_sigma_distributions;

	//Получаем шаг по времени
	double time_step = thisWind->input_time_step;

	//Зададим начальную вероятность
	gaussian_distribution_3d_fill(
		0, d,
		0, d,
		fi_packet, sigma_packet, N,
		&(thisWind->val_z));


	thisWind->pictureControl.updateData(thisWind->val_z);
	


	thisWind->experiment.creat(
		&thisWind->val_z,
		step_x, time_step,
		0, 0, 0, 0
	);


	
	//Задержка между кадрами
	int delTime = thisWind->input_time_stop;
	//Кол-во строк фурье
	int animation_step = thisWind ->input_animation_step;
	thisWind->signalDrawer.SetDataCoordinate(N, N);
	thisWind->signalDrawer.Draw(&thisWind->val_z,5);
	
	//Запуск потока
	while (status_th) {
		//---------Работа в потоке---------//
		//4.2)Прописываем необходимые команды в потоке
		for (int i = 0; i < animation_step; i++) {
			thisWind->experiment.next_step();
		}
		thisWind->pictureControl.updateData(thisWind->val_z);
		thisWind->signalDrawer.Draw(&thisWind->val_z, 5);
		//------Конец работы в потоке------//
		Sleep(delTime);		//4.3) Указываем задержку между кадрами
	}
	/*
	if (status_th) {
		thisWind->selected_coordinates_index = 1;
		thisWind->selected_frequency_index = 1;

		thisWind->experiment.conversionToFrequencies();
		vector <double> xf(thisWind->experiment.getCountFrequency());
		for (int i = 0; i < xf.size(); i++) xf[i] = i;
		thisWind->fourierDrawing.Draw(complex_to_double(thisWind->experiment.getFrequency(thisWind->selected_coordinates_index)), xf, L"F", L"X");
		thisWind->fourierDrawing.index(thisWind->selected_frequency_index);

		//Отображаем максимальынй индекс
		CString str;
		str.Format(_T("%d"), xf.size());
		thisWind->output_index_max_frequencies.SetWindowTextW(str);

		//Отображаем индекс
		str.Format(_T("%d"), thisWind->selected_frequency_index);
		thisWind->output_index_frequency.SetWindowTextW(str);

		//Отображаем максимальынй индекс
		str.Format(_T("%d"), thisWind->experiment.getSizeFrequency());
		thisWind->output_index_coordinates_max.SetWindowTextW(str);

		//Отображаем индекс
		str.Format(_T("%d"), thisWind->selected_coordinates_index);
		thisWind->output_index_coordinates.SetWindowTextW(str);

		thisWind->updateSlider();

		status_th = false;
	}
	*/
	//thisWind->AAAA.SetWindowTextW(L"stop");
	return 0;
}


//2) Функция создания потока и указываем нужно простарнство
void CWavePacket3DDlg::startAnimationFlow()
{
	if (status_th) {
		finishAnimationFlow();
		/*
		if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())choiceParameterResearched('l');
		else if (((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck())choiceParameterResearched('k');
		else if (((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck())choiceParameterResearched('c');
		else if (((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck())choiceParameterResearched('v');
		else if (((CButton*)GetDlgItem(IDC_RADIO5))->GetCheck())choiceParameterResearched('f');
		*/
		return;
	}

	//запуск потока
	status_th = true;
	LPVOID lpParameter = (LPVOID)this;
	HANDLE thread = CreateThread(
		NULL,				//LPSECURITY_ATTRIBUTES lpThreadAttributes, // дескриптор защиты
		0,					//SIZE_T dwStackSize,                       // начальный размер стека
		animationThread,	//LPTHREAD_START_ROUTINE lpStartAddress,    // функция потока
		lpParameter,		//LPVOID lpParameter,                       // параметр потока
		0,					//DWORD dwCreationFlags,                    // опции создания
		NULL				//LPDWORD lpThreadId						// идентификатор потока
	);
	//UpdateData(FALSE);
}

//3) Создаем функцию endAnimationFlow и указываем нужно простарнство
void CWavePacket3DDlg::finishAnimationFlow()
{
	status_th = false;
	//UpdateData(FALSE);
}
