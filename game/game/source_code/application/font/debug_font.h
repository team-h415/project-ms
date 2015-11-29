//=========================================================
// debug_font.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __DebugFont_H__
#define __DebugFont_H__


//-------------------------------------
// class
//-------------------------------------
class DebugFont
{
public:
	DebugFont();
	virtual ~DebugFont();

	void Update();
	void Draw(
		RECT rect,
		D3DXCOLOR color);
	void Add(
		const char *format, ...);

private:
	LPD3DXFONT font_;
	std::string string_;
};


#endif //__DebugFont_H__


//-------------------------------------
// end of file
//-------------------------------------
