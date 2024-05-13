#pragma once

#include "Model.h"

#include "WorldTransform.h"

#include <numbers>

#include "Input.h"

enum class LRDirection {
	kRight,
	kLeft
};

// 自キャラ
class Player {
public:
	// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos);
	// 更新
	void Update();
	// 描画
	void Draw();

	

	~Player();

	

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// 3Dモデル
	Model* model_ = nullptr;

	// ビューポート
	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kAttenuationLanding = 0.5f;

	static inline const float kLimitRunSpeed = 1.0f;

	LRDirection lrdDirection_ = LRDirection::kRight;

	float turnFirstRotationY_=0.0f;
	float turnTimer_ = 0.0f;

	static inline const float kTimeTurn = 0.3f;
	//設置状態フラグ
	bool onGround_ = true;
	//重力加速度(下)
	static inline const float kGravityAcceleration = 0.98f;
	//最大落下速度(下)
	static inline const float kLimitFallSpeed = 0.98f;
	//ジャンプ初速(上)
	static inline const float kJumpAcceleration = 5.0f;



};
