//=========================================================
// damage_effect.h
// author:shohei matsumoto
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __DamageEffect_H__
#define __DamageEffect_H__


//-------------------------------------
// class
//-------------------------------------
class DamageEffect : public Object
{
private:
    enum FLASH_MODE{
        FLASH_IN  = 0,
        FLASH_OUT ,
    };

public:
    DamageEffect(
        const OBJECT_PARAMETER_DESC &parameter);
    virtual ~DamageEffect();
    void Update();
    void Draw();
    void SetPoint(const int point);
private:
    void CalculateVertex();
    Vertex2D *vertex_;
    LPDIRECT3DTEXTURE9 diffuse_texture_;
    LPDIRECT3DTEXTURE9 alpha_texture_;
    Shader *shader_;
    float damage_value_;
    bool flash_flg_;
    FLASH_MODE flash_mode_;
    float flash_alpha_;

    int hp_;
};


#endif //__DamageEffect_H__


//-------------------------------------
// end of file
//-------------------------------------
