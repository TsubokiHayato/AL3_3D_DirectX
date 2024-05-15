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
void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos) {
	assert(model);

	// 3Dモデルの作成
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// プレイヤーの初期位置
	worldTransform_.translation_ = pos;

	// プレイヤーの初期角度
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	viewProjection_ = viewProjection;
}
// 更新
void Player::Update() {

	// 移動入力
	if (onGround_) {

		if (Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D)) {
			worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
			// 左右加速
			Vector3 acceleration = {};

			if (Input::GetInstance()->PushKey(DIK_D)) {
				worldTransform_.rotation_.y = std::numbers::pi_v<float>  / 2.0f;
				acceleration.x += kAcceleration;

				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}

				if (lrdDirection_ != LRDirection::kRight) {
					lrdDirection_ = LRDirection::kRight;
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_A)) {

				acceleration.x -= kAcceleration;

				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}

				if (lrdDirection_ != LRDirection::kLeft) {
					lrdDirection_ = LRDirection::kLeft;
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
			}
			// 加速/減速
			velocity_ += acceleration;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_W)) {
			// jump
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}

	} else {

		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	// 着地フラグ
	bool landing = false;

	// 地面との当たり判定
	// 下降中？
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 2.0f) {
			landing = true;
		}
	}

	// 設置判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			// めり込み
			worldTransform_.translation_.y = 2.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuationLanding);
			// 下方向速度リセット
			velocity_.y = 0.0f;
			// 設置状態に移行
			onGround_ = true;
		}
	}

	if (turnTimer_ > 0.0f) {

		turnTimer_ -= static_cast<float>(1) / 60;

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> * 3.0f / 2.0f, std::numbers::pi_v<float> / 2.0f

		};
		// 状態に応じた目標角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrdDirection_)];
		// 自キャラ角度を設定する
		worldTransform_.rotation_.y = lerp(turnFirstRotationY_, destinationRotationY, turnTimer_ / kTimeTurn);
	}

	// 移動
	worldTransform_.translation_ += velocity_;
	// 行列計算
	worldTransform_.UpdateMatrix();
	ImGui::Begin("Debug");
	ImGui::Text("A D : idou");
	ImGui::Text("W : Jump");
	ImGui::Text("playerPos.x %f", worldTransform_.translation_.x);
	ImGui::Text("playerPos.y %f", worldTransform_.translation_.y);
	ImGui::End();
}
// 描画
void Player::Draw() {
	// 3D作成
	model_->Draw(worldTransform_, *viewProjection_);
}
float lerp(float start, float end, float t) {
	float result = {};
	result = (1.0f - t) * start + t * end;
	return result;
}