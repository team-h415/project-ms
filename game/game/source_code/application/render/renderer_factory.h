//=========================================================
// renderer_factory.h
//=========================================================

//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_RENDER_RENDERERFACTORY_H__
#define __APP_RENDER_RENDERERFACTORY_H__

class RendererFactory
{
public:
	static Renderer *Create(
		const HWND &window_handle,
		const std::string &renderer);
};

#endif // __APP_RENDER_RENDERERFACTORY_H__

//-------------------------------------
// end of file
//-------------------------------------
