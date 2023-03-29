#include "pch.h"
#include "GridSweepMethod.h"


complex <double> GridSweepMethod::get_A_k() {
	return complex< double >(0., -_step_t / (2 * _step_cordinate * _step_cordinate));
}

complex <double> GridSweepMethod::get_B_k() {
	return complex< double >(0., -_step_t / (2 * _step_cordinate * _step_cordinate));
}

complex <double> GridSweepMethod::get_C_k() {
	complex< double > first_term(1.0, 0.0);     // z = 1 + 0i
	complex< double > second_term(0.0, _step_t / (_step_cordinate * _step_cordinate));
	complex< double > sum = first_term + second_term;
	return sum;
}

complex <double> GridSweepMethod::derivative2_horizon_x(int index_x, int index_y) {
	return ((data_fi->at(index_y).at(index_x + 1) - data_fi->at(index_y).at(index_x)) -
		(data_fi->at(index_y).at(index_x) - data_fi->at(index_y).at(index_x - 1))) / (_step_cordinate * _step_cordinate);
}

complex <double> GridSweepMethod::derivative2_vertical_y(int index_x, int index_y) {
	return ((data_fi->at(index_y +1).at(index_x) - data_fi->at(index_y).at(index_x)) -
		(data_fi->at(index_y).at(index_x) - data_fi->at(index_y -1).at(index_x))) / (_step_cordinate * _step_cordinate);
}

complex <double> GridSweepMethod::get_D_k_horizon_x(int index_x, int index_y) {
	complex< double > first_term = data_fi->at(index_y).at(index_x);    // z = 1 + 0i
	complex< double > second_term(0, _step_t / 2);
	complex< double > third_term(2,0);

	return first_term + second_term * (derivative2_horizon_x(index_x, index_y) + third_term * derivative2_vertical_y(index_x, index_y));
}
complex <double> GridSweepMethod::get_D_k_vertical_y(int index_x, int index_y) {
	complex< double > first_term = data_fi->at(index_y).at(index_x);    // z = 1 + 0i
	complex< double > second_term(0, _step_t / 2);
	complex< double > third_term(2, 0);

	return first_term + second_term * (third_term* derivative2_horizon_x(index_x, index_y) + derivative2_vertical_y(index_x, index_y));
}

void GridSweepMethod::creat(
	vector < vector <complex <double> >>* fi_start,
	double input_step, double input_time,
	double ny_0, double v_0,
	double ny_n, double v_n)
{
	_step_cordinate = input_step;
	_step_t = input_time;
	_ny_0 = ny_0;
	_v_0 = v_0;
	_ny_n = ny_n;
	_v_n = v_n;

	data_fi = fi_start;
	_number_of_counts = fi_start->size();

	_init = true;
}

GridSweepMethod::GridSweepMethod() {
	_init = false;
}

GridSweepMethod::GridSweepMethod(
	vector < vector <complex <double> >>* fi_start,
	double input_step, double input_time,
	double ny_0, double v_0,
	double ny_n, double v_n)
{
	_step_cordinate = input_step;
	_step_t = input_time;

	_ny_0 = ny_0;
	_v_0 = v_0;
	_ny_n = ny_n;
	_v_n = v_n;

	data_fi = fi_start;
	_number_of_counts = fi_start->size();

	_init = true;
}

void GridSweepMethod::next_step_horizon_x() {

	if (!_init) return;

	//Зануляем 0-ую и последнюю строки
	for (int i = 0; i < _number_of_counts; i++) {
		data_fi->at(0).at(i) = { 0,0 };
		data_fi->at(_number_of_counts-1).at(i) = { 0,0 };
	}

	for (int j = 1; j < _number_of_counts-1; j++) {
		//Считаем кооф A,B,C,K
		_A_k.clear();
		_B_k.clear();
		_C_k.clear();
		_D_k.clear();

		_A_k.resize(_number_of_counts);
		_B_k.resize(_number_of_counts);
		_C_k.resize(_number_of_counts);
		_D_k.resize(_number_of_counts);

		//Кооф. A, B, C const
		complex <double> a_const = get_A_k();
		complex <double> b_const = get_B_k();
		complex <double> c_const = get_C_k();

		//Зануляем 0 отсчет
		_A_k[0] = { 0,0 };
		_B_k[0] = { 0,0 };
		_C_k[0] = { 0,0 };
		_D_k[0] = { 0,0 };

		//Вычисляем кооф на интервале
		for (int i = 1; i < _number_of_counts - 1; i++) {
			_A_k[i] = a_const;
			_B_k[i] = b_const;
			_C_k[i] = c_const;
			_D_k[i] = get_D_k_horizon_x(i,j);
		}
		//Зануляем последний отсчет
		_A_k[_number_of_counts - 1] = { 0,0 };
		_B_k[_number_of_counts - 1] = { 0,0 };
		_C_k[_number_of_counts - 1] = { 0,0 };
		_D_k[_number_of_counts - 1] = { 0,0 };

		//СЧИАТЕМ КООФ. АЛЬФА И БЕТТА
		_alfa_k.clear();
		_beta_k.clear();

		_alfa_k.resize(_number_of_counts);
		_beta_k.resize(_number_of_counts);

		_alfa_k[0] = _ny_0;
		_beta_k[0] = _v_0;

		_alfa_k[1] = _ny_0;
		_beta_k[1] = _v_0;

		for (int i = 2; i < _number_of_counts - 1; i++) {
			_alfa_k[i] = -_B_k[i - 1] / (_C_k[i - 1] + _A_k[i - 1] * _alfa_k[i - 1]);
			_beta_k[i] =
				(_D_k[i - 1] - _A_k[i - 1] * _beta_k[i - 1]) /
				(_C_k[i - 1] + _A_k[i - 1] * _alfa_k[i - 1]);
		}

		//Обратная прогонка
		data_fi->at(j).at(_number_of_counts - 1) = (_ny_n * _beta_k[_beta_k.size() - 1] + _v_n) /
			(1. - _ny_n * _alfa_k[_alfa_k.size() - 1]);

		for (int i = _number_of_counts - 2; i >= 0; i--) {
			data_fi->at(j).at(i) =
				_alfa_k[i + 1] * data_fi->at(j).at(i+1) + _beta_k[i + 1];
		}
	}
	
}

void GridSweepMethod::next_step_vertical_y() {

	if (!_init) return;

	//Зануляем 0-ый и последний стоблец
	for (int j = 0; j < _number_of_counts; j++) {
		data_fi->at(j).at(0) = { 0,0 };
		data_fi->at(j).at(_number_of_counts - 1) = { 0,0 };
	}

	for (int i = 1;i < _number_of_counts-1; i++) {
		//Считаем кооф A,B,C,K
		_A_k.clear();
		_B_k.clear();
		_C_k.clear();
		_D_k.clear();

		_A_k.resize(_number_of_counts);
		_B_k.resize(_number_of_counts);
		_C_k.resize(_number_of_counts);
		_D_k.resize(_number_of_counts);

		//Кооф. A, B, C const
		complex <double> a_const = get_A_k();
		complex <double> b_const = get_B_k();
		complex <double> c_const = get_C_k();

		//Зануляем 0 отсчет
		_A_k[0] = { 0,0 };
		_B_k[0] = { 0,0 };
		_C_k[0] = { 0,0 };
		_D_k[0] = { 0,0 };

		//Вычисляем кооф на интервале
		for (int j = 1; j < _number_of_counts-1; j++) {
			_A_k[j] = a_const;
			_B_k[j] = b_const;
			_C_k[j] = c_const;
			_D_k[j] = get_D_k_vertical_y(i, j);
		}
		//Зануляем последний отсчет
		_A_k[_number_of_counts - 1] = { 0,0 };
		_B_k[_number_of_counts - 1] = { 0,0 };
		_C_k[_number_of_counts - 1] = { 0,0 };
		_D_k[_number_of_counts - 1] = { 0,0 };

		//СЧИАТЕМ КООФ. АЛЬФА И БЕТТА
		_alfa_k.clear();
		_beta_k.clear();

		_alfa_k.resize(_number_of_counts);
		_beta_k.resize(_number_of_counts);

		_alfa_k[0] = _ny_0;
		_beta_k[0] = _v_0;

		_alfa_k[1] = _ny_0;
		_beta_k[1] = _v_0;

		for (int j = 2; j < _number_of_counts - 1; j++) {
			_alfa_k[j] = -_B_k[j - 1] / (_C_k[j - 1] + _A_k[j - 1] * _alfa_k[j - 1]);
			_beta_k[j] =
				(_D_k[j - 1] - _A_k[j - 1] * _beta_k[j - 1]) /
				(_C_k[j - 1] + _A_k[j - 1] * _alfa_k[j - 1]);
		}

		//Обратная прогонка
		data_fi->at(_number_of_counts - 1).at(i) = (_ny_n * _beta_k[_beta_k.size() - 1] + _v_n) /
			(1. - _ny_n * _alfa_k[_alfa_k.size() - 1]);

		for (int j = _number_of_counts - 2; j >= 0; j--) {
			data_fi->at(j).at(i) =
				_alfa_k[j + 1] * data_fi->at(j+1).at(i) + _beta_k[j + 1];
		}
	}

}


void GridSweepMethod::next_step() {
	next_step_horizon_x();
	next_step_vertical_y();
}

vector < vector <complex <double> >>* GridSweepMethod::get_data() {
	return data_fi;
}