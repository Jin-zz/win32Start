#include "App.h"
#include "Melon.h"
#include "Pyramid.h"
#include <sstream>
#include <iomanip>
#include "Box.h"
#include <memory>
#include <algorithm>
#include "ChiliMath.h"
#include <iostream>
#include "Surface.h"
#include "DirectXTex.h"
#include "Sheet.h"
#include "Skybox.h"
#include "Bindable.h"


// 构造函数中创建窗口
App::App()
	:
	wnd(800, 600, "The Donkey Fart Box")
{

	Graphics& gfx = wnd.Gfx();
	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
	std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
	std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
	std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
	std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
	std::uniform_int_distribution<int> latdist{ 5,20 };
	std::uniform_int_distribution<int> longdist{ 10,40 };
	std::uniform_int_distribution<int> typedist{ 0,2 };

	drawables.push_back(std::make_unique<Melon>(
		gfx, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, "Image\\earth.jpg", rng, adist, ddist,
		odist, rdist, longdist, latdist
		));

	drawables.push_back(std::make_unique<Melon>(
		gfx, 0.5f, 0.0f, 0.0f, 0.0f, 7.0f, 1.0f,"Image\\moon.jpg", rng, adist, ddist,
		odist, rdist, longdist, latdist
		));

	boxs.push_back(std::make_unique<Box>(
		gfx, 0.1f, 5.0f, 0.0f, 0.0f, 3.0f, rng, adist, ddist,
	    odist, rdist, bdist
		)); 


	sheets.push_back(std::make_unique<Sheet>(
    gfx, 15.0f, 0.0f, 0.0f, 15.0f, "Image\\SpaceBox\\0.png", 0.0f, 0.0f, 270.0f, rng, adist, ddist,
    odist, rdist
    ));

	sheets.push_back(std::make_unique<Sheet>(
		gfx, 15.0f, 0.0f, 0.0f, -15.0f, "Image\\SpaceBox\\1.png", 0.0f, 180.0f, 270.0f, rng, adist, ddist,
		odist, rdist
		));

	sheets.push_back(std::make_unique<Sheet>(
		gfx, 15.0f, -15.0f, 0.0f, 0.0f, "Image\\SpaceBox\\4.png", 0.0f, 270.0f, 270.0f, rng, adist, ddist,
		odist, rdist
		));

	sheets.push_back(std::make_unique<Sheet>(
		gfx, 15.0f, 15.0f, 0.0f, 0.0f, "Image\\SpaceBox\\5.png", 0.0f, 90.0f, 270.0f, rng, adist, ddist,
		odist, rdist
		));

	sheets.push_back(std::make_unique<Sheet>(
		gfx, 15.0f, 0.0f, 15.0f, 0.0f, "Image\\SpaceBox\\2.png", 270.0f, 0.0f, 180.0f, rng, adist, ddist,
		odist, rdist
		));
	sheets.push_back(std::make_unique<Sheet>(
		gfx, 15.0f, 0.0f, -15.0f, 0.0f, "Image\\SpaceBox\\3.png", 90.0, 0.0f, 0.0f, rng, adist, ddist,
		odist, rdist
		));

	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));

}

// 处理消息内容
int App::Go()
{
	while (true)
	{
		// process all messages pending, but to not block for new messag es
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}

App::~App()
{}

void App::DoFrame()
{
	Melon* earth = drawables[0].get();
	Melon* moon = drawables[1].get();
	Box* box = boxs[0].get();

	const auto dt = timer.Mark();
	wnd.Gfx().ClearBuffer(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cam.GetMatrix(box));
	//wnd.Gfx().SetCamera(DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f));

	box->getMoonPos(moon->transX, moon->transY, moon->transZ);

	while (const auto e = wnd.kbd.ReadKey()) {
		Box::State state = boxs[0].get()->orbitName;
		char key = e.value().GetCode();
		switch (key) {
		case 'A': {
			// 切换地月轨道
			if (state == Box::State::in_earthtomoon) break;
			if (box->last_orbitName == Box::State::in_moon && box->isStart == false) {
				box->isStart = true;
			}

			box->last_orbitName = box->orbitName;
			box->orbitName = Box::State::in_earthtomoon;
			break;
		}
		case 'S': {
			// 切换地球轨道
			if (state == Box::State::in_earth || state == Box::State::in_moon) break;

			if (box->isNearEarth()) {
				box->orbitName = Box::State::in_earth;
				box->last_orbitName = Box::State::in_earth;
			}
			else {
				MessageBox(nullptr, "not in near Earth point", "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
			}
			break;
		}
		case 'D': {
			// 切换月球轨道
			if (state == Box::State::in_moon || state == Box::State::in_earth) break;

			if (box->isNearMoon(moon->transX * 2.0f, moon->transY * 2.0f, moon->transZ * 2.0f)) {
				box->isComplete = false;
				box->orbitName = Box::State::in_moon;
				box->last_orbitName = Box::State::in_moon;

			}
			else {
				MessageBox(nullptr, "not in near Moon point ", "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
			}
			break;
		}
		case 'Z': {
			// 固定摄像机
			cam.state = Camera::State::DefaultPerson;
			break;
		}
		case 'X': {
			// 第一人称摄像机
			cam.state = Camera::State::FirstPerson;
			box->cameraY0 = 0.0f;
			cam.pitch = 0.0f;
			cam.yaw = 0.0f;
			box->cameraX0 = 0.0f;
			box->cameraZ0 = 0.0f;
			break;
		}
		case 'C': {
			// 第三人称摄像机
			cam.state = Camera::State::ThirdPerson;
			cam.pitch = 0.0f;
			cam.yaw = 0.0f;
			box->cameraY0 = 10.0f;
			box->cameraX0 = 0.0f;
			box->cameraZ0 = 0.0f;

			break;
		}
		

		}
	}

	for (auto& d : sheets)
	{
		d->Update(dt);
		d->Draw(wnd.Gfx());
	}


	for (auto& d : drawables)
	{
		d->Update(dt);
		d->Draw(wnd.Gfx());
	}


	for (auto& d : boxs)
	{
		d->Update(dt);
		d->Draw(wnd.Gfx());
	}

	//for (auto& d : skyboxs)
	//{
	//	d->Update(dt);
	//	d->Draw(wnd.Gfx());
	//}



	while (const auto e = wnd.mouse.Read())
	{
		switch (e->GetType())
		{
			case Mouse::Event::Type::Move:
			{
				updateCameraRotateMouse(e->GetPosX(), e->GetPosY());
				/*std::ostringstream oss;
				oss << "Mouse moved to: (" << cameraRotateX << "," << cameraRotateY << ")\n";
				wnd.SetTitle(oss.str());*/
				break;
			}
			case Mouse::Event::Type::WheelUp:
			case Mouse::Event::Type::WheelDown:
			{
				updateThirdCameraDisY(e->getWheelDelta());

				std::ostringstream oss;
				oss << "Mouse moved to: (" << box->cameraY0 << ")\n";
				wnd.SetTitle(oss.str());
				break;
			}

		}
	}


	wnd.Gfx().EndFrame();
}

void App::updateCameraRotateMouse(float posx, float posy) {
	switch (cam.state) {
	case Camera::State::FirstPerson:
		cameraRotateX = posx / wnd.width * 360.0f - 180.0f;
		//cameraRotateX = posx / wnd.width * 180.0f - 90.0f;
		cameraRotateY = -posy / wnd.height * 90.0f + 45.0f;
		cameraRotateX = cameraRotateX * PI / 180.0f;
		cameraRotateY = cameraRotateY * PI / 180.0f;
		cam.pitch = cameraRotateY;
		cam.yaw = cameraRotateX;
		break;

	case Camera::State::ThirdPerson:

		cameraRotateX = posx / wnd.width * 10.0f - 5.0f;
		cameraRotateY = posy / wnd.height * 10.0f - 5.0f;

		cameraRotateX = abs(cameraRotateX) > abs(cameraRotateY) ? cameraRotateX : 0.0f;
		cameraRotateY = abs(cameraRotateX) > abs(cameraRotateY) ? 0.0f : cameraRotateY;


		Box* box = boxs[0].get();
		box->cameraX0 = cameraRotateY;
		box->cameraZ0 = cameraRotateX;
	}
}


void App::updateThirdCameraDisY(float delta) {
	Box* box = boxs[0].get();
	switch (cam.state) {
	case Camera::State::FirstPerson: {
		break;
	}
	case Camera::State::ThirdPerson: {
		// box->cameraY0 defalut = 10.0f 
		if (box->cameraY0 >= 16.0f && delta > 0) {
			return;
		}
		if (box->cameraY0 <= 7.0f && delta < 0) {
			return;
		}
		box->cameraY0 += delta * 0.1f;
		break;
	}
	}

}