#pragma once
#include"Vector3.h"
class Collider {

public:
	//セッター
	float GetRadius() { return radius_; }
	//ゲッター
	void SetRadius(float radius) { radius_ = radius; }

	// 衝突時に呼ばれる関数
	virtual void OnCollision(){};

	// 中心座標を取得
	virtual Vector3 GetCenterPos() const = 0;

	virtual ~Collider() = default;




private:
	// 衝突半径
	float radius_ = 1.5f;

};
