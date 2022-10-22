#include "ModelManager.h"

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
    model_.clear();
    model_.shrink_to_fit();
}

void ModelManager::CallModel(const std::string& ObjModelname)
{
    ObjModel* model = nullptr;
    model = ObjModel::CreateFromOBJ(ObjModelname);
    model_.push_back(model);
}


