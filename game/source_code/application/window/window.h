//=========================================================
// window.h
// author:ryuya nakamura
//=========================================================

//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_WINDOW_WINDOW_H__
#define __APP_WINDOW_WINDOW_H__


class Window
{
public:
	
	static void Create(
		HINSTANCE instance, 
		int command_show);
	static LRESULT CALLBACK Procidure(
		HWND window_handle,
		UINT message,
		WPARAM w_param,
		LPARAM l_param);
	static bool Messages();


private:
	Window();
	virtual ~Window();

};


#endif //__APP_WINDOW_WINDOW_H__


//-------------------------------------
// end of file
//-------------------------------------
