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
    CallModel("worm");
    CallModel("Gear");
    CallModel("skydome");
    CallModel("Shadow");
    CallModel("Head");
    CallModel("tst2");
    CallModel("BothArm");
}

void ModelManager::CallModel(const std::string& ObjModelname)
{
    ObjModel* model = nullptr;
    model = ObjModel::CreateFromOBJ(ObjModelname,true);
    model_.push_back(model);
}


