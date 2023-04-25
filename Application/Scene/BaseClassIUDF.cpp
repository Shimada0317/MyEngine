#include "BaseClassIUDF.h"


void BaseClassIUDF::Initialize()
{
	//基底クラスの初期化
	Framework::Initialize();
	BaseScene* scene_ = new TitleScene(sceneManager_);
	//シーンマネージャーに最初のシーンをセット
	sceneManager_->SetNextScene(scene_);

}

void BaseClassIUDF::Update()
{
	//基底クラスの更新
	Framework::Update();

}

void BaseClassIUDF::Draw()
{
	Framework::Draw();
}

void BaseClassIUDF::Finalize()
{
	//基底クラスの終了
	Framework::Finalize();

}
