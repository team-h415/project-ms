//=========================================================
// water_gage.h
// author:shohei matsumoto
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __WaterGage_H__
#define __WaterGage_H__


//-------------------------------------
// class
//-------------------------------------
class WaterGage : public Object
{
public:
    WaterGage(
        const OBJECT_PARAMETER_DESC &parameter);
    virtual ~WaterGage();
    void Update();
    void Draw();
    // ÉQÅ[ÉWÇÃïœçXäÑçáÇì¸ÇÍÇÈ
    void SetChangeValue(const float value){ gauge_value_ = value; }
    void SetTexture(
        const std::string &path);
private:
    void CalculateVertex();
    Vertex2D *vertex_;
    LPDIRECT3DTEXTURE9 diffuse_texture_;
    LPDIRECT3DTEXTURE9 alpha_texture_;
    Shader *shader_;
//    LPDIRECT3DTEXTURE9 texture_;
    float gauge_value_;
};


#endif //__Watergage_H__


//-------------------------------------
// end of file
//-------------------------------------
