#pragma once
#include "DrawableBase.h"

class Sheet : public DrawableBase<Sheet>
{
public:
	Sheet(Graphics& gfx, float scaleoffset, float tx, float ty, float tz, std::string fn, float anx, float any, float anz,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	// translate
	float transX = 0.0f;
	float transY = 0.0f;
	float transZ = 0.0f;
	float radius;
	// scale
	float scaleOffset;
	// rotate
	float angleX;
	float angleY;
	float angleZ;
	// pic name
	std::string filename;

	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};