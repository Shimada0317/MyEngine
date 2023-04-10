#include "Light.h"

using namespace DirectX;

ID3D12Device* Light::dev = nullptr;

void Light::StaticInitialize(ID3D12Device* dev)
{
	//最初期化チェック
	assert(!Light::dev);
	//nullptrチェック
	assert(dev);
	//静的メンバ変数のセット
	Light::dev = dev;

	
}

std::unique_ptr<Light> Light::Create()
{
	Light* instance = new Light();

	instance->Initialize();

	return std::unique_ptr<Light>(instance);
}

void Light::Initialize()
{
	HRESULT result;

	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }

	TransConstBuffer();
}

void Light::TransConstBuffer()
{
	HRESULT result;
	//定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->lightv = -lightdir;
		constMap->lightcolor = lightcolor;
		constBuff->Unmap(0, nullptr);
	}
}

void Light::SetLightDir(const XMVECTOR& lightdir)
{
	this->lightdir = XMVector3Normalize(lightdir);
	dirty = true;
}


void Light::SetLightColor(const XMFLOAT3& lightcolor)
{
	this->lightcolor = lightcolor;
	dirty = true;
}

void Light::Update()
{
	if (dirty) {
		TransConstBuffer();
		dirty = false;
	}

}

void Light::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}
