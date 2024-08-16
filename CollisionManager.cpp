#include "CollisionManager.h"

void CollisionManager::Reset() { colliders_.clear(); }

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	Vector3 posA = colliderA->GetCenterPos();
	Vector3 posB = colliderB->GetCenterPos();

	//座標の差分ベクトル
	Vector3 subtract = posB - posA;
	//座標AとBの距離を求める
	float distance = Length(subtract);

	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) {
		
		colliderA->OnCollision();
		colliderB->OnCollision();
	}



}


void CollisionManager::AddCollider(Collider* collider) {

// コライダーをリストに追加する
	colliders_.push_back(collider);
}

void CollisionManager::CheckAllCollisions() {

	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}