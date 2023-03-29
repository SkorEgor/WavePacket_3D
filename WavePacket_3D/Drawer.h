#pragma once

#include <afxwin.h>
#include <vector>
#define pi 3.1415926535897932384626433832795

using namespace std;

struct MyRGB {
	int R, G, B;
};



class Drawer
{
	struct min_max {
		double min, max;
	};
	// Прямоугольник области рисования.
	CRect frame;
	// Указатель на объект окна, области рисования.
	CWnd* wnd;
	// Контекст рисования, привязанный к области рисования.
	CDC* dc;
	// Контекст рисования, привязанный к памяти.
	CDC memDC;
	// Контекст рисования, для сохранения осей.
	CDC memDC_axis;
	// Память для контекста рисования memDC.
	CBitmap bmp;
	// Флаг для отслеживания состояния инициализации класса.
	bool init, init_data, init_axis;

	int _data_count_x, _data_count_y;

	double _screen_x_max, _screen_x_min, _screen_width;
	double _screen_y_max, _screen_y_min, _screen_height;

	//отступ от края
	double _indent_x, _indent_y;

	double _screen_pixel_y, _screen_pixel_x;

	//Название осей
	CString nameX, nameY;
public:
	// Проинициализировать объект класса на основе HWND.
	void Create(HWND hWnd)
	{
		// Получаем указатель на окно.
		wnd = CWnd::FromHandle(hWnd);
		// Получаем прямоугольник окна.
		wnd->GetClientRect(frame);
		// Получаем контекст для рисования в этом окне.
		dc = wnd->GetDC();
		// Создаем буфер-контекст.
		memDC.CreateCompatibleDC(dc);
		memDC_axis.CreateCompatibleDC(dc);
		// Создаем растр для контекста рисования.
		bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
		// Выбираем растр для использования буфер-контекстом.
		memDC.SelectObject(&bmp);
		memDC_axis.SelectObject(&bmp);
		init = true;
		init_axis = false;
	}

	void SetDataCoordinate(int data_count_x, int data_count_y) {

		_data_count_x = data_count_x;
		_data_count_y = data_count_y;

		_indent_x = 0;// frame.Width() * 0.05;

		_screen_x_max = frame.Width() - _indent_x;
		_screen_x_min = _indent_x;
		_screen_width = _screen_x_max - _screen_x_min;

		_indent_y = 0;// frame.Height() * 0.05;

		_screen_y_max = frame.Height() - _indent_y;
		_screen_y_min = _indent_y;
		_screen_height = _screen_y_max - _screen_y_min;

		_screen_pixel_x = _screen_width / _data_count_x;
		_screen_pixel_y = _screen_height / _data_count_y;

		init_data = true;
		init_axis = false;
	}

	double search_scaling_coefficients(min_max& input_border, double max_z, double min_z) {
		double renge_z = max_z - min_z;
		double renge_border = input_border.max - input_border.min;
		double k = renge_z / renge_border;
		return k;
	}

	double coordinate_between(double &val1, double &val2, double coord1, double coord2, double &val_z) {
		return coord2 - ((val2 - val_z) / (val2 - val1)) * (coord2 - coord1);
	}

	// Нарисовать график по переданным данным.
	void Draw(vector < vector <complex <double> >>*matrix, int count_step_porog=NULL)
	{
		if (!init && !init_data) return;

		CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
		CPen Xgrid_pen(PS_SOLID, 2, RGB(128, 0, 0));
		CPen Ygrid_pen(PS_SOLID, 2, RGB(0, 128, 0));
		CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen data_pen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen data_pen2(PS_SOLID, 2, RGB(38, 0, 255));
		CPen data_blue(PS_SOLID, 2, RGB(106, 90, 205));
		CPen pen_red(PS_SOLID, 2, RGB(178, 34, 34));
		CPen pen_green(PS_SOLID, 1, RGB(0, 128, 0));
		CFont font;
		font.CreateFontW(18, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// Курсив
			FALSE,				// Подчеркнутый
			FALSE,				// Перечеркнутый
			DEFAULT_CHARSET,	// Набор символов
			OUT_OUTLINE_PRECIS,	// Точность соответствия.	
			CLIP_DEFAULT_PRECIS,//  
			CLEARTYPE_QUALITY,	// Качество
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
		);
		min_max min_max_data = search_min_and_max(matrix);
		if (!init_axis) {
			// Белый фон.
			memDC.FillSolidRect(frame, RGB(255, 255, 255));

			//Отрисовка поля
			double x0, x1, y0, y1;
			
			int max_line = matrix->size();
			int max_column = matrix->at(0).size();

			

			double koof = search_scaling_coefficients(min_max_data, 255, -255);

			for (int line = 0; line < max_line; line++) {
				for (int column = 0; column < max_column; column++) {
					int R = 0, B = 0, G = 0;
					
					if (abs(matrix->at(line).at(column))* koof -255> 0) {
						R = abs(matrix->at(line).at(column)) * koof - 255;
						G = 0;
					}
					else  {
						B = -abs(matrix->at(line).at(column)) * koof - 255;
						G = 0;
					}
						

					
					//memDC.SelectObject(&AAAA);
					

					//pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); //с зеленой рамкой
					HPEN pen = (HPEN)GetStockObject(NULL_PEN); //без рамки
					CBrush AAAA(RGB(R, G, B));
					//HBRUSH brush = CreateSolidBrush(RGB(120, 120, 120));
					//HBRUSH oldbrush = (HBRUSH)SelectObject(memDC, brush);
					//HPEN oldpen = (HPEN)SelectObject(memDC, pen);

					memDC.SelectObject(AAAA);
					memDC.SelectObject(pen);


					x0 = (column-1) * _screen_pixel_x;
					y0 = (line-1)* _screen_pixel_y;
					x1 = column * _screen_pixel_x + _screen_pixel_x;
					y1 = line * _screen_pixel_y + _screen_pixel_y;

					memDC.Rectangle(
							x0
							, y0
							, x1
							, y1
						);

				}
			}

			//Сохранение в буфер
			memDC_axis.BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);


		}
		else {
			memDC.BitBlt(0, 0, frame.Width(), frame.Height(), &memDC_axis, 0, 0, SRCCOPY);
		}
		if (count_step_porog != NULL ) {
			double porog = min_max_data.min;
			double max_porog = min_max_data.max;
			double step_val = (max_porog - porog) / count_step_porog;

			int max_line = matrix->size();
			int max_column = matrix->at(0).size();

			vector<double> point_x_triangle1, point_y_triangle1;
			vector<double> point_x_triangle2, point_y_triangle2;

			for (int line = 0; line < max_line-1; line++) {
				for (int column = 0; column < max_column-1; column++) {

					//Левый нижний треугольник
					double lift_up = abs(matrix->at(line).at(column)),
						left_down = abs(matrix->at(line + 1).at(column)),
						right_down = abs(matrix->at(line + 1).at(column + 1)),
						right_up = abs(matrix->at(line).at(column + 1));

					//Находим точки пересечения
					for (double porog_i = porog; porog_i < max_porog; porog_i = porog_i + step_val) {
						point_x_triangle1.clear();
						point_y_triangle1.clear();

						point_x_triangle2.clear();
						point_y_triangle2.clear();
						//Левая сторона
						if ((lift_up > porog_i && porog_i > left_down) || (lift_up < porog_i && porog_i < left_down)) {
							point_x_triangle1.push_back(column);

							point_y_triangle1.push_back(coordinate_between(
								lift_up, left_down,
								line, line + 1,
								porog_i
							));

							//double k = (lift_up - porog_i) / (lift_up - left_down);
							//point_y_triangle1.push_back(line - k* (-1));
							//point_y_triangle1.push_back((2. * line + 1.) / 2.);
						}
						//Нижняя сторона
						if ((left_down > porog_i && porog_i > right_down) || (left_down < porog_i && porog_i < right_down)) {
							//double k = (left_down - porog_i) / (left_down - right_down);
							//point_y_triangle1.push_back(column - k * (column + 1));
							// 
							//point_x_triangle1.push_back((2. * column + 1.) / 2.);
							//

							point_x_triangle1.push_back(coordinate_between(
								left_down, right_down,
								column, column + 1,
								porog_i
							));

							point_y_triangle1.push_back(line + 1);
						}
						//Гипотенуза
						if ((lift_up > porog_i && porog_i > right_down) || (lift_up < porog_i && porog_i < right_down)) {

							point_x_triangle1.push_back(coordinate_between(
								lift_up, right_down,
								column, column + 1,
								porog_i
							));
							//point_y_triangle1.push_back((2. * column + 1.) / 2.);
							point_y_triangle1.push_back(coordinate_between(
								lift_up, right_down,
								line, line + 1,
								porog_i
							));
							//point_y_triangle1.push_back((2. * line + 1.) / 2.);


							point_x_triangle2.push_back(coordinate_between(
								lift_up, right_down,
								column, column + 1,
								porog_i
							));
							//point_x_triangle2.push_back((2. * column + 1.) / 2.);

							point_y_triangle2.push_back(coordinate_between(
								lift_up, right_down,
								line, line + 1,
								porog_i
							));
							//point_y_triangle2.push_back((2. * line + 1.) / 2.);
						}
						//Правая сторона
						if ((right_up > porog_i && porog_i > right_down) || (right_up < porog_i && porog_i < right_down)) {
							point_x_triangle2.push_back(column + 1);
							
							point_y_triangle2.push_back(coordinate_between(
								right_up, right_down,
								line, line+1,
								porog_i
							));
							//point_y_triangle2.push_back((2. * line + 1.) / 2.);
						}
						//Верхняя сторона
						if ((right_up > porog_i && porog_i > lift_up) || (right_up < porog_i && porog_i < lift_up)) {
							//point_x_triangle2.push_back((2. * column + 1.) / 2.);
							point_x_triangle2.push_back(coordinate_between(
								right_up, lift_up,
								column+1, column,
								porog_i
							));

							point_y_triangle2.push_back(line);
						}



						if (point_x_triangle1.size() >= 2) {
							point_x_triangle1 = convert_range(point_x_triangle1, _screen_x_max, _screen_x_min, max_column, 0);
							point_y_triangle1 = convert_range(point_y_triangle1, _screen_y_max, _screen_y_min, max_line, 0);

							memDC.SelectObject(&pen_green);
							memDC.MoveTo(point_x_triangle1[0], point_y_triangle1[0]);
							for (unsigned int i = 0; i < point_y_triangle1.size(); i++)
							{
								memDC.LineTo(point_x_triangle1[i], point_y_triangle1[i]);
							}
						}

						if (point_x_triangle2.size() >= 2) {
							point_x_triangle2 = convert_range(point_x_triangle2, _screen_x_max, _screen_x_min, max_column, 0);
							point_y_triangle2 = convert_range(point_y_triangle2, _screen_y_max, _screen_y_min, max_line, 0);

							memDC.SelectObject(&pen_green);
							memDC.MoveTo(point_x_triangle2[0], point_y_triangle2[0]);
							for (unsigned int i = 0; i < point_y_triangle2.size(); i++)
							{
								memDC.LineTo(point_x_triangle2[i], point_y_triangle2[i]);
							}
						}
					}
					
				}

			}
		}



		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}


	void Clean() {
		memDC.FillSolidRect(frame, RGB(255, 255, 255));
		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}
	min_max search_min_and_max(vector < vector <complex<double>> >* mass) {
		min_max a;
		a.max = NULL;
		a.min = NULL;
		if (!mass->size()) return a;

		a.max = abs(mass->at(0).at(0));
		a.min = abs(mass->at(0).at(0));

		for (int j = 0; j < mass->size(); j++) {
			for (int i = 0; i < mass->at(j).size(); i++) {
				if (a.max < abs(mass->at(j).at(i))) a.max = abs(mass->at(j).at(i));
				if (a.min > abs(mass->at(j).at(i))) a.min = abs(mass->at(j).at(i));
			}
		}

		return a;
	}



	vector<double> convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin)
	{
		vector<double> output = data;
		double k = (outmax - outmin) / (inmax - inmin);
		for (auto& item : output)
		{
			item = (item - inmin) * k + outmin;
		}

		return output;
	}
};