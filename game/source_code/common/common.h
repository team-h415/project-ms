//=========================================================
// common.h
// author:ryuya nakamura
// 共通要素定義
//=========================================================

//=========================================================
// include guard
//=========================================================
#pragma once
#ifndef __COMMON_COMMON_H__
#define __COMMON_COMMON_H__


//-------------------------------------
// include 
//-------------------------------------
#include <crtdbg.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <process.h>
#include <fbxsdk.h>
#include <stdint.h>

#include <algorithm>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <list>


//-------------------------------------
// library link
//-------------------------------------
#pragma comment(lib, "winmm.lib")


//-------------------------------------
// define 
//-------------------------------------

// 開放処理を行う際に呼び出す

// SAFE_DELETE(deleteを呼び出したいポインタ)
// SAFE_RELEASE(Release()を呼び出したいポインタ)
//		※Release()を持ったクラスのみ呼び出すこと
// SAFE_DELETE_ARRAY(deleteを呼び出したいポインタ配列)
#define SAFE_DELETE(p)			if(p){delete p;p=nullptr;}
#define SAFE_RELEASE(p)			if(p){p->Release();p=nullptr;}
#define SAFE_DELETE_ARRAY(p)	if(p){delete[](p);(p)=nullptr;}



// ウィンドウ生成時呼び出すマクロ

#define CLASS_NAME "AppClass"
#define WINDOW_NAME "Game"



// ウィンドウサイズ

// SCREEN_WIDTH = ウィンドウ横幅
// SCREEN_HEIGHT = ウィンドウ縦幅
// ※ウィンドウ枠を考慮しない値
#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 720.0f
#define SCREEN_ASPECT (SCREEN_WIDTH / SCREEN_HEIGHT)


// アサートマクロ

// 主にエラーを明示的に知るデバッグ用マクロ
// ASSERT_ERROR(想定されるエラー内容を記述)
// ASSERT_WARNING(想定される警告内容を記述)
#define ASSERT_ERROR(t)		MessageBoxA(nullptr, t, "ERROR", MB_ICONERROR)
#define ASSERT_WARNING(t)	MessageBoxA(nullptr, t, "WARNING", MB_ICONWARNING)


// 文字列変換
#define TO_STRING(str) #str
#define STRING_CONCATE(str1,str2) str1 ## str2


//-------------------------------------
// end of file
//-------------------------------------




#endif //__COMMON_COMMON_H__

//-------------------------------------
// end of file
//-------------------------------------