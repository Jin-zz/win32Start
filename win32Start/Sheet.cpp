#include "Sheet.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Plane.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"
#include "Rasterizer.h"


Sheet::Sheet(Graphics& gfx, float scaleoffset, float tx, float ty, float tz, std::string fn, float anx, float any, float anz,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist)
	:
	// pic name
	filename(fn),
	// scale
	scaleOffset(scaleoffset),
	// translate
	transX(tx),
	transY(ty),
	transZ(tz),
	angleX(anx),
	angleY(any),
	angleZ(anz)


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
		auto model = Plane::Make<Vertex>();
		model.vertices[0].tex = { 0.0f,0.0f };
		model.vertices[1].tex = { 0.0f,1.0f };
		model.vertices[2].tex = { 1.0f,0.0f };
		model.vertices[3].tex = { 1.0f,1.0f };

		AddBind(std::make_unique<Texture>(gfx, Surface::FromFile(filename)));

		AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		AddBind(std::make_unique<Sampler>(gfx));

		auto pvs = std::make_unique<VertexShader>(gfx, L"TextureVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddBind(std::move(pvs));

		AddBind(std::make_unique<PixelShader>(gfx, L"TexturePS.cso"));

		AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		//AddBind(std::make_unique<Rasterizer>(gfx, true));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Sheet::Update(float dt) noexcept
{
	//roll += droll * dt;
	//pitch += dpitch * dt;
	//yaw += dyaw * dt;
	//theta += dtheta * dt;
	//phi += dphi * dt;
	//chi += dchi * dt;
}

DirectX::XMMATRIX Sheet::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return 
		//dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		//dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(angleX * PI / 180.0f, angleY * PI / 180.0f, angleZ * PI / 180.0f) *
		dx::XMMatrixScaling(scaleOffset, scaleOffset, scaleOffset)*
		dx::XMMatrixTranslation(transX, transY, transZ);
}
