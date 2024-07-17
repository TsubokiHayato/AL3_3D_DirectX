#pragma once

#include "Model.h"

#include "WorldTransform.h"

#include <numbers>

#include "Sprite.h"
#include"PlayerBullet.h"
#include<list>



enum class LRDirection {
	kRight,
	kLeft
};

// 自キャラ
class Player {
public:
	// 初期化
	void Initialize(Model* model,Model* targetModel, uint32_t textureHandle, ViewProjection* viewProjection, Vector3 Position);
	// 更新
	void Update(const Matrix4x4 viewMatrix,const Matrix4x4 viewProjectionMatrix);
	// 描画
	void Draw();

	void DrawUI();
	
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リストの処理を参考に作成する
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	~Player();

	void Attack();

	const Vector3& GetPosition() const { return worldTransform_.translation_; }

	WorldTransform& GetWorldTransform() { return worldTransform_; };
	
	Vector3 GetWorldPos();

	void SetParent(const WorldTransform* parent);


private:

	std::list<PlayerBullet*> bullets_;

	Input* input_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// 3Dモデル
	Model* model_ = nullptr;
	Model* modelTarget_ = nullptr;

	// テクスチャーハンドル
	uint32_t textureHandle_ = 0u;

	// ビューポート
	ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransform3DReticle_;

	
	Sprite* sprite2DReticle_ = nullptr;


	XINPUT_STATE joyState;

};
