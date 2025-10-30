#pragma once
#include <math.h>
#include "Vector3f.h"

class Matrix3x3_f
{
public:
	float values[9];

	Matrix3x3_f(float value = 0.0f)
	{
		for (int i = 0; i < 9; ++i)
		{
			if (i == 0 || i == 4 || i == 8)
			{
				values[i] = value;
			}
			else
			{
				values[i] = 0.0f;
			}
		}
	}

	Matrix3x3_f(float a11, float a12, float a13,
		float a21, float a22, float a23,
		float a31, float a32, float a33)
		: values{ a11, a12, a13,
				  a21, a22, a23,
				  a31, a32, a33 }
	{
	}

	Matrix3x3_f Transposed() const
	{
		return Matrix3x3_f{
			values[0], values[3], values[6],
			values[1], values[4], values[7],
			values[2], values[5], values[8]
		};
	}

	Matrix3x3_f Inversed()
	{
		float det =
			values[0] * (values[4] * values[8] - values[5] * values[7]) -
			values[1] * (values[3] * values[8] - values[5] * values[6]) +
			values[2] * (values[3] * values[7] - values[4] * values[6]);

		if (det == 0.0f)
		{
			return Matrix3x3_f{}; // return zero matrix if non-invertible
		}

		float invDet = 1.0f / det;

		return Matrix3x3_f{
			(values[4] * values[8] - values[5] * values[7]) * invDet,
			(values[2] * values[7] - values[1] * values[8]) * invDet,
			(values[1] * values[5] - values[2] * values[4]) * invDet,

			(values[5] * values[6] - values[3] * values[8]) * invDet,
			(values[0] * values[8] - values[2] * values[6]) * invDet,
			(values[0] * values[5] - values[2] * values[3]) * invDet,

			(values[3] * values[7] - values[4] * values[6]) * invDet,
			(values[1] * values[6] - values[0] * values[7]) * invDet,
			(values[0] * values[4] - values[1] * values[3]) * invDet
		};
	}



	static Matrix3x3_f Identity()
	{
		return Matrix3x3_f{ 1.0f };
	}

	static Matrix3x3_f UniformScale(float scale)
	{
		auto mat = Identity();
		mat.values[0] = mat.values[4] = mat.values[8] = scale;
		return mat;
	}

	static Matrix3x3_f Scale(Vector3f scale)
	{
		auto mat = Identity();
		mat.values[0] = scale.x;
		mat.values[4] = scale.y;
		mat.values[8] = scale.z;
		return mat;
	}

	static Matrix3x3_f RotationX(float radians)
	{
		return Matrix3x3_f{
			1.0f,  0.0f,					0.0f,
			0.0f,  float(cos(radians)),		float(-sin(radians)),
			0.0f,  float(sin(radians)),		float(cos(radians))
		};
	}

	static Matrix3x3_f RotationY(float radians)
	{
		return Matrix3x3_f{
			 float(cos(radians)),	0.0f,  float(sin(radians)),
			 0.0f,					1.0f,  0.0f,
			 float(-sin(radians)),	0.0f,  float(cos(radians))
		};
	}

	static Matrix3x3_f RotationZ(float radians)
	{
		return Matrix3x3_f{
			float(cos(radians)), float(-sin(radians)), 0.0f,
			float(sin(radians)), float(cos(radians)),  0.0f,
			0.0f,                0.0f,                 1.0f
		};
	}

	static Matrix3x3_f RotationXYZ(Vector3f rotation)
	{
		return RotationZ(rotation.z) * RotationY(rotation.y) * RotationX(rotation.x);
	}

	Matrix3x3_f operator*(Matrix3x3_f other)
	{
		Matrix3x3_f newMatrix = Matrix3x3_f{};
		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				int newMatIndex = row * 3 + col;
				int thisMatBeginIndex = row * 3;
				int otherMatBeginIndex = col;

				float sum = 0;

				for (int i = 0; i < 3; i++)
				{
					sum += values[thisMatBeginIndex + i] * other.values[3 * i + otherMatBeginIndex];
				}
				newMatrix.values[newMatIndex] = sum;
			}
		}
		return newMatrix;
	}

	Vector3f operator*(Vector3f vec)
	{
		return {
			values[0] * vec.x + values[1] * vec.y + values[2] * vec.z,
			values[3] * vec.x + values[4] * vec.y + values[5] * vec.z,
			values[6] * vec.x + values[7] * vec.y + values[8] * vec.z
		};
	}

	Matrix3x3_f operator*(float f)
	{
		Matrix3x3_f newMatrix = Matrix3x3_f{};
		for (int i = 0; i < 9; i++)
		{
			newMatrix.values[i] = values[i] * f;
		}
		return newMatrix;
	}

	Matrix3x3_f operator/(float f)
	{
		Matrix3x3_f newMatrix = Matrix3x3_f{};
		for (int i = 0; i < 9; i++)
		{
			newMatrix.values[i] = values[i] / f;
		}
		return newMatrix;
	}

	Matrix3x3_f operator+(Matrix3x3_f other)
	{
		Matrix3x3_f newMatrix = Matrix3x3_f{};
		for (int i = 0; i < 9; i++)
		{
			newMatrix.values[i] = values[i] + other.values[i];
		}
		return newMatrix;
	}

	Matrix3x3_f operator-(Matrix3x3_f other)
	{
		Matrix3x3_f newMatrix = Matrix3x3_f{};
		for (int i = 0; i < 9; i++)
		{
			newMatrix.values[i] = values[i] - other.values[i];
		}
		return newMatrix;
	}

};

