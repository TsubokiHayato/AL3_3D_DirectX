#include "Enemy.h"
#include <TextureManager.h>
#include <Vector3.h>
#include <cassert>

#include <MT.h>

void Enemy::Fire() {

	const float kBulletSpeed = 0.4f;

	Vector3 playerVec = player_->GetWorldPos();
	Vector3 enemyVec = GetWorldPos();

	Vector3 diff{};

	diff.x = playerVec.x - enemyVec.x;
	diff.y = playerVec.y - enemyVec.y;
	diff.z = playerVec.z - enemyVec.z;

	diff = Normalize(diff);

	diff.x *= kBulletSpeed;
	diff.y *= kBulletSpeed;
	diff.z *= kBulletSpeed;

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, diff);

	bullets_.push_back(newBullet);
}

void Enemy::ApproachPhaseInstance() {

	// 発射タイマーを初期化
	ShotTimer = kFireInterval;
}

void Enemy::ApproachPhaseUpdate() {

	ShotTimer--;
	if (ShotTimer <= 0) {
		Fire();
		ShotTimer = kFireInterval;
	}
}

Vector3 Enemy::GetWorldPos() {

	Vector3 worldPos = {};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() {}

Enemy::~Enemy() {

	model_ = nullptr;
	viewProjection_ = nullptr;
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);

	// 3Dモデルの作成
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.x = 5.0f;
	worldTransform_.translation_.y = 0.0f;
	worldTransform_.translation_.z = 100.0f;
	textureHandle_ = TextureManager::Load("slime.jpg");

	viewProjection_ = viewProjection;

	ApproachPhaseInstance();
}

void Enemy::Update() {

	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// Enemyの移動ベクトル
	Vector3 moveApproach = {0.0f, 0.0f, 0.0f};
	Vector3 moveLeave = {0.0f, 0.0f, 0.0f};

	// キャラクターの移動速さ
	const float kCharaSpeed = 0.2f;

	ApproachPhaseUpdate();
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	switch (phase_) {
	case Phase::Approach:
		moveApproach.z -= kCharaSpeed;
		worldTransform_.translation_ += moveApproach;

		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:

		moveLeave.z += kCharaSpeed;
		worldTransform_.translation_ += moveLeave;

		break;
	}

	// 行列計算
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() {

	// 3D作成
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

	for (EnemyBullet* bullet : bullets_) {

		bullet->Draw(*viewProjection_);
	}
}
