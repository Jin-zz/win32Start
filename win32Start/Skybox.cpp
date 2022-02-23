#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Cube.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"
#include "Skybox.h"
#include "CubeTexture.h"
#include "Bindable.h"
#include "Rasterizer.h"
#include "SkyTransfromCbuf.h"


Skybox::Skybox(Graphics& gfx, float scaleoffset, float tx, float ty, float tz, float r,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist)
	:
	// scale
	scaleOffset(scaleoffset),
	// translate
	transX(tx),
	transY(ty),
	transZ(tz),
	radius(r),

	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))

{
	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			struct
			{
				float u;
				float v;
			} tex;
		};

		const auto model = Cube::MakeSkinned<Vertex>();
		//const auto model = Cube::Make<Vertex>();

		AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		// 添加纹理资源
		AddStaticBind(std::make_unique<Texture>(gfx, Surface::FromFile("Image\\SpaceBox\\0.jpg")));
		//AddBind(std::make_unique<Bind::CubeTexture>(gfx, "Image\\SpaceBox"));

		// 绑定采样器
		AddBind(std::make_unique<Sampler>(gfx));

		// 常量缓冲区
		//AddBind(std::make_unique<SkyboxTransformCbuf>(gfx, *this));
		AddBind(std::make_unique<TransformCbuf>(gfx, *this));

		auto pvs = std::make_unique<VertexShader>(gfx, L"TextureVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddBind(std::move(pvs));

		AddBind(std::make_unique<PixelShader>(gfx, L"TexturePS.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));


		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }  // 纹理信息

		};
		AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	
		AddBind(std::make_unique<Rasterizer>(gfx, true));


	}
	else
	{
		SetIndexFromStatic();
	}

	//AddBind(std::make_unique<TransformCbuf>(gfx, *this));

	// model deformation transform (per instance, not stored as bind)
	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling(scaleOffset, scaleOffset, scaleOffset)
	);
}

void Skybox::Update(float dt) noexcept
{

}

DirectX::XMMATRIX Skybox::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) *
		//dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		//dx::XMMatrixTranslation(radius, 0.0f, 0.0f) *
		//dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		dx::XMMatrixTranslation(transX, transY, transZ);
}