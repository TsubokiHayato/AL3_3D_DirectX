#include "Enemy.h"

#include <cassert>

#include "MT_Matrix.h"

#include "ImGuiManager.h"

void Enemy::Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection, const Vector3& position) {
	// 基底クラスの初期化
	BaseCharacter::Initialize(models, viewProjection);

	assert(models[kModelIndexHead]);
	assert(models[kModelIndexL_arm]);
	assert(models[kModelIndexR_arm]);

	modelHead_ = models[kModelIndexHead];

	modelLeftArm_ = models[kModelIndexL_arm];
	modelRightArm_ = models[kModelIndexR_arm];

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldHeadTransform_.Initialize();
	worldLArmTransform_.Initialize();
	worldRArmTransform_.Initialize();

	worldHeadTransform_.parent_ = &worldTransform_;
	worldLArmTransform_.parent_ = &worldHeadTransform_;
	worldRArmTransform_.parent_ = &worldHeadTransform_;
	

	worldTransform_.translation_ = position;

	//	worldTransform_.translation_ = {0.0f, 1.0f, 0.0f};

	viewProjection_ = viewProjection;
}

void Enemy::Update() {
	// 回転角度を更新
	theta += 0.1f; // 回転速度を調整

	worldTransform_.translation_.x = sin(theta)*7;
	worldTransform_.translation_.y = 1.0f;
	worldTransform_.translation_.z = cos(theta)*7;



	// ImGuiによるUIの更新
	ImGui::DragFloat3("Scale", &worldTransform_.scale_.x, 0.1f);
	ImGui::DragFloat3("Rotation", &worldTransform_.rotation_.x, 0.1f);
	ImGui::DragFloat3("Transform", &worldTransform_.translation_.x, 0.1f);

	// マトリックスの更新
	worldTransform_.UpdateMatrix();
	worldHeadTransform_.UpdateMatrix();
	worldLArmTransform_.UpdateMatrix();
	worldRArmTransform_.UpdateMatrix();
}

void Enemy::Draw() {

	modelHead_->Draw(worldHeadTransform_, *viewProjection_);
	/*modelLeftArm_->Draw(worldLArmTransform_, *viewProjection_);
	modelRightArm_->Draw(worldRArmTransform_, *viewProjection_);*/
}

Vector3 Enemy::GetWorldTransform()const {

	const Vector3 offset = {0.0f, 1.0f, 0.0f};

	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}

Vector3 Enemy::GetCenterPos() const {

	const Vector3 offset = {0.0f, 1.0f, 0.0f};
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;

}

