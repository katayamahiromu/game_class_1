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

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//�X�e�[�W�o�^
	void Register(Sperm_child* sperm_child);

	//�X�e�[�W�S�폜
	void Clear();

	//�G�l�~�[���擾
	int GetSpermCount() const { return static_cast<int>(spermArray.size()); }
	//�G�l�~�[�擾
	Sperm_child* GetSperm(int index) { return spermArray.at(index); }

	void Remove(Sperm_child*sperm);

	//���q���m�̓����蔻��
	void SpermVsSperm();
private:
	std::vector<Sperm_child*>spermArray;
	std::vector<Sperm_child*>canAttackSperm;
	std::set<Sperm_child*>remove;
};