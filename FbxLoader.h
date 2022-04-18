#pragma once

#include "fbxsdk.h"

#include<string>
#include <d3d12.h>
#include <d3dx12.h>
#include"FbxModel.h"

class FbxLoader
{
private://エイリアス
	//std::を省略
	using string = std::string;
public://定数
	//もでる格納パス
	static const string baseDirectory;
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

/// <summary>
/// 初期化
/// </summary>
/// <param name="device">D3D12デバイス</param>
	void Initialize(ID3D12Device* device);

/// <summary>
/// ファイルからFBXモデル読込
/// </summary>
/// <param name="modelName">モデル名</param>
	void LoadModelFromFile(const string& modelName);
	/// <summary>
	/// ファイルからFBXモデル読込
	/// </summary>
	/// <param name="Model">読み込み先のモデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseNodeRecursive(FbxModel* model, FbxNode* fbxNode,Node* parent=nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="model">読み込み先のモデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMesh(FbxModel* model, FbxNode* fbxNode);

	//頂点読み取り
	void ParseMeshVertices(FbxModel* model, FbxMesh* fbxMesh);
	//面積情報読み取り
	void ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh);
	//マテリアル読み取り
	void ParseMaterial(FbxModel* model, FbxNode* fbxNode);
	//テクスチャ読み取り
	void LoadTexture(FbxModel* model, const  std::string& fullpath);
/// <summary>
/// 後始末
/// </summary>
	void Finalize();

	//ディレクトリを含んだファイルパスからファイル名を抽出
	std::string ExtractFileName(const std::string& path);

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

	//D3D12デバイス
	ID3D12Device* device = nullptr;
	//FBXマネージャー
	FbxManager* fbxManager = nullptr;
	//FBXインポーター
	FbxImporter* fbxImporter = nullptr;

	//テクスチャが無い場合の標準テクスチャファイル名
	static const string defaultTextureName;
};