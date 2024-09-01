
#include <Player.h>

#include <algorithm>
#include <cassert>

#include "ViewProjection.h"
#include "WinApp.h"
#include "WorldTransform_MT.h"
#include <ImGuiManager.h>
#include <MT.h>
#include <TextureManager.h>

Player::~Player() {
	model_ = nullptr;
	modelTarget_ = nullptr;
	viewProjection_ = nullptr;

	delete sprite2DReticle_;

	input_ = nullptr;
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Attack() {

	if (input_->PushKey(DIK_SPACE)) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = worldTransform3DReticle_.translation_ - GetWorldPos();
		velocity = Normalize(velocity) * kBulletSpeed;

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		newBullet->SetParent(worldTransform_.parent_);
		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPos() {

	Vector3 worldPos = {};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

// 初期化
void Player::Initialize(Model* model, Model* targetModel,  ViewProjection* viewProjection, Vector3 Position) {
	assert(model);

	// 3Dモデルの作成
	model_ = model;
	modelTarget_ = targetModel;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = Position;

	

	//// プレイヤーの初期角度
	 worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	viewProjection_ = viewProjection;

	worldTransform3DReticle_.Initialize();

	Vector2 anchorPoint = {0.5f, 0.5f};
	uint32_t textureReticle = TextureManager::Load("2D_Reticle.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, {}, {1.0f, 1.0f, 1.0f, 1.0f}, anchorPoint);

	input_ = Input::GetInstance();
}
// 更新
void Player::Update(const Matrix4x4 viewMatrix, const Matrix4x4 viewProjectionMatrix) {

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});


	Vector2 spritePos = sprite2DReticle_->GetPosition();

	// playerの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};

	// キャラクターの移動速さ
	const float kCharaSpeed = 0.2f;

	// 移動処理

	if (input_->PushKey(DIK_A)) {
		move.x -= kCharaSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharaSpeed;
	}

	if (input_->PushKey(DIK_S)) {
		move.y -= kCharaSpeed;
	} else if (input_->PushKey(DIK_W)) {
		move.y += kCharaSpeed;
	}

	Attack();

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	const float kDistancePlayerTo3DReticle = 50.0f;
	//
	Vector3 offset = {0, 0, 1.0f};
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	offset = Normalize(offset) * kDistancePlayerTo3DReticle;
	worldTransform3DReticle_.translation_ = offset;

	worldTransform3DReticle_.UpdateMatrix();

	Vector3 positionReticle = worldTransform3DReticle_.translation_;

	POINT mousePos;
	GetCursorPos(&mousePos);

	HWND hwnd = WinApp::GetInstance()->GetHwnd();

	ScreenToClient(hwnd, &mousePos);

	// 2Dレティクルのスプライトにマウス座標を設定
	Vector2 reticlePosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
	sprite2DReticle_->SetPosition(reticlePosition);

	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matVPV = viewMatrix * viewProjectionMatrix * matViewport;

	Matrix4x4 matInverseVPV = Inverse(matVPV);

	Vector3 posNear = Vector3(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y), 0.0f);
	Vector3 posFar = Vector3(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y), 1.0f);

	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	Vector3 mouseDirection = posFar - posNear; // posNearからposFarへのベクトルを計算する
	mouseDirection = Normalize(mouseDirection);

	worldTransform3DReticle_.translation_.x = posNear.x + mouseDirection.x * kDistanceTestObject; // posNearからmouseDirectionの方向にkDistanceTestObject進んだ座標
	worldTransform3DReticle_.translation_.y = posNear.y + mouseDirection.y * kDistanceTestObject; // posNearからmouseDirectionの方向にkDistanceTestObject進んだ座標
	worldTransform3DReticle_.translation_.z = posNear.z + mouseDirection.z * kDistanceTestObject; // posNearからmouseDirectionの方向にkDistanceTestObject進んだ座標
	worldTransform3DReticle_.UpdateMatrix();

	
	// 移動限界座標
	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 11.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.translation_ += move;

	// 行列計算
	worldTransform_.UpdateMatrix();
}

// 描画
void Player::Draw() {
	// 3D作成
	model_->Draw(worldTransform_, *viewProjection_);
	// modelTarget_->Draw(worldTransform3DReticle_, *viewProjection_);
	for (PlayerBullet* bullet : bullets_) {

		bullet->Draw(*viewProjection_);
	}
}

void Player::DrawUI() { sprite2DReticle_->Draw(); }

void Player::OnCollision() {}
