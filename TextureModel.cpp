#include "TextureModel.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include<string>
#include<cassert>
#include<fstream>
#include<sstream>

const float TextureModel::radius = 5.0f;				// 底面の半径
const float TextureModel::prizmHeight = 8.0f;			// 柱の高さ
ID3D12Device* TextureModel::dev = nullptr;
UINT TextureModel::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* TextureModel::cmdList = nullptr;
ComPtr<ID3D12RootSignature> TextureModel::rootsignature;
ComPtr<ID3D12PipelineState> TextureModel::pipelinestate;
ComPtr<ID3D12DescriptorHeap> TextureModel::descHeap;
ComPtr<ID3D12Resource> TextureModel::vertBuff;
ComPtr<ID3D12Resource> TextureModel::indexBuff;
ComPtr<ID3D12Resource> TextureModel::texbuff;
CD3DX12_CPU_DESCRIPTOR_HANDLE TextureModel::cpuDescHandleSRV;
CD3DX12_GPU_DESCRIPTOR_HANDLE TextureModel::gpuDescHandleSRV;
XMMATRIX TextureModel::matView{};
XMMATRIX TextureModel::matProjection{};
XMFLOAT3 TextureModel::eye = { 0, 0, -5.0f };
XMFLOAT3 TextureModel::target = { 0, 0, 0 };
XMFLOAT3 TextureModel::up = { 0, 1, 0 };
D3D12_VERTEX_BUFFER_VIEW TextureModel::vbView{};
D3D12_INDEX_BUFFER_VIEW TextureModel::ibView{};
TextureModel::VertexPos TextureModel::vertices[vertexCount];
unsigned short TextureModel::indices[indexCount];
XMMATRIX TextureModel::matBillbord = XMMatrixIdentity();
XMMATRIX TextureModel::matBillbordY = XMMatrixIdentity();

TextureModel* TextureModel::loadFromTex(const wchar_t* pngname)
{
	TextureModel* model = new TextureModel();

	model->InitializeDescriptorHeap();

	model->CreateModel();

	model->LoadTexture(pngname);

	return model;
}


bool TextureModel::LoadTexture(const wchar_t* pngname)
{
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		pngname, WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result)) {
		return result;
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// テクスチャ用バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texbuff));
	if (FAILED(result)) {
		return result;
	}

	// テクスチャバッファにデータ転送
	result = texbuff->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);
	if (FAILED(result)) {
		return result;
	}

	// シェーダリソースビュー作成
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		cpuDescHandleSRV
	);

	return true;
}

bool TextureModel::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

void TextureModel::CreateModel()
{
	HRESULT result = S_FALSE;

	VertexPos verticesPoint[] = {
		{{0.0f,0.0f,0.0f}},
	};

	std::copy(std::begin(verticesPoint), std::end(verticesPoint), vertices);

	unsigned short indicesSquare[] = {
		0,1,2,
		2,1,3,
	};

	std::copy(std::begin(indicesSquare), std::end(indicesSquare), indices);

	// 頂点バッファ生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// インデックスバッファ生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// 頂点バッファへのデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {

		// 全インデックスに対して
		for (int i = 0; i < _countof(indices); i++)
		{
			indexMap[i] = indices[i];	// インデックスをコピー
		}

		indexBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);
}

void TextureModel::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	// 描画コマンド
	cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
}
