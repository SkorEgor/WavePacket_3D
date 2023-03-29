#pragma once

namespace math3d
{
	/////////////////////////////////////////////////
	// ������� �������������� � 3-������ ������������
	/////////////////////////////////////////////////
	class mat4
	{
		friend class vec4;

		// ���� �������
		static const int _rank = 4;

		// �������� �������
		float _matrix[_rank][_rank];

	public:

		// ����������� �� ���������
		mat4() { reset(); }

		// �����������
		mat4(float m11, float m12, float m13,
			float m21, float m22, float m23,
			float m31, float m32, float m33,
			float dx = 0, float dy = 0, float dz = 0);

		// �������� ������������
		mat4 & operator = (const mat4 &right);

		// �������� ��������� �� �������
		mat4 operator * (const mat4 &right);

		// ����� ������� � ���������
		void reset();

		// ������� ������������ ��� X. ���� � ��������!
		mat4 &rotateX(float angle);

		// ������� ������������ ��� Y. ���� � ��������!
		mat4 &rotateY(float angle);

		// ������� ������������ ��� Z. ���� � ��������!
		mat4 &rotateZ(float angle);

		// ������� (�����)
		mat4 &translate(float x, float y, float z);

		// ���������������
		mat4 &scale(float x, float y, float z);

		// ������������� �������������� ����� ��� X
		mat4 &perspective(float center);
	};

	// ------------------------------------------------------------

	///////////////////////////////////////////////////////////////
	// ������ - ���������� ���������� ����� � 3-������ ������������
	///////////////////////////////////////////////////////////////
	class vec4
	{
		// ����������� �������
		static const int _size = 4;

		// �������� �������
		float _vector[_size];

	public:

		// �����������
		vec4(float x = 0, float y = 0, float z = 0);

		// �������� ������������
		vec4 &operator = (const vec4 &right);

		// �������� ��������� �� �������
		vec4 operator * (const mat4 &right);

		// �������� ������ �� ���������� X
		float &x() const { return (float &)_vector[0]; }

		// �������� ������ �� ���������� Y
		float &y() const { return (float &)_vector[1]; }

		// �������� ������ �� ���������� Z
		float &z() const { return (float &)_vector[2]; }
	};
}
