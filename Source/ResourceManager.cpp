#include"Graphics/Graphics.h"
#include"ResourceManager.h"

//モデルリソース読み込み
std::shared_ptr<ModelResource>ReosurceManager::LoadModelResource(const char* filename)
{
    // モデル検索
    auto it = models.find(filename);
    if (it != models.end()) {
        // モデルが見つかった場合、そのモデルを取得して返す
        if (auto model = it->second.lock()) {
            return model;
        }
    }

    // 新規モデルリソース作成＆読み込み
    std::shared_ptr<ModelResource> model = std::make_shared<ModelResource>();
    model->Load(Graphics::Instance().GetDevice(), filename);
    // マップに登録
    models[filename] = model;
    return model;
}