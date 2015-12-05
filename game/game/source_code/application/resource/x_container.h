//=========================================================
// x_container.h
// author:takeshi iwasawa
//=========================================================

//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __XContainer_H__
#define __XContainer_H__


//================================================================================
// ƒNƒ‰ƒX
//================================================================================
class XContainer
{
	public:
		XContainer(const std::string &path);
		~XContainer(void);

		LPD3DXMESH mesh_;
		LPD3DXBUFFER material_buffer_;
		DWORD material_count_;

		std::string file_path_;

	private:
		void Load(const std::string &path);
};

#endif // _FBX_MANAGER_H_