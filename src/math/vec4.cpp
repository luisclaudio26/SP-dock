#include "../../inc/math/vec4.h"

Vec4::Vec4()
{
	this->_x = this->_y = this->_z = 0.0;
	this->_w = 1.0;
}

Vec4::Vec4(double x, double y, double z, double w)
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
	this->_w = w;
}

Vec4 Vec4::operator+(const Vec4& rhs) const
{
	Vec4 out;
	out._x = this->_x + rhs.x();
	out._y = this->_y + rhs.y();
	out._z = this->_z + rhs.z();
	out._w = this->_w + rhs.w();

	return out;
}

Vec4 Vec4::operator*(double scalar) const
{
	Vec4 out;
	out._x = this->_x * scalar;
	out._y = this->_y * scalar;
	out._z = this->_z * scalar;
	out._w = this->_w * scalar;

	return out;
}


void Vec4::operator=(const Vec4& rhs)
{
	//HINT: Not sure what's the best way to implement this;
	//move semantics? Normal copy?
	this->_x = rhs.x();
	this->_y = rhs.y();
	this->_z = rhs.z();
	this->_w = rhs.w();
}

Vec4 Vec4::operator-() const
{
	return (*this) * (-1.0);
}

Vec4 Vec4::operator-(const Vec4& rhs) const
{
	return (*this) + (-rhs);
}

double Vec4::dot(const Vec4& rhs) const
{
	double x = this->_x * rhs.x();
	double y = this->_y * rhs.y();
	double z = this->_z * rhs.z();
	double w = this->_w * rhs.w();

	return x + y + z + w;
}

double Vec4::norm() const
{
	return sqrt( this->dot(*this) );
}

Vec4 Vec4::normalize() const
{
	return (*this) * (1.0/this->norm());
}