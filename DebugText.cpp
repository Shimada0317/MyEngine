#include "DebugText.h"

DebugText::DebugText() {
}

DebugText::~DebugText(){
	for (int i = 0; i < _countof(spriteDatas); i++) {
		delete spriteDatas[i];
	}
}

DebugText* DebugText::GetInstance() {
	static DebugText instance;
	return &instance;
}

void DebugText::Initialize(UINT texnumber) {
	for (int i = 0; i < _countof(spriteDatas); i++) {
		spriteDatas[i] = Sprite::SpriteCreate(texnumber, { 0,0 });
	}
}

void DebugText::Print(const  std::string& text, float x, float y, float size = 1.0f) {
	SetPos(x, y);
	SetSize(size);

	NPrint((int)text.size(), text.c_str());
}

void DebugText::NPrint(int len, const char* text) {
	for (int i = 0; i < len; i++) {
		if (spriteIndex >= maxChar) {
			break;
		}

		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		spriteDatas[spriteIndex]->SetPosition({ this->posX + fontWidth * this->size * i, this->posY });
		spriteDatas[spriteIndex]->SetTexRect({ (float)fontIndexX * fontWidth,(float)fontIndexY * fontHeight }, { (float)fontWidth,(float)fontHeight });
		spriteDatas[spriteIndex]->SetSize({ fontWidth * this->size,fontHeight * this->size });

		spriteIndex++;

	}
}

void DebugText::Printf(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	int  w = vsnprintf(buffer, buffersize - 1, fmt, args);
	NPrint(w, buffer);
	va_end(args);
}

void DebugText::DrawAll(ID3D12GraphicsCommandList* cmdList)
{
	for (int i = 0; i < spriteIndex; i++) {
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}