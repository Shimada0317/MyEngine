#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
public:
    ///<summary>
    ///�R���X�g���N�^
    ///</summary>
    PostEffect();

    ///<summary>
    ///�R���X�g���N�^
    ///</summary>
    ///<param name="cmdList">�R�}���h���X�g</param>
    void Draw(ID3D12GraphicsCommandList* cmdList);
};

