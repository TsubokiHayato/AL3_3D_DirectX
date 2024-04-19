#pragma once

#include "WorldTransform.h"
#include "Model.h"

//#include "TextureManager.h"


//自キャラ
class Player {
public:

	// 初期化
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);
	//更新
	void Update();
	//描画
	void Draw();

private:

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//3Dモデル
	Model* model_ = nullptr;

	//テクスチャーハンドル
	uint32_t textureHandle_ = 0u;

	//ビューポート
	ViewProjection* viewProjection_=nullptr;

};



