#pragma once
#include <complex>
#include <vector>

using namespace std;


class GridSweepMethod
{
private:
	double _step_cordinate;	//+
	double _step_t;	//+
	double _sigma;
	int _number_of_counts;
	vector < vector <complex <double> >> *data_fi;	//+

	vector < complex <double>> _A_k;
	vector < complex <double>> _B_k;
	vector < complex <double>> _C_k;
	vector < complex <double>> _D_k;

	vector < complex <double>> _alfa_k;
	vector < complex <double>> _beta_k;
	//Граничные условия
	double _ny_0;
	double _v_0;
	double _ny_n;
	double _v_n;

	bool _init;
private:
	complex <double> get_A_k();	//+
	complex <double> get_B_k();	//+
	complex <double> get_C_k();	//+
	complex <double> get_D_k_horizon_x(int index_x, int index_y);//+
	complex <double> get_D_k_vertical_y(int index_x, int index_y);//+

	//Вторые производные
	complex <double> derivative2_horizon_x(int index_x, int index_y);
	complex <double> derivative2_vertical_y(int index_x, int index_y);
public:
	vector < vector <complex <double> >> _frequency_fi;
	GridSweepMethod();
	GridSweepMethod(
		vector < vector <complex <double> >>* fi_start,
		double input_step, double input_time,
		double ny_0, double v_0,
		double ny_n, double v_n);
	void creat(
		vector < vector <complex <double> >>* fi_start,
		double input_step, double input_time,
		double ny_0, double v_0,
		double ny_n, double v_n);

	void next_step_horizon_x();
	void next_step_vertical_y();
	void next_step();

	vector < vector <complex <double> >>* get_data();

};
