#include "../../inc/math/vec4.h"

Vec4::Vec4()
{
	this->_x = this->_y = this->_z = this->_w = 0.0;
}

Vec4::Vec4(double x, double y, double z, double w)
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
	this->_w = w;
}