#pragma once
class Quaternion
{
public:
	float qw;
	float qx;
	float qy;
	float qz;
public:
	Quaternion(void);
	~Quaternion(void);

	void SetEulerAngle(float x_rotate, float y_rotate, float z_rotate);
	void GetEulerAngle(float& x_rotate, float& y_rotate, float& z_rotate);
};

