#include "Graphics/Graphics.h"
#include "Graphics/Model.h"
#include<stdio.h>
#include"../ResourceManager.h"

// コンストラクタ
Model::Model(const char* filename)
{
	// リソース読み込み
	resource = ReosurceManager::Instance().LoadModelResource(filename);
	// ノード
	const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();

	nodes.resize(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// 行列計算
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform(transform);
}

// 変換行列計算
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ローカル行列算出
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ワールド行列算出
		DirectX::XMMATRIX ParentTransform;
		if (node.parent != nullptr)
		{
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// 計算結果を格納
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

//アニメーション更新処理
void Model::UpdateAnimation(float elapsedTime)
{
	//再生中でないなら処理しない
	if (!IsPlayAnimation())return;
	//再生を止めてたら
	if (anime_stop)return;

	//ブレンド率の計算
	float blendRate = 1.0f;
	if (animationBlendSeconds > animationBlendTime)
	{
		animationBlendTime += elapsedTime;
		blendRate = animationBlendTime / animationBlendSeconds;
		blendRate *= blendRate;
	}

	//指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAimationIndex);

	//アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int KeyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0;keyIndex < KeyCount - 1;++keyIndex)
	{
		//現在の時間がどのキーフレームの間にいるか判定する
		const ModelResource::Keyframe& Keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& Keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSeconds >= Keyframe0.seconds &&
			currentAnimationSeconds < Keyframe1.seconds)
		{
			//再生時間とキーフレームの時間から補完率を算出する
			float rate = (currentAnimationSeconds - Keyframe0.seconds) /(Keyframe1.seconds - Keyframe0.seconds);
			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0;nodeIndex < nodeCount;++nodeIndex)
			{
				if (blendRate < 1.0f)
				{
					//2つのキーフレーム間の補完計算
					const ModelResource::NodeKeyData& key0 = Keyframe0.nodeKeys.at(nodeIndex);
					const ModelResource::NodeKeyData& key1 = Keyframe1.nodeKeys.at(nodeIndex);
					Node& node = nodes[nodeIndex];
					//回転 トランスレートの計算

					DirectX::XMVECTOR Key0_Pos = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR Key1_Pos = DirectX::XMLoadFloat3(&key1.translate);
					DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Key0_Pos, Key1_Pos, blendRate);
					DirectX::XMStoreFloat3(&node.translate, Position);

					DirectX::XMVECTOR Key0_Rot = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR Key1_Rot = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR Rotation = DirectX::XMQuaternionSlerp(Key0_Rot, Key1_Rot, blendRate);
					DirectX::XMStoreFloat4(&node.rotate, Rotation);

				}
				else//通常の計算
				{
					//2つのキーフレーム間の補完計算
					const ModelResource::NodeKeyData& key0 = Keyframe0.nodeKeys.at(nodeIndex);
					const ModelResource::NodeKeyData& key1 = Keyframe1.nodeKeys.at(nodeIndex);
					Node& node = nodes[nodeIndex];
					//回転 トランスレートの計算

					DirectX::XMVECTOR Key0_Pos = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR Key1_Pos = DirectX::XMLoadFloat3(&key1.translate);
					DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Key0_Pos, Key1_Pos, rate);
					DirectX::XMStoreFloat3(&node.translate, Position);

					DirectX::XMVECTOR Key0_Rot = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR Key1_Rot = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR Rotation = DirectX::XMQuaternionSlerp(Key0_Rot, Key1_Rot, rate);
					DirectX::XMStoreFloat4(&node.rotate, Rotation);
				}
			}
			break;
		}

	}

	//時間経過
	currentAnimationSeconds += elapsedTime;

	//再生時間が終端時間を超えたら
	if (currentAnimationSeconds >= animation.secondsLength)
	{
		//ループするか
		if (animationLoopFlag) {
			//再生時間を巻き戻す
			currentAnimationSeconds -= animation.secondsLength;
		}
		else {
			currentAimationIndex = -1;
			animationEndFlag = true;
		}
	}
}
//アニメーション再生
void Model::PlayAnimation(int index, bool loop, float blendSeconds)
{
	currentAimationIndex = index;
	currentAnimationSeconds = 0.0f;
	animationLoopFlag = loop;
	animationEndFlag = false;
	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;
}

//アニメーション再生中か
bool Model::IsPlayAnimation()const
{
	if (currentAimationIndex < 0)return false;
	if (currentAimationIndex >= resource->GetAnimations().size())return false;
	return true;
}

Model::Node* Model::FindNode(const char* name)
{
	//全てのノードを総当たりで名前比較する
	for (Node node : nodes) {
		for (Node *node_childe : node.children) {
			if (strcmp(node_childe->name,name)==0)
			{
				return node_childe;
			}
		}
	}
	//見つからなかった
	return nullptr;
}
