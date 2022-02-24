#pragma once
#include "Window.h"
#include "Timer.h"
#include "Camera.h"

class App
{
public:
	App();
	// master frame / message loop
	// 初始化游戏逻辑
	int Go();
	~App();

private:
	// 每一帧的游戏逻辑 
	void DoFrame();
public:
	float cameraRotateX = 0.0f;
	float cameraRotateY = 0.0f;

private:
	Window wnd;
	Timer timer;
	Camera cam;
	std::vector<std::unique_ptr<class Melon>> drawables;
	std::vector<std::unique_ptr<class Melon>> melons;
	std::vector<std::unique_ptr<class Box>> boxs;
	std::vector<std::unique_ptr<class Skybox>> skyboxs;
	std::vector<std::unique_ptr<class Sheet>> sheets;


	void updateCameraRotateMouse(float posx, float posy);
	void updateThirdCameraDisY(float delta);
	void updateCameraRotateKeyboard(float posx, float posy);

	static constexpr size_t nDrawables = 180;
};