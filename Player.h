#pragma once

#include "WorldTransform.h"
#include "Model.h"


//#include "TextureManager.h"


//自キャラ
class Player {
public:

	// 初期化
	void Initialize(Model* modelHead,Model* modelBody, Model* modelLeftArm,Model* modelRightArm, ViewProjection* viewProjection);
	//更新
	void Update();
	//描画
	void Draw();

private:

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	WorldTransform worldHeadTransform_;
	WorldTransform worldBodyTransform_;
	WorldTransform worldLArmTransform_;
	WorldTransform worldRArmTransform_;

	


	//3Dモデル
	Model* modelHead_ = nullptr;
	Model* modelBody_ = nullptr;
	Model* modelLeftArm_ = nullptr;
	Model* modelRightArm_ = nullptr;

	//テクスチャーハンドル
	uint32_t textureHandle_ = 0u;

	//ビューポート
	ViewProjection* viewProjection_=nullptr;

	ViewProjection* HeadViewProjection_;
	ViewProjection* BodyViewProjection_;
	ViewProjection* LArmViewProjection_;
	ViewProjection* RArmViewProjection_;


	Vector3 move;
	

};



