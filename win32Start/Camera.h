#pragma once
#include "Graphics.h"
#include "Box.h"

class Camera
{
public:
    DirectX::XMMATRIX GetMatrix(Box* box) const noexcept;
    void SpawnControlWindow() noexcept;
    void Reset() noexcept;
public:
    //DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    //DirectX::XMVECTOR focus = DirectX::XMVectorSet(0.0f, 1.0, 0.0, 0.0f);
    //DirectX::XMVECTOR up = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    float pitch = 0.0f;
    float yaw = 0.0f;
    enum class State
    {
        DefaultPerson,
        FirstPerson,
        ThirdPerson
    };
    State state = State::DefaultPerson;
private:
    float r = 10.0f;
    float theta = 0.0f;
    float phi = 0.0f;
    float roll = 0.0f;
};