#include "PostEffect.h"
#include"WinApp.h"
#include"Input.h"
#include<d3dx12.h>
#include <cassert>

#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;

//静的メンバ変数の実態
const float PostEffect::clearColor[4] = { 0.25f,0.0f,0.0f,0.0f };//緑っぽい色



PostEffect::PostEffect()
	:Sprite(
		100,//番号
		{ 0,0 },//座標
		{ 500.0f,500.0f },//サイズ
		{ 1,1,1,1 },//色
		{ 0.0f,0.0f },//アンカーポイント
		false,//左右反転
		false)//上下反転
{
}

PostEffect::~PostEffect()
{
}

void PostEffect::CreatePipeline()
{
#pragma region シェーダ読み込み
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectTestVS.hlsl",  // シェーダファイル名
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

		assert(0);
	}


	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectTestPS.hlsl",   // シェーダファイル名
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

		assert(0);
	}


	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//xy座標
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
	gpipeline.DepthStencilState.DepthEnable = false;
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


	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;//描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0〜255指定のRGBA
	gpipeline.SampleDesc.Count = 1;//1ぴくせるにつき1回サンプリング


	//gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//gpipeline.DepthStencilState.DepthEnable = false;


#pragma endregion


	CD3DX12_DESCRIPTOR_RANGE  descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);


	CD3DX12_ROOT_PARAMETER rootparm[2];
	rootparm[0].InitAsConstantBufferView(0);
	rootparm[1].InitAsDescriptorTable(1, &descRangeSRV);

#pragma region シグネチャー

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);


	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparm), rootparm, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

#pragma endregion
	ComPtr<ID3DBlob>rootSigBlob;
	//バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	//ルートシグネチャの生成
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature.Get();
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
	assert(SUCCEEDED(result));

}

void PostEffect::CreateVertBuff()
{
	HRESULT result;

	//頂点バッファ生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//頂点データ
	VertexPosUv vertices[vertNum] = {
		{{-0.5f,-0.5f,0.0f},{0.0f,1.0f}},
		{{-0.5f,+0.5f,0.0f},{0.0f,0.0f}},
		{{+0.5f,-0.5f,0.0f},{1.0f,1.0f}},
		{{+0.5f,+0.5f,0.0f},{1.0f,0.0f}},
	};

	//頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView.StrideInBytes = sizeof(VertexPosUv);
}

void PostEffect::CreateConstBuff()
{
	HRESULT result;

	//定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) +
			0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
}

void PostEffect::CreateTexBuff()
{
	HRESULT result;

	//テクスチャリソースの設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		WinApp::window_width,
		(UINT)WinApp::window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);


	//テックスチャバッファの生成
	for (int i = 0; i < 2; i++) {
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
				D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor),
			IID_PPV_ARGS(&texBuff[i]));
		assert(SUCCEEDED(result));
		{//テクスチャを赤クリア
			//画素数(1280x720=921600ピクセル)
			const UINT pixelCount = WinApp::window_width * WinApp::window_height;
			//画素数1行分のデータサイズ
			const UINT rowPitch = sizeof(UINT) * WinApp::window_width;
			//画像全体のデータサイズ
			const UINT depthPitch = rowPitch * WinApp::window_height;
			//画像イメージ
			UINT* img = new UINT[pixelCount];
			for (int j = 0; j < pixelCount; j++) { img[j] = 0xff0000ff; }

			//テクスチャバッファにデータ転送
			result = texBuff[i]->WriteToSubresource(0, nullptr,
				img, rowPitch, depthPitch);
			assert(SUCCEEDED(result));
			delete[] img;
		}
	}
}

void PostEffect::SRVDesc()
{
	HRESULT result;
	//SRV用のデスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 2;
	//SRV用のデスクリプタヒープを生成
	result = dev->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;


	//デスクリプタヒープにSRV設定
	dev->CreateShaderResourceView(texBuff[0].Get(),
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
		);

}

void PostEffect::RTVDesc()
{
	HRESULT result;
	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 2;

	//RTV用デスクリプタヒープを生成
	result = dev->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));


		//デスクリプタヒープにRTV作成
	for (int i = 0; i < 2; i++) {
		dev->CreateRenderTargetView(texBuff[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV))
		);
	}
}

void PostEffect::DepthBuff()
{
	HRESULT result;

	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WinApp::window_width,
			WinApp::window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

	//深度バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuff));
	assert(SUCCEEDED(result));
}

void PostEffect::DSVDesc()
{
	HRESULT result;
	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV用デスクリプタヒープを作成
	result = dev->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	//デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::Initialize()
{
	HRESULT result;


	CreateVertBuff();

	CreateConstBuff();

	CreateTexBuff();

	SRVDesc();

	RTVDesc();

	DepthBuff();

	DSVDesc();

	CreatePipeline();
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	/*this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);*/

	ConstBufferData* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = this->color;
		constMap->mat = XMMatrixIdentity();
		this->constBuff->Unmap(0, nullptr);
	}
	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//プリミティブ形状の設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };

	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	//cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), this->texNumber, descriptorHandleIncrementSize));

	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	cmdList->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//if (Input::GetInstance()->TriggerKey(DIK_0)) {
	//	//デスクリプタヒープにSRV作成
	//	static int tex = 0;
	//	//テクスチャバッファ番号0と1で切り替え
	//	tex = (tex + 1) % 2;

	//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	//	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//	srvDesc.Texture2D.MipLevels = 1;
	//	dev->CreateShaderResourceView(texBuff[tex].Get(),
	//		&srvDesc,
	//		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	//	);
	//}

	for (int i = 0; i < 2; i++) {
		//リソースバリアを変更
		cmdList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET));
	}
	//レンダーターゲット用ディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH[2];
	for (int i = 0; i < 2; i++) {
		rtvH[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(2, rtvH, false, &dsvH);

	CD3DX12_VIEWPORT viewport[2];
	CD3DX12_RECT scissorRects[2];

	for (int i = 0; i < 2; i++) {
		viewport[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);
		scissorRects[i] = CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height);
	}

	//ビューポートの設定
	cmdList->RSSetViewports(2, viewport);
	//シザリング矩形の設定
	cmdList->RSSetScissorRects(2, scissorRects);
	for (int i = 0; i < 2; i++) {
		//全画面クリア
		cmdList->ClearRenderTargetView(rtvH[i], clearColor, 0, nullptr);
	}
	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0,
		nullptr);
}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	for (int i = 0; i < 2; i++) {
		//リソースバリアを変更
		cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	}
}
