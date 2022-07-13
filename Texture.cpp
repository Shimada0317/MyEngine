#include "Texture.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
const float Texture::radius = 5.0f;				// 底面の半径
const float Texture::prizmHeight = 8.0f;			// 柱の高さ
ID3D12Device* Texture::device = nullptr;
UINT Texture::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* Texture::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Texture::rootsignature;
ComPtr<ID3D12PipelineState> Texture::pipelinestate;
ComPtr<ID3D12DescriptorHeap> Texture::descHeap;
ComPtr<ID3D12Resource> Texture::vertBuff;
ComPtr<ID3D12Resource> Texture::indexBuff;
ComPtr<ID3D12Resource> Texture::texbuff;
CD3DX12_CPU_DESCRIPTOR_HANDLE Texture::cpuDescHandleSRV;
CD3DX12_GPU_DESCRIPTOR_HANDLE Texture::gpuDescHandleSRV;
XMMATRIX Texture::matView{};
XMMATRIX Texture::matProjection{};
XMFLOAT3 Texture::eye = { 0, 0, -5.0f };
XMFLOAT3 Texture::target = { 0, 0, 0 };
XMFLOAT3 Texture::up = { 0, 1, 0 };
D3D12_VERTEX_BUFFER_VIEW Texture::vbView{};
D3D12_INDEX_BUFFER_VIEW Texture::ibView{};
Texture::VertexPos Texture::vertices[vertexCount];
unsigned short Texture::indices[indexCount];
XMMATRIX Texture::matBillbord = XMMatrixIdentity();
XMMATRIX Texture::matBillbordY = XMMatrixIdentity();

Camera* Texture::camera = nullptr;

bool Texture::StaticInitialize(ID3D12Device* device, int window_width, int window_height, ID3D12GraphicsCommandList* cmdList, Camera* camera)
{
	// nullptrチェック
	assert(device);

	Texture::device = device;
	Texture::cmdList = cmdList;
	Texture::camera = camera;
	TextureModel::setDev(device);


	// カメラ初期化
	InitializeCamera(window_width, window_height);

	// パイプライン初期化
	InitializeGraphicsPipeline();

	return true;
}

void Texture::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void Texture::PostDraw()
{

}

Texture* Texture::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	Texture* texture = new Texture();
	if (texture == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!texture->Initialize()) {
		delete texture;
		assert(0);
		return nullptr;
	}

	return texture;
}

void Texture::SetEye(XMFLOAT3 eye)
{
	Texture::eye = eye;

	UpdateViewMatrix();
}

void Texture::SetTarget(XMFLOAT3 target)
{
	Texture::target = target;

	UpdateViewMatrix();
}

void Texture::CameraMoveVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Texture::CameraMoveEyeVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	SetEye(eye_moved);
}

void Texture::InitializeCamera(int window_width, int window_height)
{
	//UpdateViewMatrix();

	// ビュー行列の生成
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));

	// 平行投影による射影行列の生成
	//constMap->mat = XMMatrixOrthographicOffCenterLH(
	//	0, window_width,
	//	window_height, 0,
	//	0, 1);
	// 透視投影による射影行列の生成
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);
}

bool Texture::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob;
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/TextureVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
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
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/TexturePS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
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
		exit(1);
	}
	//ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/TextureGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
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
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_ONE;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	//gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	if (FAILED(result)) {
		return result;
	}

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	if (FAILED(result)) {
		return result;
	}

	return true;
}

void Texture::UpdateViewMatrix()
{
	//XMVECTOR eyePosition = XMLoadFloat3(&eye);

	//XMVECTOR targetPosition = XMLoadFloat3(&target);

	//XMVECTOR upVector = XMLoadFloat3(&up);

	//XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	//assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	//assert(!XMVector3IsInfinite(cameraAxisZ));
	//assert(!XMVector3Equal(upVector, XMVectorZero()));
	//assert(!XMVector3IsInfinite(upVector));

	//cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//XMVECTOR cameraAxisX;

	//cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);

	//cameraAxisX = XMVector3Normalize(cameraAxisX);

	//XMVECTOR cameraAxisY;

	//cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	//XMMATRIX matCameraRot;

	//matCameraRot.r[0] = cameraAxisX;
	//matCameraRot.r[1] = cameraAxisY;
	//matCameraRot.r[2] = cameraAxisZ;
	//matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	//matView = XMMatrixTranspose(matCameraRot);

	//XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);

	//XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	//XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	//XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);

	//XMVECTOR transition = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//matView.r[3] = transition;

	//matBillbord.r[0] = cameraAxisX;
	//matBillbord.r[1] = cameraAxisY;
	//matBillbord.r[2] = cameraAxisZ;
	//matBillbord.r[3] = XMVectorSet(0, 0, 0, 1);

	//XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	//ybillCameraAxisX = cameraAxisX;

	//ybillCameraAxisY = XMVector3Normalize(upVector);

	//ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	//matBillbordY.r[0] = ybillCameraAxisX;
	//matBillbordY.r[1] = ybillCameraAxisY;
	//matBillbordY.r[2] = ybillCameraAxisZ;
	//matBillbordY.r[3] = XMVectorSet(0, 0, 0, 1);

	// ビュー行列の更新
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

bool Texture::Initialize()
{
	// nullptrチェック
	assert(device);

	HRESULT result;
	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	return true;
}

void Texture::Update()
{
	assert(camera);

	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	// ワールド行列の合成
	matWorld = XMMatrixIdentity(); // 変形をリセット
	//matWorld *= matBillbord;
	//matWorld *= matBillbordY;
	matWorld *= matScale; // ワールド行列にスケーリングを反映
	matWorld *= matRot; // ワールド行列に回転を反映
	matWorld *= matTrans; // ワールド行列に平行移動を反映

	// 親オブジェクトがあれば
	if (parent != nullptr) {
		// 親オブジェクトのワールド行列を掛ける
		matWorld *= parent->matWorld;
	}

	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	const XMFLOAT3& cameraPos = camera->GetEye();

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->world = matWorld;
	constMap->viewproj = matViewProjection;
	constMap->camerapos = cameraPos;
	constMap->mat = matView * matProjection*matWorld;
	constBuff->Unmap(0, nullptr);
}

void Texture::Draw()
{
	// nullptrチェック
	assert(device);
	assert(Texture::cmdList);

	if (model == nullptr)return;

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	model->Draw(cmdList);
}
