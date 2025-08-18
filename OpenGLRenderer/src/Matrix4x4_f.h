#pragma once

class Matrix4x4_f
{
public:
	float matrix[16];

	Matrix4x4_f(float value = 0.0f)
	{
		for(int i = 0; i < 16; ++i)
		{
			if(i == 0 || i == 5 || i == 10 || i == 15)
			{
				matrix[i] = value;
			} else
			{
				matrix[i] = 0.0f;
			}
			
		}
	}

	Matrix4x4_f(float a11, float a12, float a13, float a14,
				float a21, float a22, float a23, float a24,
				float a31, float a32, float a33, float a34,
				float a41, float a42, float a43, float a44)
					: matrix{ a11, a12, a13, a14,
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
		mat.matrix[0] = mat.matrix[5] = mat.matrix[10] = scale;
		return mat;
	}

	static Matrix4x4_f Scale(Vector3f scale)
	{
		auto mat = Identity();
		mat.matrix[0] = scale.x;
		mat.matrix[5] = scale.y;
		mat.matrix[10] = scale.z;
		return mat;
	}

	static Matrix4x4_f Translation(Vector3f position)
	{
		auto mat = Identity();
		mat.matrix[3] = position.x;
		mat.matrix[7] = position.y;
		mat.matrix[11] = position.z;
		return mat;
	}

};
