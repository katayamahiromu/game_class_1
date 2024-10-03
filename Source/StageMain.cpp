#include"StageMain.h"

StageMain::StageMain() {
	model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");

	//DirectX::XMVECTOR VolumeMin = DirectX::XMVectorReplicate(FLT_MAX);
	//DirectX::XMVECTOR VolumeMax = DirectX::XMVectorReplicate(-FLT_MAX);

	//// 頂点データをワールド空間変換し、三角形データを作成
	//for (const Model::Mesh& mesh : stage->GetMeshes())
	//{
	//	DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&mesh.node->worldTransform);
	//	for (size_t i = 0; i < mesh.indices.size(); i += 3)
	//	{
	//		// 頂点データをワールド空間変換
	//		uint32_t a = mesh.indices.at(i + 0);
	//		uint32_t b = mesh.indices.at(i + 1);
	//		uint32_t c = mesh.indices.at(i + 2);
	//		DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&mesh.vertices.at(a).position);
	//		DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&mesh.vertices.at(b).position);
	//		DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&mesh.vertices.at(c).position);
	//		A = DirectX::XMVector3Transform(A, WorldTransform);
	//		B = DirectX::XMVector3Transform(B, WorldTransform);
	//		C = DirectX::XMVector3Transform(C, WorldTransform);

	//		// 法線ベクトルを算出
	//		DirectX::XMVECTOR N = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(B, A), DirectX::XMVectorSubtract(C, A));
	//		if (DirectX::XMVector3Equal(N, DirectX::XMVectorZero()))
	//		{
	//			// 面を構成できない場合は除外
	//			continue;
	//		}
	//		N = DirectX::XMVector3Normalize(N);

	//		// 三角形データを格納
	//		CollisionMesh::Triangle& triangle = collisionMesh.triangles.emplace_back();
	//		DirectX::XMStoreFloat3(&triangle.positions[0], A);
	//		DirectX::XMStoreFloat3(&triangle.positions[1], B);
	//		DirectX::XMStoreFloat3(&triangle.positions[2], C);
	//		DirectX::XMStoreFloat3(&triangle.normal, N);

	//		// モデル全体のAABBを計測
	//		VolumeMin = DirectX::XMVectorMin(VolumeMin, A);
	//		VolumeMin = DirectX::XMVectorMin(VolumeMin, B);
	//		VolumeMin = DirectX::XMVectorMin(VolumeMin, C);
	//		VolumeMax = DirectX::XMVectorMax(VolumeMax, A);
	//		VolumeMax = DirectX::XMVectorMax(VolumeMax, B);
	//		VolumeMax = DirectX::XMVectorMax(VolumeMax, C);
	//	}
	//}

	//DirectX::XMFLOAT3 volumeMin, volumeMax;
	//DirectX::XMStoreFloat3(&volumeMin, VolumeMin);
	//DirectX::XMStoreFloat3(&volumeMax, VolumeMax);
	//// TODO①:モデル全体のAABBからXZ平面に指定のサイズで分割されたコリジョンエリアを作成する
	//{
	//	const int cellSize = 4;
	//	const float invCellSize = 1.0f / cellSize;

	//	int gridMinX = static_cast<int>(floor(volumeMin.x * invCellSize));
	//	int gridMinZ = static_cast<int>(floor(volumeMin.z * invCellSize));
	//	int gridMaxX = static_cast<int>(ceil(volumeMax.x * invCellSize));
	//	int gridMaxZ = static_cast<int>(ceil(volumeMax.z * invCellSize));

	//	collisionMesh.areas.resize((gridMaxX - gridMinX + 1) * (gridMaxZ - gridMinZ + 1));

	//	auto getIndex = [&](int x, int z) {
	//		return (z - gridMinZ) * (gridMaxX - gridMinX + 1) + (x - gridMinX);
	//	};

	//	for (size_t i = 0; i < collisionMesh.triangles.size(); ++i) {
	//		const CollisionMesh::Triangle& triangle = collisionMesh.triangles[i];
	//		DirectX::XMFLOAT3 Min = triangle.positions[0];
	//		DirectX::XMFLOAT3 Max = triangle.positions[0];

	//		for (int j = 1; j < 3; ++j) {
	//			Min.x = (((Min.x) > (triangle.positions[j].x)) ? (Min.x) : (triangle.positions[j].x));
	//			Min.z = (((Min.z) > (triangle.positions[j].z)) ? (Min.z) : (triangle.positions[j].z));
	//			Max.x = (((Max.x) > (triangle.positions[j].x)) ? (Max.x) : (triangle.positions[j].x));
	//			Max.z = (((Max.z) > (triangle.positions[j].z)) ? (Max.z) : (triangle.positions[j].z));
	//		}

	//		int startX = static_cast<int>(floor(Min.x * invCellSize));
	//		int startZ = static_cast<int>(floor(Min.z * invCellSize));
	//		int endX = static_cast<int>(ceil(Max.x * invCellSize));
	//		int endZ = static_cast<int>(ceil(Max.z * invCellSize));

	//		for (int z = startZ; z <= endZ; ++z) {
	//			for (int x = startX; x <= endX; ++x) {
	//				collisionMesh.areas[getIndex(x, z)].triangleIndices.push_back(i);
	//			}
	//		}
	//	}

	//	for (int z = gridMinZ; z <= gridMaxZ; ++z) {
	//		for (int x = gridMinX; x <= gridMaxX; ++x) {
	//			int index = getIndex(x, z);
	//			collisionMesh.areas[index].boundingBox.Center = {
	//				(x + 0.5f) * cellSize, 0.0f, (z + 0.5f) * cellSize
	//			};
	//			collisionMesh.areas[index].boundingBox.Extents = {
	//				cellSize * 0.5f, volumeMax.y - volumeMin.y, cellSize * 0.5f
	//			};
	//		}
	//	}
	//}
}

StageMain::~StageMain() {
	delete model;
}

void StageMain::Update(float elapsedTime) {

}

void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader) {
	shader->Draw(dc, model);
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::InterserctRayVsModel(start, end, model,hit);
}