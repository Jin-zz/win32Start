#pragma once
#include "Window.h"
#include "ChiliTimer.h"

class App
{
public:
	App();
	// master frame / message loop
	// 初始化游戏逻辑
	int Go();
private:
	// 每一帧的游戏逻辑 
	void DoFrame();
private:
	Window wnd;
	ChiliTimer timer;
};