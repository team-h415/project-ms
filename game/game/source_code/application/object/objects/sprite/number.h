//=========================================================
// Number.h
// author:shohei matsutmoto
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Number_H__
#define __Number_H__


//-------------------------------------
// class
//-------------------------------------
class Number : public Object
{
public:
    Number(
        OBJECT_PARAMETER_DESC &parameter , const int value);
    virtual ~Number();
    void Update();
    void Draw();
    void SetTexture(
        const std::string &path);
	void SetTexture(
		LPDIRECT3DTEXTURE9 texture);
    void SetValue(int value){ value_ = value; }
    int GetValue(void){ return value_; }
private:
    void CalculateVertex();
    Vertex2D *vertex_;
    LPDIRECT3DTEXTURE9 texture_;
    int value_;
};


#endif //__Number_H__


//-------------------------------------
// end of file
//-------------------------------------
