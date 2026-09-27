#pragma once
#include <iostream>

class Vector {
public:
	Vector() = default;
	Vector(double x, double y, double z);

	double getX() const;
	void setX(double x);
	double getY() const;
	void setY(double y);
	double getZ() const;
	void setZ(double z);

	void Input();
	void Output() const;

	double Lenght() const;
	Vector Normalize() const;
	Vector operator+ (const Vector& other) const;
	Vector operator- (const Vector& other) const;
	Vector operator* (double scalar) const;
	bool operator==(const Vector& other) const;
	double Dot(const Vector& other) const;
	Vector Cross(const Vector& other) const;
	double Mixed(const Vector& b, const Vector& c) const;
	Vector DoubleCross(const Vector& b, const Vector& c) const;

private:
	double x_ = 0.0, y_ = 0.0, z_ = 0.0;

};
