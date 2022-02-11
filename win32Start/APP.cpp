#include "App.h"
#include <sstream>
#include <iomanip>
// 构造函数中创建窗口
App::App()
	:
	wnd(800, 600, "The Donkey Fart Box")
{}
// 处理消息内容
int App::Go()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	// test mouse 
	//while (const auto e = wnd.mouse.Read())
	//{
	//	switch (e->GetType())
	//	{
	//	case Mouse::Event::Type::Move:
	//	{
	//		std::ostringstream oss;
	//		oss << "Mouse moved to: (" << e->GetPosX() << "," << e->GetPosY() << ")\n";
	//		wnd.SetTitle(oss.str());
	//		break;
	//	}
	//	}
	//}

	// test keyboard
	//static std::string title;
	//while (const auto e = wnd.kbd.ReadKey())
	//{
	//	if (e->IsPress() && e->GetCode() == VK_BACK)
	//	{
	//		title.clear();
	//		wnd.SetTitle(title);
	//	}
	//}
	//while (const auto c = wnd.kbd.ReadChar())
	//{
	//	if (*c != 0x8) // don't print backspace
	//	{
	//		title += *c;
	//		wnd.SetTitle(title);
	//	}
	//}

	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().DrawTestTriangle(timer.Peek());
	wnd.Gfx().EndFrame();
}