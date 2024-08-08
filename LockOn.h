#pragma once

#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "optional"

#include <memory>
#include"Enemy.h"

 class LockOn {

public:

	 void Initialize(uint32_t textureReticle);

	void Update(std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	void Draw();

	void Search(std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

private:
	std::unique_ptr<Sprite> lockOnMark_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビューポート
	ViewProjection* viewProjection_ = nullptr;

	const Enemy* target_ = nullptr;

	uint32_t textureHandle;

	float kDegreeToRadian;

	float minDistance_ = 10.0f;
	float maxDistance_ = 30.0f;
	float angleRange_ = 20.0f * kDegreeToRadian;

 };
