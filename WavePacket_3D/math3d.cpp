#define	_USE_MATH_DEFINES
#include "pch.h"
#include "math3d.h"
#include <string.h>
#include <math.h>

using namespace math3d;
#define M_PI       3.14159265358979323846   // pi
mat4::mat4(float m11, float m12, float m13,
	float m21, float m22, float m23,
	float m31, float m32, float m33,
	float dx, float dy, float dz ) {
	_matrix[0][0] = m11; _matrix[0][1] = m12; _matrix[0][2] = m13; _matrix[0][3] = 0;
	_matrix[1][0] = m21; _matrix[1][1] = m22; _matrix[1][2] = m23; _matrix[1][3] = 0;
	_matrix[2][0] = m31; _matrix[2][1] = m32; _matrix[2][2] = m33; _matrix[2][3] = 0;
	_matrix[3][0] = dx; _matrix[3][1] = dy; _matrix[3][2] = dz; _matrix[3][3] = 1;
}

mat4 & mat4::operator = (const mat4& right) {
	memcpy(_matrix, right._matrix, sizeof(right._matrix));
	return *this;
}

void mat4::reset() {
	_matrix[0][0] = 1; _matrix[0][1] = 0; _matrix[0][2] = 0; _matrix[0][3] = 0;
	_matrix[1][0] = 0; _matrix[1][1] = 1; _matrix[1][2] = 0; _matrix[1][3] = 0;
	_matrix[2][0] = 0; _matrix[2][1] = 0; _matrix[2][2] = 1; _matrix[2][3] = 0;
	_matrix[3][0] = 0; _matrix[3][1] = 0; _matrix[3][2] = 0; _matrix[3][3] = 1;
}

mat4 mat4::operator * (const mat4& right) {
	mat4 matrixReturn;
	for (int j = 0; j < 4; j++) {
		for (int jR = 0; jR < 4; jR++) {
			float newVal = 0;
			for (int i = 0; i < 4; i++) newVal += _matrix[j][i] * right._matrix[i][jR];
			matrixReturn._matrix[j][jR] = newVal;
		}
	}
	return matrixReturn;
}

mat4& mat4::rotateX(float angle) {
	double angelRad = (angle * M_PI) / 180;
	mat4 matrixRotateX(
		1, 0, 0,
		0, cos(angelRad), sin(angelRad),
		0, -sin(angelRad), cos(angelRad));
	*this = *this * matrixRotateX;
	return *this;
}

mat4& mat4::rotateY(float angle) {
	double angelRad = (angle * M_PI) / 180;
	mat4 matrixRotateX(
		cos(angelRad), 0, sin(angelRad),
		0, 1, 0,
		-sin(angelRad), 0, cos(angelRad));
	*this = *this * matrixRotateX;
	return *this;
}

mat4& mat4::rotateZ(float angle) {
	double angelRad = (angle * M_PI) / 180;
	mat4 matrixRotateX(
		cos(angelRad), -sin(angelRad), 0,
		sin(angelRad), cos(angelRad), 0,
		0, 0, 1);
	*this = *this * matrixRotateX;
	return *this;
}

mat4& mat4::translate(float x, float y, float z) {
	mat4 matrixTranslate(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1,
		x, y, z);
	*this = *this * matrixTranslate;
	return *this;
}

mat4& mat4::scale(float x, float y, float z) {
	mat4 matrixTranslate(
		x, 0, 0,
		0, y, 0,
		0, 0, z);
	*this = *this * matrixTranslate;
	return *this;
}

mat4& mat4::perspective(float center) {
	mat4 matrixTranslate;
	matrixTranslate._matrix[0][3] = -1 / center;
	*this = *this * matrixTranslate;
	return *this;
}

//Определения для векторов
vec4::vec4(float x, float y, float z) {
	_vector[0] = x;
	_vector[1] = y;
	_vector[2] = z;
	_vector[3] = 1;
}

vec4& vec4::operator = (const vec4& right) {
	memcpy(_vector, right._vector, sizeof(right._vector));
	return *this;
}
//!
//! /*
vec4 vec4::operator * (const mat4& right) {
	vec4 vectorReturn;
	for (int mat_j = 0; mat_j < 4; mat_j++) {
		float newVal = 0;
		for (int vec_i = 0; vec_i < 4; vec_i++) {
			newVal +=  _vector[vec_i] * right._matrix[vec_i][mat_j];
		}
		vectorReturn._vector[mat_j] = newVal;
	}

	double last = vectorReturn._vector[3];
	for (int i = 0; i < 4; i++) vectorReturn._vector[i] /= last;

	return vectorReturn;
}