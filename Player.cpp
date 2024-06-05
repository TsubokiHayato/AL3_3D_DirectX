#include <Player.h>

#include <algorithm>
#include <cassert>

#include <ImGuiManager.h>

float lerp(float start, float end, float t);

Player::~Player() {
	model_ = nullptr;
	viewProjection_ = nullptr;
}

// 初期化
void Player::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);

	// 3Dモデルの作成
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// プレイヤーの初期角度
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	input_ = Input::GetInstance();

	viewProjection_ = viewProjection;
}
// 更新
void Player::Update() {

	// playerの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};

	// キャラクターの移動速さ
	const float kCharaSpeed = 0.2f;

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

	ImGui::SliderFloat3("Player Position", &worldTransform_.translation_.x, -600.0f, 600.0f);

	// 移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;

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
}