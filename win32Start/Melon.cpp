#include "Melon.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Sphere.h"


Melon::Melon(Graphics& gfx,
	float scaleoffset, float tx, float ty, float tz, float ang,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_int_distribution<int>& longdist,
	std::uniform_int_distribution<int>& latdist)
	:
	// scale
	scaleOffset(scaleoffset),
	// translate
	transX(tx),
	transY(ty),
	transZ(tz),
	angle(ang),
	//r(rdist(rng)),
	droll(ddist(rng)),
	//dpitch(ddist(rng)),
	//dyaw(ddist(rng)),
	//dphi(odist(rng)),
	//dtheta(odist(rng)),
	//dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		auto pvs = std::make_unique<VertexShader>(gfx, L"ColorIndexVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"ColorIndexPS.cso"));

		struct PixelShaderConstants
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[8];
		};
		const PixelShaderConstants cb2 =
		{
			{
				{ 1.0f,1.0f,1.0f },
				{ 1.0f,0.0f,0.0f },
				{ 0.0f,1.0f,0.0f },
				{ 1.0f,1.0f,0.0f },
				{ 0.0f,0.0f,1.0f },
				{ 1.0f,0.0f,1.0f },
				{ 0.0f,1.0f,1.0f },
				{ 0.0f,0.0f,0.0f },
			}
		};
		AddStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(gfx, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	struct Vertex
	{
		dx::XMFLOAT3 pos;
	};
	auto model = Sphere::MakeTesselated<Vertex>(latdist(rng), longdist(rng));
	// deform vertices of model by linear transformation
	model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 1.0f));

	AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Melon::Update(float dt) noexcept
{
	roll += droll * dt;
	//pitch += dpitch * dt;
	//yaw += dyaw * dt;
	//theta += dtheta * dt;
	//phi += dphi * dt;
	//chi += dchi * dt;
}

DirectX::XMMATRIX Melon::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	dx::XMFLOAT3 vec(0.0f, 1.0f, 0.0f);
	dx::XMVECTOR axis = dx::XMLoadFloat3(&vec);
	
	return dx::XMMatrixScaling(scaleOffset, scaleOffset, scaleOffset) * // Ëõ·Å¾ØÕó
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *  // Ðý×ª¾ØÕó Å·À­½Ç
		dx::XMMatrixRotationAxis(axis, angle) *
		dx::XMMatrixTranslation(transX, transY, transZ) *  // Æ½ÒÆ¾ØÕó
		//dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		dx::XMMatrixTranslation(0.0f, 0.0f, 10.0f);
}

