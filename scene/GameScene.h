#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

//#include <DebugCamera.h>
#include "Player.h"
#include<memory>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	/*-----------
	　 キャラクタ
	-----------*/

	std::unique_ptr<Player> player = nullptr;

	/*------------
	   テクスチャ
	------------*/
	uint32_t textureHandle = 0;
	std::unique_ptr<Model> model = nullptr;

	
	/*--------------
	* ワールド・ビュー
	--------------*/
	
	ViewProjection viewProjection;

};
