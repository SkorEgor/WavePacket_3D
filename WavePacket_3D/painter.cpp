// painter.cpp: файл реализации
//
#include "pch.h"
#include "painter.h"


ULONG_PTR token;
// painter



IMPLEMENT_DYNAMIC(painter, CStatic)

painter::painter()
{

	GdiplusStartupInput input;
	Status a;
	a = GdiplusStartup(&token, &input, NULL);
	if (a != Ok) MessageBox(L"Ошибка GDI", L"Error");

	_angelX = 0;
	_angelY = -10;
	_angelZ = 20;
	_k = 0.5;
	_brash = true;
	_persp = true;
}

painter::~painter()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(painter, CStatic)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void painter::OnKeyDown(double in_angelY, double in_angelZ, bool in_persp, bool in_brash)
{
	_angelY += in_angelY;
	_angelZ += in_angelZ;
	if (in_persp) _persp = !_persp;
	if (in_brash) _brash = !_brash;
	Invalidate(FALSE);
}



min_max search_min_and_max(vector < vector <complex<double>> >*mass) {
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

double search_scaling_coefficients(min_max &input_border, double max_z, double min_z) {
	double renge_z = max_z - min_z;
	double renge_border = input_border.max - input_border.min;
	double k = renge_z / renge_border;
	return k;
}

void painter::updateData(vector < vector <complex<double>> >& in_val) {
	_val_z = &in_val;
	Invalidate(FALSE);
}

void painter::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (_val_z==NULL) return;

	Graphics gr(lpDrawItemStruct->hDC);
	gr.Clear(Color::AliceBlue);

	//Получаем размеры окна
	int width = lpDrawItemStruct->rcItem.right;
	int height = lpDrawItemStruct->rcItem.bottom;

	//создаем буфер
	Gdiplus::Bitmap backBuffer(width, height, &gr);
	Graphics temp(&backBuffer);


	Matrix matr;
	double xScale = width / 4.;
	double yScale = height / 2.;
	matr.Scale(xScale, -yScale);

	matr.Translate(2, -1);
	matr.Scale(_k, _k);
	temp.SetTransform(&matr);

	//Размер вектора
	int N_size = _val_z->size();
	
	min_max border = search_min_and_max(_val_z);
	double scaling_coefficients = search_scaling_coefficients(border, 1., -1.);

	/*X - в значения функции*/
	double maxX = 2; double minX = -2;
	double rengeX = maxX - minX;
	double StepX = rengeX / N_size;

	/*Y - в значения функции*/
	double maxY = 2; double minY = -2;
	double rengeY = maxY - minY;
	double StepY = rengeY / N_size;



	mat4 matrix;
	if (_persp) matrix.perspective(20);
	matrix.rotateY(_angelY);
	matrix.rotateZ(_angelZ);


	temp.Clear(Color::Black);

	for (int x_index = 0; x_index < N_size-1; x_index++) {
		for (double y_index = 0; y_index < N_size-1; y_index++) {
			double x_i = minX + x_index * StepX;
			double y_i = minY + y_index * StepY;

			vec4 vec1(x_i, y_i,
				abs(_val_z->at(x_index)
				.at(y_index))
				* scaling_coefficients - 1);

			vec4 vec2(x_i + StepX, y_i,
				abs(_val_z->at(x_index + 1)
				.at(y_index))
				* scaling_coefficients - 1);

			vec4 vec3(x_i + StepX, y_i + StepY,
				abs(_val_z->at(x_index + 1)
				.at(y_index + 1))
				* scaling_coefficients - 1);

			vec4 vec4(x_i, y_i + StepY,
				abs(_val_z->at(x_index)
				.at(y_index + 1))
				* scaling_coefficients - 1);

			vec1 = vec1 * matrix;
			vec2 = vec2 * matrix;
			vec3 = vec3 * matrix;
			vec4 = vec4 * matrix;

			PointF pointArray[] = {
				PointF(vec1.y(), vec1.z()),
				PointF(vec2.y(), vec2.z()),
				PointF(vec3.y(), vec3.z()),
				PointF(vec4.y(), vec4.z())
			};

			Gdiplus::SolidBrush subgrid_brash(Color::Blue);
			Gdiplus::Pen subgrid_pen(Color::White, 0.0001);
			if (_brash)temp.FillPolygon(&subgrid_brash, pointArray, 4);
			temp.DrawPolygon(&subgrid_pen, pointArray, 4);
		}
	}
	gr.DrawImage(&backBuffer, 0, 0, 0, 0, width, height, UnitPixel);

	
}


