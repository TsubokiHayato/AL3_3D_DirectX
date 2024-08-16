#pragma once
#include "Collider.h"
#include <vector>
#include <list>

class CollisionManager {

public:
	//リセット
	void Reset();

	//コライダー2つ衝突判定と応答
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	//全ての当たり判定チェック
	void CheckAllCollisions();

	// コライダーの登録
	void AddCollider(Collider* collider);

private:
	//コライダー
	std::list<Collider*> colliders_;

};
