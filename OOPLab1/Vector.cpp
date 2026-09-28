#include "Vector.h"
#include <cmath>
using namespace std;

Vector::Vector(double x, double y, double z)
	: x_(x), y_(y), z_(z) {
}

double Vector::getX() const { return x_; }
void Vector::setX(double x) { x_ = x; }
double Vector::getY() const { return y_; }
void Vector::setY(double y) { y_ = y; }
double Vector::getZ() const { return z_; }
void Vector::setZ(double z) { z_ = z; }

void Vector::Input()
{
	cin >> x_ >> y_ >> z_;
}

void Vector::Output() const
{
	cout << "(" << x_ << ", " << y_ << ", " << z_ << ")";
}

double Vector::Lenght() const
{
	return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

Vector Vector::Normalize() const
{
	double len = Lenght();
	if (len == 0.0)
		return Vector();
	return Vector(x_ / len, y_ / len, z_ / len);
}

Vector Vector::operator+ (const Vector& other) const 
{
	return Vector(x_ + other.x_, y_ + other.y_, z_ + other.z_);
}

Vector Vector::operator- (const Vector& other) const
{
	return Vector(x_ - other.x_, y_ - other.y_, z_ - other.z_);
}

Vector Vector::operator* (double scalar) const
{
	return Vector(x_ * scalar, y_ * scalar, z_ * scalar);
}

bool Vector::operator== (const Vector& other) const
{
	return x_ == other.x_ && y_ == other.y_ && z_ == other.z_;
}

Vector Vector::Cross(const Vector& other) const
{
	return Vector(y_ * other.z_ - z_ * other.y_, z_ * other.x_ - x_ * other.z_, x_ * other.y_ - y_ * other.x_);
}

double Vector::Dot(const Vector& other) const
{
	return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
}

double Vector::Mixed(const Vector& b, const Vector& c) const 
{
	return Dot(b.Cross(c));
}

Vector Vector::DoubleCross(const Vector& b, const Vector& c) const
{
	return b * Dot(c) - c * Dot(b);
}

Vector operator* (double scalar, const Vector v)
{
	return v * scalar;
}

