#pragma once

#include "Model.h"

#include "WorldTransform.h"

#include <numbers>

#include "Input.h"
#include "PlayerBullet.h"
#include <list>

//行動フェーズ
enum class Phase {
	Approach,//接近
	Leave//離脱
};

class Enemy {

	public: 
		// 初期化
	    void Initialize(Model* model, ViewProjection* viewProjection);
	    // 更新
	    void Update();
	    // 描画
	    void Draw();
		
		private:

			// ワールドトランスフォーム
	        WorldTransform worldTransform_;

	        // 3Dモデル
	        Model* model_ = nullptr;

	        // テクスチャーハンドル
	        uint32_t textureHandle_ = 0u;

			// ビューポート
	        ViewProjection* viewProjection_ = nullptr;

			//フェーズ
	        Phase phase_ = Phase::Approach;
};
