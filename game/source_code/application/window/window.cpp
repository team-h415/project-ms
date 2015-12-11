//=========================================================
// window.cpp
// author:ryuya nakamura
//=========================================================

//-------------------------------------
// include 
//-------------------------------------
#include "../../common/common.h"
#include "window_holder.h"
#include "window.h"


//-------------------------------------
// Window()
//-------------------------------------
Window::Window()
{

}

//-------------------------------------
// ~Window()
//-------------------------------------
Window::~Window()
{

}


//-------------------------------------
// Create()
//-------------------------------------
void Window::Create(
	HINSTANCE instance,
	int command_show)
{
	WindowHolder::instance_ = instance;
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		Procidure,
		0,
		0,
		WindowHolder::instance_,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 0),
		NULL,
		CLASS_NAME,
		NULL,
	};

	RegisterClassEx(&wcex);

	WindowHolder::handle_ = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		static_cast<UINT>(SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		static_cast<UINT>(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL,
		NULL,
		WindowHolder::instance_,
		NULL);

	ShowWindow(WindowHolder::handle_, command_show);
	UpdateWindow(WindowHolder::handle_);
}

//-------------------------------------
// Procidure()
//-------------------------------------
LRESULT CALLBACK Window::Procidure(
	HWND window_handle,
	UINT message,
	WPARAM w_param,
	LPARAM l_param)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (w_param)
		{
		case VK_ESCAPE:
			DestroyWindow(window_handle);
			break;
		}
		break;
	}
	return DefWindowProc(
		window_handle,
		message,
		w_param,
		l_param);
}


//-------------------------------------
// Messages()
//-------------------------------------
bool Window::Messages()
{
	MSG message;
	if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) == 0) 
		return false;

	if (message.message == WM_QUIT) return true;
	
	TranslateMessage(&message);
	DispatchMessage(&message);
	return false;
}


//-------------------------------------
// end of file
//-------------------------------------
