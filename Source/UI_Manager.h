#pragma once
#include"UI.h"
#include<vector>
#include<set>

class UI_Manager
{
public:
	UI_Manager();
	~UI_Manager();


	// ƒVƒ“ƒOƒ‹ƒgƒ“‰»
	static UI_Manager& Instance()
	{
		static UI_Manager instance;
		return instance;
	}

	void Update(float elapsedTime);
	void Render();

	void Register(UI*ui);
	void Clear();
	void Remove(UI*ui);

	int GetEnemyCount()const { return static_cast<int>(UiArray.size()); }
	UI* GetUI(int index) { return UiArray.at(index); }
private:
	std::vector<UI*>UiArray;
	std::set<UI*>remove;
};