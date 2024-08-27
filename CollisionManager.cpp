#include "CollisionManager.h"
#include"cassert"

#include "GlobalVariables.h"
#include <iostream>
void CollisionManager::Initialize() {


    globalVariables = GlobalVariables::GetInstance();
    const char* groupName = "CollisionManager";
    globalVariables->CreateGroup(groupName);

    // グローバル変数を追加または取得
    globalVariables->AddItem(groupName, "isCheckCollision", isCheckCollision);

    // 最新の値を取得
    isCheckCollision = globalVariables->GetBoolValue(groupName, "isCheckCollision");

	
	

    model_ICO.reset(Model::CreateFromOBJ("ICO", true));

    assert(model_ICO);
}

void CollisionManager::UpdateWorldTransform() {

	   isCheckCollision = globalVariables->GetBoolValue("CollisionManager", "isCheckCollision");
	if (!isCheckCollision) {
		return;
	}

	for (Collider* collider : colliders_) {
		collider->UpdateWorldTransform();
	}
}

void CollisionManager::Draw(const ViewProjection& viewProjection) {
	isCheckCollision = globalVariables->GetBoolValue("CollisionManager", "isCheckCollision");
	if (!isCheckCollision) {
		return;
	}

	for (Collider* collider : colliders_) {

		collider->Draw(model_ICO.get(), viewProjection);
	}
}

void CollisionManager::Reset() { colliders_.clear(); }

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	Vector3 posA = colliderA->GetCenterPos();
	Vector3 posB = colliderB->GetCenterPos();

	// 座標の差分ベクトル
	Vector3 subtract = posB - posA;
	// 座標AとBの距離を求める
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