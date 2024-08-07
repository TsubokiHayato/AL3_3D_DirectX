#pragma once

#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "optional"

#include <memory>
#include"Enemy.h"

 class LockOn {

public:
	void Initialize();

	void Update();

	void Draw();

private:
	std::unique_ptr<Sprite> lockOnMark_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビューポート
	ViewProjection* viewProjection_ = nullptr;

	const Enemy* target_ = nullptr;

	uint32_t textureHandle;
 };
