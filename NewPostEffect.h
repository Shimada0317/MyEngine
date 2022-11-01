#pragma once
#include "sprite.h"
class NewPostEffect :
    public Sprite
{
public:

    NewPostEffect();


    void Draw(ID3D12GraphicsCommandList* cmdList);

};

