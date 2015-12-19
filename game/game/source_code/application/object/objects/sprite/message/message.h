//=========================================================
// message.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Message_H__
#define __Message_H__


//-------------------------------------
// class
//-------------------------------------
class Message : public Object
{
public:
	Message(
		const OBJECT_PARAMETER_DESC &parameter);
	~Message();
	void Update();
	void Draw();
	void SetTexture(
		const std::string &path);
	void Play();
	void Move(float value){
		dest_position_ += value;
	}

private:
	void CalculateVertex();
	Vertex2D *vertex_;
	LPDIRECT3DTEXTURE9 frame_texture_;
	LPDIRECT3DTEXTURE9 message_texture_;
	int frame_count_;
	float dest_position_;
	bool play_;
};


#endif //__Message_H__


//-------------------------------------
// end of file
//-------------------------------------
