//=========================================================
// common.h
// author:ryuya nakamura
// ���ʗv�f��`
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

// �J���������s���ۂɌĂяo��

// SAFE_DELETE(delete���Ăяo�������|�C���^)
// SAFE_RELEASE(Release()���Ăяo�������|�C���^)
//		��Release()���������N���X�̂݌Ăяo������
// SAFE_DELETE_ARRAY(delete���Ăяo�������|�C���^�z��)
#define SAFE_DELETE(p)			if(p){delete p;p=nullptr;}
#define SAFE_RELEASE(p)			if(p){p->Release();p=nullptr;}
#define SAFE_DELETE_ARRAY(p)	if(p){delete[](p);(p)=nullptr;}



// �E�B���h�E�������Ăяo���}�N��

#define CLASS_NAME "AppClass"
#define WINDOW_NAME "Game"



// �E�B���h�E�T�C�Y

// SCREEN_WIDTH = �E�B���h�E����
// SCREEN_HEIGHT = �E�B���h�E�c��
// ���E�B���h�E�g���l�����Ȃ��l
#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 720.0f
#define SCREEN_ASPECT (SCREEN_WIDTH / SCREEN_HEIGHT)


// �A�T�[�g�}�N��

// ��ɃG���[�𖾎��I�ɒm��f�o�b�O�p�}�N��
// ASSERT_ERROR(�z�肳���G���[���e���L�q)
// ASSERT_WARNING(�z�肳���x�����e���L�q)
#define ASSERT_ERROR(t)		MessageBoxA(nullptr, t, "ERROR", MB_ICONERROR)
#define ASSERT_WARNING(t)	MessageBoxA(nullptr, t, "WARNING", MB_ICONWARNING)


// ������ϊ�
#define TO_STRING(str) #str
#define STRING_CONCATE(str1,str2) str1 ## str2


//-------------------------------------
// end of file
//-------------------------------------




#endif //__COMMON_COMMON_H__

//-------------------------------------
// end of file
//-------------------------------------