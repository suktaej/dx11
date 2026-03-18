#pragma once

#include <Windows.h>
#include <math.h>

#define SAFE_DELETE(p) { if(p) { delete p; p = nullptr; }}
#define DEFINITION_SINGLE(Type) Type* Type::mInst = nullptr;

#define DECLARE_SINGLE(Type) \
private:\
	Type();\
	~Type();\
private:\
	static Type* mInst;\
public:\
	static Type* getInst()\
	{\
		if(!mInst)\
			mInst = new Type;\
		return mInst;\
	}\
	static void destroyInst()\
	{\
		SAFE_DELETE(mInst);\
	}
