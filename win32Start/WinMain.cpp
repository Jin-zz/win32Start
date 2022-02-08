#include "Window.h"

int CALLBACK WinMain (
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
){
	Window wnd(800, 300, "Booxx");
	Window wnd2(300, 400, "Booxx2");


	// 消息处理
	MSG msg;
	BOOL gResult;

	while ( (gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		// WM_KEYDOWN 转化为 WM_CHAR
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	if (gResult == -1) {
		return -1;
	}

	return msg.wParam;

}