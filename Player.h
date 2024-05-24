#pragma once

#include "Model.h"

#include "WorldTransform.h"

#include <numbers>

#include<DebugText.h>
#include "Input.h"

class MapChipField;

enum class LRDirection { kRight, kLeft };

struct CollisionMapInfo {

	// 天井衝突フラグ
	bool isHitCeiling = false;

	// 着地フラグ
	bool isLanding = false;
	// 壁接触フラグ
	bool isHitWall;

	// 移動量
	Vector3 movement;
	// 正しくは　amount of movement
};

enum Corner {
	kRightBottom, // 右下
	kLeftBottom,  // 左上
	kRightTop,    // 右上
	kLeftTop,     // 左上

	kNumCorner // 要素数
};

// 自キャラ
class Player {
private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// 3Dモデル
	Model* model_ = nullptr;

	// ビューポート
	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.2f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kAttenuationLanding = 0.2f;

	static inline const float kLimitRunSpeed = 0.5f;

	LRDirection lrdDirection_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;

	static inline const float kTimeTurn = 0.3f;
	// 設置状態フラグ
	bool onGround_ = false;
	// 重力加速度(下)
	static inline const float kGravityAcceleration = 0.05f;
	// 最大落下速度(下)
	static inline const float kLimitFallSpeed = 0.5f;
	// ジャンプ初速(上)
	static inline const float kJumpAcceleration = 1.0f;

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.0f;
	static inline const float kHeight = 1.0f;

	static inline const float kBlank =0.0f;
	

public:
	// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos);
	// 更新
	void Update();
	// 描画
	void Draw();

	~Player();
	const Vector3& GetPosition() const { return worldTransform_.translation_; }

	WorldTransform& GetWorldTransform() { return worldTransform_; };

	const Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; };

	void MapCollision(CollisionMapInfo& info);

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	void TopCollision(CollisionMapInfo& info);
	
	void BottomCollision(CollisionMapInfo& info);

	void LeftCollision(CollisionMapInfo& info);

	void RightCollision(CollisionMapInfo& info);
	

	//判定結果を反映して移動させる
	void CellingReflectionMove(const CollisionMapInfo& info);

	void HitCeiling(const CollisionMapInfo& info);

	void HitLanding(const CollisionMapInfo& info);
};
