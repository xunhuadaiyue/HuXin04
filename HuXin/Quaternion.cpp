#include "pch.h"
#include "Quaternion.h"
#include <math.h>

#define CLAMP(x , min , max) ((x) > (max) ? (max) : ((x) < (min) ? (min) : x))

Quaternion::Quaternion(void)
{
	qw=0;
	qx=0;
	qy=0;
	qz=0;
}


Quaternion::~Quaternion(void)
{
}

void Quaternion::SetEulerAngle(float x_rotate, float y_rotate, float z_rotate)
{	
	x_rotate = x_rotate/180.0*3.14159;
	y_rotate = y_rotate/180.0*3.14159;
	z_rotate = z_rotate/180.0*3.14159;

	float fCosZ = cos(z_rotate * .5f);
	float fSinZ = sin(z_rotate * .5f);
	float fCosX = cos(x_rotate * .5f);
	float fSinX = sin(x_rotate * .5f);
	float fCosY = cos(y_rotate * .5f);
	float fSinY = sin(y_rotate * .5f);

	qw = fCosZ * fCosX * fCosY + fSinZ * fSinX * fSinY;
	qx = fCosZ * fSinX * fCosY - fSinZ * fCosX * fSinY;
	qy = fCosZ * fCosX * fSinY + fSinZ * fSinX * fCosY;
	qz = fSinZ * fCosX * fCosY - fCosZ * fSinX * fSinY;
}


 void Quaternion::GetEulerAngle(float& x_rotate, float& y_rotate, float& z_rotate) 
{
	x_rotate  = atan2(2 * (qw * qx + qz * qy) , 1 - 2 * (qy * qy + qx * qx));
	y_rotate  = asin(CLAMP(2 * (qw * qy - qx * qz) , -1.0f , 1.0f));
	z_rotate  = atan2(2 * (qw * qz + qx * qy) , 1 - 2 * (qz * qz + qy * qy));

	x_rotate = x_rotate/3.14159*180;
	y_rotate = y_rotate/3.14159*180;
	z_rotate = z_rotate/3.14159*180;
}