#pragma once
#include <math.h>
#include "Vector3f.h"
#include "Vector4f.h"

class Matrix4x4_f
{
public:
	float values[16];

	Matrix4x4_f(float value = 0.0f)
	{
		for(int i = 0; i < 16; ++i)
		{
			if(i == 0 || i == 5 || i == 10 || i == 15)
			{
				values[i] = value;
			} else
			{
				values[i] = 0.0f;
			}
			
		}
	}

	Matrix4x4_f(float a11, float a12, float a13, float a14,
				float a21, float a22, float a23, float a24,
				float a31, float a32, float a33, float a34,
				float a41, float a42, float a43, float a44)
					: values{ a11, a12, a13, a14,
							  a21, a22, a23, a24,
							  a31, a32, a33, a34,
							  a41, a42, a43, a44 }
	{
	}

	static Matrix4x4_f Identity()
	{
		return Matrix4x4_f{ 1.0f };
	}

	static Matrix4x4_f UniformScale(float scale)
	{
		auto mat = Identity();
		mat.values[0] = mat.values[5] = mat.values[10] = scale;
		return mat;
	}

	static Matrix4x4_f Scale(Vector3f scale)
	{
		auto mat = Identity();
		mat.values[0] = scale.x;
		mat.values[5] = scale.y;
		mat.values[10] = scale.z;
		return mat;
	}

	static Matrix4x4_f Translation(Vector3f position)
	{
		auto mat = Identity();
		mat.values[3] = position.x;
		mat.values[7] = position.y;
		mat.values[11] = position.z;
		return mat;
	}

	static Matrix4x4_f RotationX(float radians)
	{
		return Matrix4x4_f{
			1.0f,  0.0f,					0.0f,					 0.0f,
			0.0f,  float(cos(radians)),		float( - sin(radians)),	 0.0f,
			0.0f,  float(sin(radians)),		float(cos(radians)),	 0.0f,
			0.0f,  0.0f, 					0.0f,					 1.0f
		};
	}
	static Matrix4x4_f RotationY(float radians)
	{
		return Matrix4x4_f{
			 float(cos(radians)),		0.0f,  float(sin(radians)),  0.0f,
			 0.0f,						1.0f,  0.0f,				 0.0f,
			 float(-sin(radians)),		0.0f,  float(cos(radians)),  0.0f,
			 0.0f,						0.0f,  0.0f,				 1.0f
		};
	}

	static Matrix4x4_f RotationZ(float radians)
	{
		return Matrix4x4_f{
			float(cos(radians)), float(-sin(radians)),	0.0f,  0.0f,
			float(sin(radians)), float(cos(radians)),   0.0f,  0.0f,
			0.0f,                0.0f,					1.0f,  0.0f,
			0.0f,		         0.0f, 					0.0f,  1.0f

		};
	}

	static Matrix4x4_f RotationXYZ(Vector3f rotation)
	{
		return RotationZ(rotation.z) * RotationY(rotation.y) * RotationX(rotation.x);
	}

	static Matrix4x4_f Perspective(float fov, float nearZ, float farZ, float ratio)
	{
		float d = 1.0f / tan(fov / 2.0f);
		float a = -(farZ - nearZ) / (nearZ - farZ);
		float b = 2.0f * farZ * nearZ / (nearZ - farZ);
	
		return Matrix4x4_f{
			d/ratio, 0.0f, 0.0f, 0.0f,
			0.0f, d,    0.0f, 0.0f,
			0.0f, 0.0f, a,    b,
			0.0f, 0.0f, 1.0f, 0.0f
		};
	}

	Matrix4x4_f operator*(Matrix4x4_f other)
	{
		Matrix4x4_f newMatrix = Matrix4x4_f{};
		for(int row = 0; row < 4; row++)
		{
			for(int col = 0; col < 4; col++)
			{
				int newMatIndex = row * 4 + col;
				int thisMatBeginIndex = row * 4;
				int otherMatBeginIndex = col;

				float sum = 0;

				for(int i = 0; i < 4; i++)
				{
					sum += values[thisMatBeginIndex + i] * other.values[4 * i + otherMatBeginIndex];
				}
				newMatrix.values[newMatIndex] = sum;
			}
		}
		return newMatrix;
	}

	Vector4f operator*(Vector4f vec)
	{
		return {
			values[0] * vec.x + values[1] * vec.y + values[2] * vec.z + values[3] * vec.w,
			values[4] * vec.x + values[5] * vec.y + values[6] * vec.z + values[7] * vec.w,
			values[8] * vec.x + values[9] * vec.y + values[10] * vec.z + values[11] * vec.w,
			values[12] * vec.x + values[13] * vec.y + values[14] * vec.z + values[15] * vec.w
		};
		
	}

	Matrix4x4_f operator*(float f)
	{
		Matrix4x4_f newMatrix = Matrix4x4_f{};
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				int index = row * 4 + col;
				newMatrix.values[index] = values[index] * f;
			}
		}
		return newMatrix;
	}

	Matrix4x4_f operator/(float f)
	{
		Matrix4x4_f newMatrix = Matrix4x4_f{};
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				int index = row * 4 + col;
				newMatrix.values[index] = values[index] / f;
			}
		}
		return newMatrix;
	}

	Matrix4x4_f operator+(Matrix4x4_f other)
	{
		Matrix4x4_f newMatrix = Matrix4x4_f{};
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				int index = row * 4 + col;
				newMatrix.values[index] = values[index] + other.values[index];
			}
		}
		return newMatrix;
	}

	Matrix4x4_f operator-(Matrix4x4_f other)
	{
		Matrix4x4_f newMatrix = Matrix4x4_f{};
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				int index = row * 4 + col;
				newMatrix.values[index] = values[index] - other.values[index];
			}
		}
		return newMatrix;
	}

};
