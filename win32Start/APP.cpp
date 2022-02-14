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
		gfx, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, rng, adist, ddist,
		odist, rdist, longdist, latdist
		));

	drawables.push_back(std::make_unique<Melon>(
		gfx, 0.5f, 5.0f, 0.0f, 0.0f, 5.0f, 1.0f, rng, adist, ddist,
		odist, rdist, longdist, latdist
		));


	boxs.push_back(std::make_unique<Box>(
		gfx, 0.5f, 5.0f, 0.0f, 0.0f, 5.0f, rng, adist, ddist,
	    odist, rdist, bdist
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
	const auto dt = timer.Mark();
	wnd.Gfx().ClearBuffer(0.07f, 0.0f, 0.12f);

	Melon* earth = drawables[0].get();
	Melon* moon = drawables[1].get();
	Box* box = boxs[0].get();

	box->getMoonPos(moon->transX, moon->transY, moon->transZ);

	while (const auto e = wnd.kbd.ReadKey()) {
		Box::State state = boxs[0].get()->orbitName;
		// 切换地月轨道
		if (e.value().GetCode() == 'A') {
			if (state == Box::State::in_earthtomoon) break;
			if (box->last_orbitName == Box::State::in_moon && box->isStart == false) {
				box->isStart = true;
			}

			box->last_orbitName = box->orbitName;
			box->orbitName = Box::State::in_earthtomoon;
		}
		// 切换地球轨道
		else if (e.value().GetCode() == 'S') {
			if (state == Box::State::in_earth || state == Box::State::in_moon) break;

			if (box->isNearEarth()) {
				box->orbitName = Box::State::in_earth;
				box->last_orbitName = Box::State::in_earth;
			}
			else {
				MessageBox(nullptr, "not in near Earth point", "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
			}

		}
		// 切换月球轨道
		else if (e.value().GetCode() == 'D') {
			if (state == Box::State::in_moon || state == Box::State::in_earth) break;

			if (box->isNearMoon(moon->transX * 2.0f, moon->transY * 2.0f, moon->transZ * 2.0f)) {
				box->isComplete = false;
				box->orbitName = Box::State::in_moon;
				box->last_orbitName = Box::State::in_moon;

			}
			else {
				MessageBox(nullptr, "not in near Moon point ", "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
			}


		}
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
	wnd.Gfx().EndFrame();
}