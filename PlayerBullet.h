#pragma once
#include "Model.h"

#include "WorldTransform.h"

#include <numbers>

#include "Input.h"

class PlayerBullet {
public:
	void Initialize(Model* model, const Vector3& pos);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// 3Dモデル
	Model* model_ = nullptr;

	// テクスチャーハンドル
	uint32_t textureHandle_ = 0u;

	// ビューポート
	ViewProjection* viewProjection_ = nullptr;
		
};
