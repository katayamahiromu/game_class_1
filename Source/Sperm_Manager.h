#pragma once
#include<vector>
#include<set>
#include"Sperm.h"

class Sperm_Manager
{
private:
	Sperm_Manager(){}
	~Sperm_Manager() {};
public:
	static Sperm_Manager& Instance()
	{
		static Sperm_Manager instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//ステージ登録
	void Register(Sperm_child* sperm_child);

	//ステージ全削除
	void Clear();

	//エネミー数取得
	int GetSpermCount() const { return static_cast<int>(spermArray.size()); }
	//エネミー取得
	Sperm_child* GetSperm(int index) { return spermArray.at(index); }

	void Remove(Sperm_child*sperm);

	//精子同士の当たり判定
	void SpermVsSperm();
private:
	std::vector<Sperm_child*>spermArray;
	std::vector<Sperm_child*>canAttackSperm;
	std::set<Sperm_child*>remove;
};