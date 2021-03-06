#include "Box.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Sphere.h"
#include "Cube.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"

Box::Box(Graphics& gfx, float scaleoffset, float tx, float ty, float tz, float r,
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
	radius(r)

	//r(rdist(rng)),
	//droll(ddist(rng)),
	//dpitch(ddist(rng)),
	//dyaw(ddist(rng)),
	//dphi(odist(rng)),
	//dtheta(odist(rng)),
	//dchi(odist(rng)),
	//chi(adist(rng)),
	//theta(adist(rng)),
	//phi(adist(rng))
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

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
		// 添加纹理资源
		AddStaticBind(std::make_unique<Texture>(gfx, Surface::FromFile("Image\\wall.jpg")));

		// 绑定采样器
		AddStaticBind(std::make_unique<Sampler>(gfx));

		auto pvs = std::make_unique<VertexShader>(gfx, L"TextureVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"TexturePS.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }  // 纹理信息
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));

	// model deformation transform (per instance, not stored as bind)
	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng))
	);
}

//void Box::setTangent() {
//	namespace dx = DirectX;
//	tangent = dx::XMVector3Transform(
//		dx::XMVectorSet(transX * 2.0f, transY * 2.0f,transZ * 2.0f, 0.0f),
//		dx::XMMatrixRotationZ(90)
//	);
//	tangent = dx::XMVector3Transform(tangent,
//		dx::XMMatrixTranslation(transX, transY, transZ)
//	);
//	UpDirection = dx::XMVectorSet(transX, transY, transZ, 0.0f);
//
//}

//DirectX::XMVECTOR Box::getTangentPos(float dis) {
//	namespace dx = DirectX;
//	DirectX::XMVECTOR point;
//	point = dx::XMVector3Transform(
//		dx::XMVectorSet(transX * 0.5f, transY * 0.5f, transZ * 0.5f, 0.0f),
//		dx::XMMatrixRotationZ(90)
//	);
//	point = dx::XMVector3Transform(tangent,
//		dx::XMMatrixTranslation(transX, transY, transZ)
//	);
//
//	return point;
//
//}

void Box::updateCamera() {
	namespace dx = DirectX;

	cameraPos = dx::XMVector3Transform(
		dx::XMVector3Transform(
		dx::XMVectorSet(cameraX0, cameraY0, cameraZ0, 0.0f),
		GetTransformXM()
		),
		dx::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f* PI / 180.0f)
	);

	cameraFocus = dx::XMVector3Transform(
		dx::XMVectorSet(0.0f, 1.0, 0.0, 0.0f),
		GetTransformXM()
	);
	cameraUp = dx::XMVector3Transform(
		dx::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f),
		GetTransformXM()
	);

}


void Box::Update(float dt) noexcept
{
	//roll += droll * dt;
	//pitch += dpitch * dt;
	//yaw += dyaw * dt;
	//theta += dtheta * dt;
	//phi += dphi * dt;
	//chi += dchi * dt;

	updateCamera();

	if (radius != 0) {
		float ofx = 0.0f;
		float ofy = 0.0f;
		angle = angle + rotateSpeed * dt;

		if (orbitName == State::in_earth) {
			rotateSpeed = 50.0f;
			last_angle = 0.0f; // 重置
			ofx = radius * cos(angle * PI / 180.0f);
			ofy = radius * sin(angle * PI / 180.0f);
		}
		else if (orbitName == State::in_moon || isStart)
		{ 
			if (isStart) {
				if (int(angle - moon_angle) % 360 == 0) {
					isStart = false;
				}
			}
			rotateSpeed = 50.0f;
			last_angle = 0.0f; // 重置
			moon_angle = moon_angle == 0.0f ? angle : moon_angle;
			float angleInMoon = angle - moon_angle;

			float r = radius_inMoon;
			// 月球上点
			float moonx = r * cos(angleInMoon * PI / 180.0f);
			float moony = r * sin(angleInMoon * PI / 180.0f);
			// 旋转后的椭圆上点(实时)
			float moonxRotate = moonx * cos(moon_angle * PI / 180.0f) - moony * sin(moon_angle * PI / 180.0f);
			float moonyRotate = moonx * sin(moon_angle * PI / 180.0f)  + moony * cos(moon_angle * PI / 180.0f);

			ofx = moonxRotate  + transX_moon;
			ofy = moonyRotate + transY_moon;


		}
		else if(orbitName == State::in_earthtomoon && isStart == false)
		{

			last_angle = last_angle == 0.0f ? angle : last_angle;

			float a = 5.75f;
			float b = 4.0f;

			float angleInOval = angle - last_angle;

			// 椭圆右端点
			float ovalx0 = a * cos(0.0f * PI / 180.0f);
			float ovaly0 = b * sin(0.0f * PI / 180.0f);
			// 旋转后的椭圆右端点（初始点）
			float ovalx0Rotate = ovalx0 * cos(last_angle * PI / 180.0f) - ovaly0 * sin(last_angle * PI / 180.0f);
			float ovaly0Rotate = ovalx0 * sin(last_angle * PI / 180.0f) + ovaly0 * cos(last_angle * PI / 180.0f);


			// 椭圆上点
			float ovalx = a * cos(angleInOval * PI / 180.0f);
			float ovaly = b * sin(angleInOval * PI / 180.0f); 
			// 旋转后的椭圆上点(实时)
			float ovalxRotate = ovalx * cos(last_angle * PI / 180.0f) - ovaly * sin(last_angle * PI / 180.0f);
			float ovalyRotate = ovalx * sin(last_angle * PI / 180.0f) + ovaly * cos(last_angle * PI / 180.0f);

			float ofx0;
			float ofy0;
			// 点的初始位置 右端点
			if (last_orbitName == State::in_earth) {
				ofx0 = radius * cos(last_angle * PI / 180.0f);
				ofy0 = radius * sin(last_angle * PI / 180.0f);
			}
			else {
				float r = 7.0f + radius_inMoon;
				ofx0 = r * cos(last_angle * PI / 180.0f);
				ofy0 = r * sin(last_angle * PI / 180.0f);
			}
			


			rotateSpeed = 50.0f;

			ofx = ovalxRotate - (ovalx0Rotate - ofx0);
			ofy = ovalyRotate - (ovaly0Rotate - ofy0);
		}

		transX = ofx;
		transY = ofy;



	}
}

bool Box::isNearEarth() noexcept {
	std::vector<float> v1{ transX, transY, transZ};
	std::vector<float> v2{ 0.0f, 0.0f, 0.0f };
	float dis = getDis(v1, v2);
	float goalDis = 3.0f;  // 地球轨道半径
	if (dis > goalDis) {
		return false;
	}
	else {
		return true;
	}

	return false;
}

bool Box::isNearMoon(float moonPosx, float moonPosy, float moonPosz) noexcept {
	std::vector<float> v1{ transX * 2.0f, transY * 2.0f, transZ * 2.0f };
	std::vector<float> v2{ moonPosx, moonPosy, moonPosz };
	float dis = getDis(v1, v2);
	float goalDis = 3.2f;  // 月球轨道半径
	if (dis > goalDis) {
		return false;
	}
	else {
		radius_inMoon = dis / 2.0f;
		return true;
	}

	return false;

}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	// 圆周运动 旋转再平移
////dx::XMFLOAT3 vec(0.0f, 1.0f, 0.0f);
////dx::XMVECTOR axis = dx::XMLoadFloat3(&vec);
//dx::XMMATRIX cycleMat = dx::XMMatrixRotationAxis(axis, angle);

	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) *
		dx::XMMatrixScaling(scaleOffset, scaleOffset, scaleOffset) * // 缩放矩阵
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, angle * PI / 180.0f) *
		dx::XMMatrixTranslation(transX, transY, transZ);  // 平移矩阵
		//dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		//dx::XMMatrixTranslation(0.0f, 0.0f, 10.0f);
}

