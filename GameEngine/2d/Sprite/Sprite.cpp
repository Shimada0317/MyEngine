	#include "Sprite.h"
#include <cassert>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "Sprite.h"


#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

UINT Sprite::descriptorHandleIncrementSize;
ID3D12Device* Sprite::dev = nullptr;
ComPtr<ID3D12PipelineState> Sprite::pipelinestate;
ComPtr<ID3D12RootSignature> Sprite::rootsignature;
ComPtr<ID3D12Resource> Sprite::texBuff[spriteSRVCount];
ComPtr<ID3D12DescriptorHeap> Sprite::descHeap;
ID3D12GraphicsCommandList* Sprite::cmdList = nullptr;
XMMATRIX Sprite::matProjection;

Sprite::~Sprite()
{

}


bool Sprite::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{

	assert(device);

	Sprite::dev = device;
	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
#pragma region シェーダ読み込み
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/SpriteVS.hlsl",  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		
		return false;
	}


	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/SpritePS.hlsl",   // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		
		return false;
	}


	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//xyz座標
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//uv座標
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

#pragma endregion

#pragma region パイプライン

	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定

	//ラスタライザ
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//デプステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//常に上書き

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//標準設定
	blenddesc.BlendEnable = true;//ブレンドを有効にする
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//テストの値を0%使う


	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	////加算
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う

	////減算
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う

	////反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0fデストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//使わない

	//半透明
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースのα値
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0fソースの値

	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;//描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0〜255指定のRGBA
	gpipeline.SampleDesc.Count = 1;//1ぴくせるにつき1回サンプリング

	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	gpipeline.DepthStencilState.DepthEnable = false;


#pragma endregion


	CD3DX12_DESCRIPTOR_RANGE descRangeCBV, descRangeSRV;
	descRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootparm[2];
	rootparm[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparm[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

#pragma region シグネチャー

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparm), rootparm, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

#pragma endregion
	ComPtr<ID3DBlob>rootSigBlob;
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));


	// パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature.Get();
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	matProjection =XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width,
		(float)window_height, 0.0f,
		0.0f, 1.0f
	);

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = spriteSRVCount;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	return true;
}

bool Sprite::LoadTexture(UINT texnumber, const wchar_t* filename)
{
	assert(dev);

	HRESULT result;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename, 
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	if (FAILED(result)) {
		assert(0);
		return false;
	}

	const Image* img = scratchImg.GetImage(0, 0, 0);

	//リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);
	
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff[texnumber])
	);
	if (FAILED(result)) {
		assert(0);
		return false;
	}
	//テクスチャバッファにデータ転送
	result = texBuff[texnumber]->WriteToSubresource(
		0,
		nullptr,//全領域へコピー
		img->pixels,//元データアドレス
		(UINT)img->rowPitch,//1ラインサイズ
		(UINT)img->slicePitch//全サイズ
	);

	if (FAILED(result)) {
		assert(0);
		return false;
	}

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	D3D12_RESOURCE_DESC resDesc = texBuff[texnumber]->GetDesc();
	srvDesc.Format = resDesc.Format;//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	
	//ヒープの2番目にシェーダリソースビュー作成
	dev->CreateShaderResourceView(texBuff[texnumber].Get(),//ビューと関連付けるバッファ
		&srvDesc,//テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(),texnumber,descriptorHandleIncrementSize)
	);
	return true;
}

void Sprite::PreDraw(ID3D12GraphicsCommandList* cmdList)
{

	assert(Sprite::cmdList == nullptr);

	Sprite::cmdList = cmdList;

	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::PostDraw()
{
	Sprite::cmdList = nullptr;
}

Sprite* Sprite::SpriteCreate(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	// 仮サイズ
	XMFLOAT2 size = { 10.0f, 10.0f };

	if (texBuff[texNumber])
	{
		// テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = texBuff[texNumber]->GetDesc();
		// スプライトのサイズをテクスチャのサイズに設定
		size = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// Spriteのインスタンスを生成
	Sprite* sprite = new Sprite(texNumber, position, size, color, anchorpoint, isFlipX, isFlipY);
	if (sprite == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!sprite->InitializeSprite()) {
		delete sprite;
		assert(0);
		return nullptr;
	}

	return sprite;
}




Sprite::Sprite(UINT texNumber, XMFLOAT2 position,XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	this->position = position;
	this->size = size;
	this->anchorpoint = anchorpoint;
	this->matWorld = XMMatrixIdentity();
	this->color = color;
	this->texNumber = texNumber;
	this->FlipX = isFlipX;
	this->FlipY = isFlipY;
	this->texSize = size;
}

bool Sprite::InitializeSprite()
{
	// nullptrチェック
	assert(dev);

	HRESULT result = S_FALSE;

	// 頂点バッファ生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// 頂点バッファへのデータ転送
	Trans();

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView.StrideInBytes = sizeof(VertexPosUv);

	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->mat = matProjection;
		constBuff->Unmap(0, nullptr);
	}

	return true;
}

void Sprite::SetRotation(float rotation)
{
	this->rotation = rotation;

	Trans();
}

void Sprite::SetPosition(XMFLOAT2 position)
{
	this->position = position;

	Trans();
}

void Sprite::SetSize(XMFLOAT2 size)
{
	this->size = size;

	Trans();
}

void Sprite::SetFlipX(bool FlipX) 
{
	this->FlipX = FlipX;

	Trans();
}

void Sprite::SetFlipY(bool FlipY)
{
	this->FlipY = FlipY;

	Trans();
}

void Sprite::SetTexRect(XMFLOAT2 texBase, XMFLOAT2 texSize)
{
	this->texBase = texBase;
	this->texSize = texSize;

	Trans();
}

void Sprite::SetColor(XMFLOAT4 color)
{
	GetColor = color;
}

void Sprite::SetAnchor(XMFLOAT2 anchorpoint)
{
	this->anchorpoint = anchorpoint;

	Trans();
}


void Sprite::Draw()
{
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	ConstBufferData* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) 
	{
		constMap->color = GetColor;
		constMap->mat = this->matWorld * matProjection;
		this->constBuff->Unmap(0, nullptr);
	}

	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };

	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), this->texNumber, descriptorHandleIncrementSize));

	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::Trans()
{
	HRESULT result = S_FALSE;

	enum {LB,LT,RB,RT};

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;
	if (FlipX) {
		left = -left;
		right = -right;
	}

	if (FlipY) {
		top = -top;
		bottom = -bottom;
	}

	VertexPosUv vertices[vertNum];

	vertices[LB].pos = { left,	bottom,	0.0f };
	vertices[LT].pos = { left,	top,	0.0f };
	vertices[RB].pos = { right,	bottom,	0.0f };
	vertices[RT].pos = { right,	top,	0.0f };
	// テクスチャ情報取得
	if (texBuff[texNumber])
	{
		D3D12_RESOURCE_DESC resDesc = texBuff[texNumber]->GetDesc();

		float tex_left = texBase.x / resDesc.Width;
		float tex_right = (texBase.x + texSize.x) / resDesc.Width;
		float tex_top = texBase.y / resDesc.Height;
		float tex_bottom = (texBase.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,	tex_bottom }; // 左下
		vertices[LT].uv = { tex_left,	tex_top }; // 左上
		vertices[RB].uv = { tex_right,	tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,	tex_top }; // 右上
	}

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}