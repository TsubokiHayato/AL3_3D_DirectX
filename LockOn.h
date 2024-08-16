#pragma once

#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "optional"

#include <memory>
#include"Enemy.h"

#include"GlobalVariables.h"

 class LockOn {

public:

	 void Initialize(uint32_t textureReticle);

	void Update(std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	void Draw();

	//検索
	void Search(std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	//範囲外判定
	bool IsOutOfRangeJudgment(const ViewProjection& viewProjection);

	Vector3 GetTargetPos() const;

	bool ExistTarget() const { return target_ ? true : false; }
	


 void ApplyGlobalVariables();
private:
	std::unique_ptr<Sprite> lockOnMark_=nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	

	const Enemy* target_ = nullptr;

	uint32_t textureHandle;
	float kDegreeToRadian = 3.14f / 180.0f;


	float minDistance_ = 10.0f;
	float maxDistance_ = 30.0f;
	float angleRange_ = 20.0f * kDegreeToRadian;

	bool isSearch = false;

	GlobalVariables* globalVariables;
	
 };
