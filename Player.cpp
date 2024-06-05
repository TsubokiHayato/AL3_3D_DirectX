#define NOMINMAX
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

	viewProjection_ = viewProjection;
}
// 更新
void Player::Update() {

	
	// 行列計算
	worldTransform_.UpdateMatrix();

	
}
// 描画
void Player::Draw() {
	// 3D作成
	model_->Draw(worldTransform_, *viewProjection_);
}