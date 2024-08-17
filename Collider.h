#pragma once
#include"Vector3.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"
class Collider {

public:
	//初期化
	void Initialize();


	//セッター
	float GetRadius() { return radius_; }
	//ゲッター
	void SetRadius(float radius) { radius_ = radius; }

	// 衝突時に呼ばれる関数
	virtual void OnCollision(){};

	// 中心座標を取得
	virtual Vector3 GetCenterPos() const = 0;

	virtual ~Collider() = default;

	//ワールドトランスフォームの初期化
	void UpdateWorldTransform();

		// 描画
	void Draw(Model* model, const ViewProjection& viewProjection);

private:
	// 衝突半径
	float radius_ = 1.0f;

	WorldTransform worldTransform;
};
