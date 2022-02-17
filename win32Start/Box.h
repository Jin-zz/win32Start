#pragma once
#include "Drawable.h"
#include "DrawableBase.h"
#include <string>
#include <vector>

class Box : public DrawableBase<Box>
{
public:
    Box(Graphics& gfx, float scaleoffset, float tx, float ty, float tz, float r,
        std::mt19937& rng,
        std::uniform_real_distribution<float>& adist,
        std::uniform_real_distribution<float>& ddist,
        std::uniform_real_distribution<float>& odist,
        std::uniform_real_distribution<float>& rdist,
        std::uniform_real_distribution<float>& bdist);
    void Update(float dt) noexcept override;
    DirectX::XMMATRIX GetTransformXM() const noexcept override;
    bool isNearEarth() noexcept;
    bool isNearMoon(float moonPosx, float moonPosy, float moonPosz) noexcept;
    float getDis(std::vector<float>v1, std::vector<float>v2) noexcept {
        float res = 0.0f;
        for (int i = 0; i < v1.size(); i++) {
            res += (v1[i] - v2[i]) * (v1[i] - v2[i]);
        }
        res = std::sqrt(res);
        return res;
    }
    void getMoonPos(float x, float y, float z) {
        transX_moon = x;
        transY_moon = y;
        transZ_moon = z;
    }
    DirectX::XMVECTOR tangent = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    DirectX::XMVECTOR UpDirection = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    //void setTangent();
    //DirectX::XMVECTOR getTangentPos(float dis);
    void updateCamera();

public:
    enum class State
    {
        in_earth,
        in_earthtomoon,
        in_moon,
        nothing
    };

    // change obit
    State orbitName = State::in_earth;  // in_earth in_moon in_earthtomoon
    State last_orbitName = State::nothing;
    bool isStart = false; // 月球轨道切换 地月轨道 是否进入地月轨道
    bool isComplete = false; // 月球轨道切换 地月轨道 是否进入地月轨道
    float radius_inMoon = 0.0f;

    // translate
    float transX = 0.0f;
    float transY = 0.0f;
    float transZ = 0.0f;

    float transX_moon = 0.0f;
    float transY_moon = 0.0f;
    float transZ_moon = 0.0f;

    //camera 
    DirectX::XMVECTOR cameraPos = DirectX::XMVectorSet(0.1f, 0.0f, 0.0f, 0.0f);
    DirectX::XMVECTOR cameraFocus = DirectX::XMVectorSet(0.0f, 1.0, 0.0, 0.0f);
    DirectX::XMVECTOR cameraUp = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    float cameraX0 = 0.0f;
    float cameraY0 = 0.0f; // first camera = 0.0f ; third camera = 10.f
    float cameraZ0 = 0.0f;



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
    float moon_angle = 0.0f;
    // cycle size
    float radius;

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

    // model transform
    DirectX::XMFLOAT3X3 mt;

};