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
}

void ModelManager::Finalize()
{
   
}

void ModelManager::CallModel(const std::string& ObjModelname)
{
    ObjModel* model = nullptr;
    model = ObjModel::CreateFromOBJ(ObjModelname);
    model_.push_back(model);
}


