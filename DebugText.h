#pragma once

#include"Sprite.h"
#include<Windows.h>
#include<string>

class DebugText
{
public:

	static const int maxChar = 256;
	static const int fontWidth = 9;
	static const int fontHeight = 18;
	static const int fontLineCount = 14;
	static const int buffersize = 256;
public:
	static DebugText* GetInstance();
public:

	void Initialize(UINT texnumber);

	inline void SetPos(float x, float y) {
		posX = x;
		posY = y;
	}


	inline void SetSize(float size) { this->size = size; }

	void Print(const  std::string& text, float x, float y, float size);
	void NPrint(int len, const char* text);
	void Printf(const char* fmt, ...);

	void DrawAll(ID3D12GraphicsCommandList* cmdList);

private:
	DebugText();
	DebugText(const DebugText&) = delete;
	~DebugText();
	DebugText& operator=(const DebugText&) = delete;

private:
	Sprite* spriteDatas[maxChar] = {};
	int spriteIndex = 0;

	float posX = 0.0f;
	float posY = 0.0f;
	float size = 1.0f;

	char buffer[buffersize];
};

