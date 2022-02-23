#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

class Camera;

class SkyboxTransformCbuf : public Bindable
{
protected:
	struct Transforms
	{
		DirectX::XMMATRIX viewProj;  // 顶点着色器的常量输入
	};
public:
	//SkyboxTransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot = 0u);
	SkyboxTransformCbuf(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx) noexcept override;
protected:
	void UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept;
	Transforms GetTransforms(Graphics& gfx) noexcept;
private:
	std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
	const Drawable& parent;

};
