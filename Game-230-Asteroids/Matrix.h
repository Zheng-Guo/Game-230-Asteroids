#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Matrix {
private:
	float m11, m12, m21, m22;
public:
	Matrix(float m1 = 0, float m2 = 0, float m3 = 0, float m4 = 0) :m11(m1),
		m12(m2),
		m21(m3),
		m22(m4) {

	}
	Vector2f operator*(const Vector2f &v);
};

Vector2f Matrix::operator*(const Vector2f &v) {
	float x = m11*v.x + m12*v.y;
	float y = m21*v.x + m22*v.y;
	return Vector2f(x, y);
}