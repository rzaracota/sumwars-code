#include "VisibleEntity.h"
#include "..\Common\Include\d3dutil.h"
#include "Math.h"

VisibleEntity::VisibleEntity()
{
	 D3DUtil_InitMaterial (mat, .75f,.75f,.75f,.75f);
	 scale=1.0f;
}

VisibleEntity::~VisibleEntity()
{}


void VisibleEntity::SetMaterial(float r, float g, float b, float a)
{
	D3DUtil_InitMaterial (mat, r,g,b,a);
}

void VisibleEntity::Render(long time)
{
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &oMatrix);
}

void VisibleEntity::SetRotationYawPitchRoll(float yaw, float pitch, float roll)
{
	D3DXMATRIX mat; D3DXMatrixRotationYawPitchRoll(&mat, yaw, pitch, roll);
	oMatrix*=mat;
	MultiplyInScale();
}

void VisibleEntity::LookAt(const D3DXVECTOR3& lookAt)
{
	D3DXMATRIX mat;
	static const D3DXVECTOR3 UP(0.0, 1.0, 0.0f);
	D3DXMatrixIdentity(&mat);

	LookAtMatrix(D3DXVECTOR3(oMatrix._41,oMatrix._42,oMatrix._43), lookAt, UP, oMatrix);
	MultiplyInScale();
}


void VisibleEntity::MultiplyInScale(void)
{
	if (scale!=1.0f)
	{
		D3DXMATRIX scaleMatrix;
		ZeroMemory(&scaleMatrix, sizeof(D3DXMATRIX));
		scaleMatrix._11=scale;
		scaleMatrix._22=scale;
		scaleMatrix._33=scale;
		scaleMatrix._44=1.0f;
		D3DXMatrixMultiply(&oMatrix, &scaleMatrix, &oMatrix);
	}
}