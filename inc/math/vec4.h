#ifndef _VEC4_H_
#define _VEC4_H_

#include <cmath>

class Vec4
{
private:
	float _x, _y, _z, _w;

public:
	Vec4();
	Vec4(double x, double y, double z, double w);

	//-----------------------------------------
	//------------ Access methods -------------
	//-----------------------------------------
	double x() { return this->_x; }
	double y() { return this->_y; }
	double z() { return this->_z; }
};

#endif