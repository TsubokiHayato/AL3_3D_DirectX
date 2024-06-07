#pragma once

#include "Model.h"

#include "WorldTransform.h"

#include <numbers>

#include "Input.h"
#include "PlayerBullet.h"
#include <list>

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

};
