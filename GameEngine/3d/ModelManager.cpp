#include "ModelManager.h"

ModelManager::~ModelManager()
{
    for (ObjModel* modelMa : model_) {
        delete modelMa;
    }
    model_.clear();
}

ModelManager* ModelManager::GetInstance()
{
    static ModelManager instance;
    return &instance;
}

void ModelManager::Initialize()
{
    CallModel("worm",true);
    CallModel("Gear",true);
    CallModel("Shadow",true);
    CallModel("Head",true);
    CallModel("tst2",true);
    CallModel("BothArm",true);
    CallModel("skydome");
    CallModel("bills");
    CallModel("bil",true);
    CallModel("World");
}

void ModelManager::CallModel(const std::string& ObjModelname, bool Smoothing)
{
    ObjModel* model = nullptr;
    model = ObjModel::CreateFromOBJ(ObjModelname,Smoothing);
    model_.push_back(model);
}


