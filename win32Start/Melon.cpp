#include "Melon.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Sphere.h"


Melon::Melon(Graphics& gfx,
	float scaleoffset, float tx, float ty, float tz, float r, float rotatespeed,
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
	radius(r),
	// rotate speed
	rotateSpeed(rotatespeed),
	//r(rdist(rng)),
	//droll(ddist(rng)),
	//dpitch(ddist(rng)),
	//dyaw(ddist(rng)),
	//dphi(odist(rng)),
	//dtheta(odist(rng)),
	//dchi(odist(rng)),
	chi(0.0f),
	theta(0.0f),
	phi(0.0f)
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
	roll += rotateSpeed * dt;
	//pitch += dpitch * dt;
	//yaw += dyaw * dt;
	//theta += dtheta * dt;
	//phi += dphi * dt;
	//chi += dchi * dt;

	if (radius != 0) {
		last_angle = angle;
		angle = angle + 50.0f * dt;
		//transX += angle;
		float ofx = radius * cos(angle * PI / 180.0f);
		float ofy = radius * sin(angle * PI / 180.0f);

		transX = ofx;
		transY = ofy;

		//transY += (radius * cos(to_rad(angle)) - radius * cos(to_rad(last_angle))) * 0.1f;

		//transX += dt;
	}

}

DirectX::XMMATRIX Melon::GetTransformXM() const noexcept
{
	namespace dx = DirectX;


	// 圆周运动 旋转再平移
	//dx::XMFLOAT3 vec(0.0f, 1.0f, 0.0f);
	//dx::XMVECTOR axis = dx::XMLoadFloat3(&vec);
	//dx::XMMATRIX cycleMat = dx::XMMatrixRotationAxis(axis, angle);
	
	return dx::XMMatrixScaling(scaleOffset, scaleOffset, scaleOffset) * // 缩放矩阵
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *  // 旋转矩阵 欧拉角
		//cycleMat *
		dx::XMMatrixTranslation(transX, transY, transZ) *  // 平移矩阵
		//dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		dx::XMMatrixTranslation(0.0f, 0.0f, 10.0f);
}

