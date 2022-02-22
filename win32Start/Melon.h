#pragma once
#include "DrawableBase.h"

class Melon : public DrawableBase<Melon>
{
public:
    Melon(Graphics& gfx, float scaleoffset, float tx, float ty, float tz, float r, float rotatespeed, std::string fn,
        std::mt19937& rng,
        std::uniform_real_distribution<float>& adist,
        std::uniform_real_distribution<float>& ddist,
        std::uniform_real_distribution<float>& odist,
        std::uniform_real_distribution<float>& rdist,
        std::uniform_int_distribution<int>& longdist,
        std::uniform_int_distribution<int>& latdist);
    void Update(float dt) noexcept override;
    DirectX::XMMATRIX GetTransformXM() const noexcept override;

public:
    // translate
    float transX = 0.0f;
    float transY = 0.0f;
    float transZ = 0.0f;
    std::string filename;
private:
    // pos
    struct {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    } pos;
    // scale
    float scaleOffset;

    // rotate speed
    float rotateSpeed = 0.0f;
    // rotate
    float angle = 0.0f;
    float last_angle = 0.0f;
    // cycle size
    float radius;
    // positional
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