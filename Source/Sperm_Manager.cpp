#include"Sperm_Manager.h"
#include"Collision.h"

void Sperm_Manager::Update(float elapsedTime)
{
	for (Sperm_child* sperm : spermArray)
	{
		sperm->Update(elapsedTime);
	}
    for (Sperm_child* sperm : remove) {
        std::vector<Sperm_child*>::iterator it = std::find(spermArray.begin(), spermArray.end(), sperm);
        if (it != spermArray.end()) {
            spermArray.erase(it);
        }
        delete sperm;
    }
    remove.clear();
    SpermVsSperm();
}

void Sperm_Manager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (Sperm_child* sperm : spermArray)
	{
		sperm->Render(dc, shader);
	}
}

void Sperm_Manager::Register(Sperm_child* sperm_child)
{
	spermArray.push_back(sperm_child);
}

void Sperm_Manager::Clear()
{
    for (Sperm_child* sperm : spermArray)
    {
        delete sperm;
    }
    spermArray.clear();
}

void Sperm_Manager::Remove(Sperm_child*sperm)
{
    remove.insert(sperm);
}

void Sperm_Manager::SpermVsSperm()
{
    int sperm_count = GetSpermCount();
    for (int i = 0; i < sperm_count; ++i)
    {
        Sperm_child* sperm1 = GetSperm(i);
        for (int j = 0; j < sperm_count; ++j)
        {
            Sperm_child* sperm2 = GetSperm(j);
            if (sperm1 == sperm2)continue;

            // Õ“Ëˆ—
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectCylinderVsCylinder
            (sperm1->GetPosition(),
                sperm1->GetRadius(),
                sperm1->GetHeight(),
                sperm2->GetPosition(),
                sperm2->GetRadius(),
                sperm2->GetHeight(),
                outPosition))
            {
                // ‰Ÿ‚µo‚µŒã‚ÌˆÊ’u’²®
                sperm2->SetPositon(outPosition);
            }
        }
    }
}