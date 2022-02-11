#pragma once
#include "Window.h"
#include "ChiliTimer.h"

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
private:
	Window wnd;
	ChiliTimer timer;
	std::vector<std::unique_ptr<class Box>> boxes;
};