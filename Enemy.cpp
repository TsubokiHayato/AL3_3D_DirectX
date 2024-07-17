#include "Enemy.h"
#include "GameScene.h"
#include <MT.h>
#include <TextureManager.h>
#include <Vector3.h>
#include <cassert>

Enemy::~Enemy() {

	model_ = nullptr;
	
	viewProjection_ = nullptr;
}

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);

	// 3Dモデルの作成
	model_ = model;
	textureHandle_ = TextureManager::Load("slime.jpg");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	viewProjection_ = viewProjection;

	ApproachPhaseInstance();
}

void Enemy::Update() {
	// キャラクターの移動速さ
	const float kCharaSpeed = -0.05f;
	// Enemyの移動ベクトル
	Vector3 moveApproach = {0.0f, 0.0f, kCharaSpeed};
	Vector3 moveLeave = {0.0f, 0.0f, 0.0f};

	switch (phase_) {
	case Phase::Approach:
		ApproachPhaseUpdate();

		worldTransform_.translation_.x += +moveApproach.x;
		worldTransform_.translation_.y += +moveApproach.y;
		worldTransform_.translation_.z += +moveApproach.z;

		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:

		worldTransform_.translation_.x -= +moveApproach.x;
		worldTransform_.translation_.y -= +moveApproach.y;
		worldTransform_.translation_.z -= +moveApproach.z;

		break;
	}

	// 行列計算
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() {

	// 3D作成
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}

void Enemy::Fire() {

	const float kBulletSpeed = 0.8f;

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

	gameScene_->AddEnemyBullet(newBullet);
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

void Enemy::OnCollision() { isDead_ = true; }

Vector3 Enemy::GetWorldPos() {

	Vector3 worldPos = {};

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}