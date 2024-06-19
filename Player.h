#pragma once

#include "Model.h"

#include "WorldTransform.h"

#include <numbers>


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
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, Vector3 Position);
	// 更新
	void Update();
	// 描画
	void Draw();

	
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

	// テクスチャーハンドル
	uint32_t textureHandle_ = 0u;

	// ビューポート
	ViewProjection* viewProjection_ = nullptr;

	


};
