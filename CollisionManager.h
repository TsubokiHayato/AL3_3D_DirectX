#pragma once
#include "Collider.h"
#include "Model.h"
#include <list>
#include <vector>
class GlobalVariables;

class CollisionManager {

public:
	// 初期化
	void Initialize();

	// ワールドトランスフォームの更新
	void UpdateWorldTransform();

	// リセット
	void Reset();

	// コライダー2つ衝突判定と応答
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	// 全ての当たり判定チェック
	void CheckAllCollisions();

	// コライダーの登録
	void AddCollider(Collider* collider);

	void Draw(const ViewProjection& viewProjection);

	
	private:
	// コライダー
	std::list<Collider*> colliders_;

	// デバッグ表示用のモデル
	std::unique_ptr<Model> model_ICO;

	GlobalVariables* globalVariables;

	bool isCheckCollision = false;
};
