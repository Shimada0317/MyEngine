#include "DebugCamera.h"
#include<cassert>

using namespace DirectX;

DebugCamera::DebugCamera(int window_width, int window_height, Input* input):Camera(window_height,window_height)
{
	assert(input);

	this->input = input;
	//ƒXƒP[ƒ‹’²®
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;

}

void DebugCamera::Update()
{
	Camera::Update();

}
