#define NOMINMAX
#include <Player.h>

#include <algorithm>
#include <array>
#include <cassert>

#include <ImGuiManager.h>
#include <MapChipField.h>

float lerp(float start, float end, float t);

Player::~Player() {
	model_ = nullptr;
	viewProjection_ = nullptr;
}

void Player::MapCollision(CollisionMapInfo& info) {
	TopCollision(info);
	BottomCollision(info);
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {

	Vector3 result{};

	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kLeftTop
	};

	result.x = center.x + offsetTable[static_cast<uint32_t>(corner)].x;
	result.y = center.y + offsetTable[static_cast<uint32_t>(corner)].y;
	result.z = center.z + offsetTable[static_cast<uint32_t>(corner)].z;

	return result;
}

void Player::TopCollision(CollisionMapInfo& info) {

	// 上昇あり？
	if (info.movement.y <= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> posNew;

	for (uint32_t i = 0; i < posNew.size(); ++i) {

		posNew[i] = CornerPosition(worldTransform_.translation_ + info.movement, static_cast<Corner>(i));
	}

	MapChipType mapChipType{};

	bool hit = false;

	// 左上点当たり判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(posNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右上点あたり判定

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(posNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		// Vector3 Harf{kWidth, kHeight , 0};
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.movement);
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.movement.y = std::max(0.0f, (rect.bottom - worldTransform_.translation_.y) - ((kHeight / 2) + kBlank));
		//
		info.isHitCeiling = true;
	}
}
void Player::BottomCollision(CollisionMapInfo& info) {

	// 上昇あり？
	if (info.movement.y >= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> posNew;

	for (uint32_t i = 0; i < posNew.size(); ++i) {

		posNew[i] = CornerPosition(worldTransform_.translation_ + info.movement, static_cast<Corner>(i));
	}

	MapChipType mapChipType{};

	bool hit = false;

	// 左下点当たり判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(posNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右下点あたり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(posNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		Vector3 Harf{kWidth, kHeight , 0};
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.movement-Harf);
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.movement.y = std::min(0.0f, (rect.top - worldTransform_.translation_.y) + ((kHeight / 2)+kBlank ));
		// 地面にあたったことを記録する
		info.isLanding = true;
	}
}
// 判定結果を反映して移動させる③
void Player::CellingReflectionMove(const CollisionMapInfo& info) { worldTransform_.translation_ += info.movement; }

// 天井に接触している場合の処理④
void Player::HitCeiling(const CollisionMapInfo& info) {
	if (info.isHitCeiling) {

		DebugText::GetInstance()->ConsolePrintf("hit ceiling");
		velocity_.y = 0;
	} else {
		ImGui::Text("not hit ceiling ");
	}
}
void Player::HitLanding(const CollisionMapInfo& info) {
	if (onGround_) {
		// 設置状態の処理
		if (velocity_.y > 0.0f) {

			onGround_ = false;
		} else {

			MapChipType mapChipType{};

			bool hit = false;

			//// 左下点当たり判定
			// kLeftBottom + Vector3(0, -0, 0);

			//// 右下点あたり判定
			// kRightBottom + Vector3(0, -0, 0);

			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			if (!hit) {
				// 空中状態に切り替える
				onGround_ = false;
			}
		}

	} else {

		// 空中状態の処理

		if (info.isLanding) {

			onGround_ = true;

			velocity_.x *= (1.0f - kAttenuationLanding);

			velocity_.y = 0.0f;
		}
	}
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

	/*------------
	*
	   移動入力 : 1

	-------------*/
	if (onGround_) {

		if (Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D)) {

			// 左右加速
			Vector3 acceleration = {};

			if (Input::GetInstance()->PushKey(DIK_D)) {
				worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
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

				worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

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

	/*--------------------------

	移動量を加味して衝突判定する : 2

	-------------------------*/
	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo{};

	// 移動量に速度の値をコピー
	collisionMapInfo.movement = velocity_;

	MapCollision(collisionMapInfo);

	/*--------------------------

	　判定結果を反映して移動させる : 3

	-------------------------*/

	CellingReflectionMove(collisionMapInfo);

	/*-------------------------
	*
	天井に接触しているときの処理 : 4

	--------------------------*/
	HitCeiling(collisionMapInfo);

	/*-------------------------

	壁に接触しているときの処理 : 5

	--------------------------*/

	/*-----------------

	設置状態の切り替え : 6

	-----------------*/
	// 着地フラグ
	bool landing = false;

	// 地面との当たり判定
	// 下降中？
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.7f) {
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
			worldTransform_.translation_.y = 1.7f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuationLanding);
			// 下方向速度リセット
			velocity_.y = 0.0f;
			// 設置状態に移行
			onGround_ = true;
		}
	}

	/*-----------

	 旋回制御 : 7

	-----------*/
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
	// worldTransform_.translation_ += velocity_;

	/*---------

	行列計算 : 8

	----------*/
	worldTransform_.UpdateMatrix();

	// imgui

	ImGui::Begin("Debug");
	ImGui::Text("A D : idou");
	ImGui::Text("W : Jump");

	ImGui::Text("playerPos.x %f", worldTransform_.translation_.x);
	ImGui::Text("playerPos.y %f", worldTransform_.translation_.y);

	ImGui::Text("playerVelocity.x %f", velocity_.x);
	ImGui::Text("playerVelocity.y %f", velocity_.y);

	ImGui::Text("hit ceiling %d", collisionMapInfo.isHitCeiling);
	ImGui::Text("hit Bottom %d", collisionMapInfo.isLanding);

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
