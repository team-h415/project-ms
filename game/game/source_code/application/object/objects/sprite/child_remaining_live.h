//=========================================================
// child_remaining_live.h
// author:shohei matsumoto
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __ChildRemainingLive_H__
#define __ChildRemainingLive_H__


//-------------------------------------
// class
//-------------------------------------
class ChildRemainingLive:public Object
{
public:
    ChildRemainingLive(
        const OBJECT_PARAMETER_DESC &parameter);
    virtual ~ChildRemainingLive();
    void Initialize();
    void Update();
    void Draw();
    void SetValue(int value);
    void SetTexture(const std::string &path);
    void SetFigureOffset(float Offset);
    float GetFigureOffset(){ return figure_offset_; }
    void GenerateNumber();
    int GetRemainingLiveValue(){ return remaining_lives_value_; }
    void ChildDeth();
private:
    void UpdateNumber();
    Number **p_number_;
    Sprite2D *p_sprite2d_[2];
    LPDIRECT3DTEXTURE9 texture_;
    float figure_offset_;
    unsigned int figure_;
    int remaining_lives_value_;
};


#endif //__ChildRemainingLive_H__


//-------------------------------------
// end of file
//-------------------------------------
