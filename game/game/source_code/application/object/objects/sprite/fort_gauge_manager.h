//=========================================================
// fort_gauge_manager.h
// author:shohei matsumoto
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __FortGaugeManager_H__
#define __FortGaugeManager_H__


//-------------------------------------
// include
//-------------------------------------
#include "fort_gage.h"


//-------------------------------------
// class
//-------------------------------------
class FortGaugeManager :public Object
{
public:
    FortGaugeManager(
        const OBJECT_PARAMETER_DESC &parameter);
    virtual ~FortGaugeManager();
    void Update();
    void Draw();
    void SetTexture(
        const std::string &path);
    void SetFortLife(const int id , const float life);
    FORT_STATE GetFortState(const int id){ fort_gauge_[id]->GetFortState(); }
private:
    FortGage *fort_gauge_[3];
    LPDIRECT3DTEXTURE9 texture_;
};


#endif //__FortGaugeManager_H__


//-------------------------------------
// end of file
//-------------------------------------