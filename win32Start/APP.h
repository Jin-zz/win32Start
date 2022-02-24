#pragma once
#include "Window.h"
#include "Timer.h"
#include "Camera.h"

class App
{
public:
	App();
	// master frame / message loop
	// ��ʼ����Ϸ�߼�
	int Go();
	~App();

private:
	// ÿһ֡����Ϸ�߼� 
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