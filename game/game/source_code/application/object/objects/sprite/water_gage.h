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
// macro
//-------------------------------------
namespace{
    static const int kMaxGage = 1000;

    static const int kGageDown = 10;

    static const int kGageRegene = 50;
}


//-------------------------------------
// class
//-------------------------------------
class WaterGage : public Sprite2D
{
public:
    WaterGage(
        const OBJECT_PARAMETER_DESC &parameter);
    virtual ~WaterGage();

private:
    void CalculateVertex();
    Vertex2D *vertex_;
    LPDIRECT3DTEXTURE9 texture_;
    int gage_volume_;
};


#endif //__Watergage_H__


//-------------------------------------
// end of file
//-------------------------------------
