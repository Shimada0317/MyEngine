#include "NewPostEffect.h"
#include<d3dx12.h>

using namespace DirectX;

NewPostEffect::NewPostEffect()
	:Sprite(
		100,
		{0,0},
		{500.0f,500.0f},
		{1,1,1,1},
		{0.0f,0.0f},
		false,
		false
	)
{
}

void NewPostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	ConstBufferData* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = this->color;
		constMap->mat = this->matWorld * matProjection;
		this->constBuff->Unmap(0, nullptr);
	}

	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());

	cmdList->SetGraphicsRootSignature(rootsignature.Get());

	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };

	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), this->texNumber, descriptorHandleIncrementSize));

	

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	cmdList->DrawInstanced(4, 1, 0, 0);


}


