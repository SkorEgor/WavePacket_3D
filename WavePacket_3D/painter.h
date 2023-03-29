#pragma once
#include <gdiplus.h>
#include "math3d.h"
#include <vector>
#include <complex>
using namespace Gdiplus;
using namespace math3d;
using namespace std;

struct min_max {
	double min, max;
};

class painter : public CStatic
{
	DECLARE_DYNAMIC(painter)
private:
	vector < vector <complex<double>> >* _val_z;

	int _angelX ;
	int _angelY ;
	int _angelZ ;
	double _k ;
	bool _brash;
	bool _persp;
public:
	painter();
	virtual ~painter();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void OnKeyDown(double in_angelY, double in_angelZ, bool in_persp, bool in_brash);
	void updateData(vector < vector <complex<double>> >&in_val);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


