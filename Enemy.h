#pragma once

#include "BaseCharacter.h"
#include "Model.h"
#include "WorldTransform.h"

class Enemy {
public:
	
	// 初期化
	void Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection);

	// 更新
	void Update();

	// 描画
	void Draw();


private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	WorldTransform worldHeadTransform_;
	WorldTransform worldLArmTransform_;
	WorldTransform worldRArmTransform_;

	// 3Dモデル
	Model* modelHead_ = nullptr;
	Model* modelLeftArm_ = nullptr;
	Model* modelRightArm_ = nullptr;

	// ビューポート
	const ViewProjection* viewProjection_ = nullptr;

	static const int kModelIndexHead = 0;
	static const int kModelIndexL_arm = 1;
	static const int kModelIndexR_arm = 2;


	float theta = 0.0f;
};