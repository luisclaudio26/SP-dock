#ifndef _VEC4_H_
#define _VEC4_H_

#include <cmath>

class Vec4
{
private:
	double _x, _y, _z, _w;

public:
	Vec4();
	Vec4(double x, double y, double z, double w);

	//-----------------------------------------
	//------------ Access methods -------------
	//-----------------------------------------
	double x() const { return this->_x; }
	double y() const { return this->_y; }
	double z() const { return this->_z; }
	double w() const { return this->_w; }

	//-------------------------------------
	//------------ Operations -------------
	//-------------------------------------
	double dot(const Vec4& rhs) const;
	double norm() const;
	Vec4 normalize() const;
	Vec4 operator+(const Vec4& rhs) const;
	Vec4 operator*(double scalar) const;
	Vec4 operator-() const;
	Vec4 operator-(const Vec4& rhs) const;
	void operator=(const Vec4& rhs);
};

#endif