#include "SkyTransfromCbuf.h"


//SkyboxTransformCbuf::SkyboxTransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot)
//	:
//	parent(parent),
//	pVcbuf{ std::make_unique<VertexConstantBuffer<Transforms>>(gfx,slot) }
//{}

SkyboxTransformCbuf::SkyboxTransformCbuf(Graphics& gfx, const Drawable& parent)
	:
	parent(parent)
{
	if (!pVcbuf)
	{
		pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx);
	}
}


void SkyboxTransformCbuf::Bind(Graphics& gfx) noexcept
{
	const Transforms& tf = GetTransforms(gfx);
	pVcbuf->Update(gfx, tf);
	pVcbuf->Bind(gfx);


	//INFOMAN_NOHR(gfx);
	//GFX_THROW_INFO_ONLY(UpdateBindImpl(gfx, GetTransforms(gfx)));
}

void SkyboxTransformCbuf::UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept
{
	pVcbuf->Update(gfx, tf);
	pVcbuf->Bind(gfx);
}

SkyboxTransformCbuf::Transforms SkyboxTransformCbuf::GetTransforms(Graphics& gfx) noexcept
{
	SkyboxTransformCbuf::Transforms res = {
		DirectX::XMMatrixTranspose(parent.GetTransformXM() * gfx.GetCamera() * gfx.GetProjection())
	};
	//SkyboxTransformCbuf::Transforms res = {
	//DirectX::XMMatrixTranspose(gfx.GetCamera() * gfx.GetProjection())
	//};
	return res;
}
