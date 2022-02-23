#pragma once
#include "Drawable.h"
#include "DrawableBase.h"
#include <string>
#include <vector>

class Skybox : public DrawableBase<Skybox> 
{
public:
    Skybox(Graphics& gfx, float scaleoffset, float tx, float ty, float tz, float r,
        std::mt19937& rng,
        std::uniform_real_distribution<float>& adist,
        std::uniform_real_distribution<float>& ddist,
        std::uniform_real_distribution<float>& odist,
        std::uniform_real_distribution<float>& rdist,
        std::uniform_real_distribution<float>& bdist);
    void Update(float dt) noexcept override;
    DirectX::XMMATRIX GetTransformXM() const noexcept override;

    std::string filePath;
    // translate
    float transX = 0.0f;
    float transY = 0.0f;
    float transZ = 0.0f;
    float radius;
    // scale
    float scaleOffset;


    // model transform
    DirectX::XMFLOAT3X3 mt;


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