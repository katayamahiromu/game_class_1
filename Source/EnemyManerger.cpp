#include"EnemyManeger.h"
#include"Graphics/Graphics.h"
#include"Collision.h"

//更新処理
void EnemeyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies) {
		enemy->Update(elapsedTime);
	}
    //破棄処理
    //更新処理が終わったあとに破棄リストに積まれたオブジェクトを破棄する
    for (Enemy* enemy : removes) {
        //std::vectorから要素を削除する場合はイテレーターを使わなければならない
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
        if (it != enemies.end()) {
            enemies.erase(it);
        }
        //エネミーの破棄処理
        delete enemy;
    }
    //破棄リストをクリア
    removes.clear();
	//敵同士の衝突判定
	CollisionEnemyVsEnemy();
}

//描画処理
void EnemeyManager::Render(ID3D11DeviceContext* dc, Shader* shader) {
	for (Enemy* enemy : enemies) {
		enemy->Render(dc, shader);
	}
}

//エネミー登録
void EnemeyManager::Register(Enemy* enemy) {
	enemies.emplace_back(enemy);
}

//エネミー削除
void EnemeyManager::Remove(Enemy* enemy){
    //破棄リストに追加
    removes.insert(enemy);
}

//エネミー全削除
void EnemeyManager::Clear() {
	for (Enemy* enemy : enemies) {
		delete enemy;
	}
	enemies.clear();
}

//デバッグプリミティブ描画
void EnemeyManager::DrawDebugPrimitive() {
	for (Enemy* enemy : enemies) {
		enemy->DrawDebugPrimitive();
	}
}

void EnemeyManager::CollisionEnemyVsEnemy() {
    int enemyCount = GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy1 = GetEnemy(i);
        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy2 = GetEnemy(j);
            if (enemy1 == enemy2)continue;

            // 衝突処理
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectCylinderVsCylinder
            (enemy1->GetPosition(),
             enemy1->GetRadius(),
             enemy1->GetHeight(),
             enemy2->GetPosition(),
             enemy2->GetRadius(),
             enemy2->GetHeight(),
             outPosition))
            {
                // 押し出し後の位置調整
                enemy2->SetPositon(outPosition);
            }
        }
    }
}