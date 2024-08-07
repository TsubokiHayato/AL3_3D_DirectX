#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"


#include "Player.h"

#include"Enemy.h"
#include<memory>

#include"SkyDome.h"
#include"Plane.h"

#include<DebugCamera.h>

#include"FollowCamera.h"

#include"LockOn.h"

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

	std::unique_ptr<Enemy> enemy = nullptr;

	std::list<std::unique_ptr<Enemy>> enemies;

	std::unique_ptr<SkyDome> skyDome = nullptr;

	
	std::unique_ptr<Plane> plane = nullptr;

	std::unique_ptr < DebugCamera> debugCamera_ = nullptr;



	/*------------
	   テクスチャ
	------------*/
	uint32_t textureHandle = 0;

	/*------------
	   model
	------------*/
	std::unique_ptr<Model> modelPlayer_Head = nullptr;
	std::unique_ptr<Model> modelPlayer_Body = nullptr;
	std::unique_ptr<Model> modelPlayer_LeftArm = nullptr;
	std::unique_ptr<Model> modelPlayer_RightArm = nullptr;

	std::unique_ptr<Model> modelHammer = nullptr;

	std::unique_ptr<Model> modelEnemy_Head = nullptr;
	std::unique_ptr<Model> modelEnemy_LeftArm = nullptr;
	std::unique_ptr<Model> modelEnemy_RightArm = nullptr;


	std::unique_ptr<Model> modelSkyDome = nullptr;

	std::unique_ptr<Model> modelPlane = nullptr;
	
	/*--------------
	* ワールド・ビュー
	--------------*/
	
	ViewProjection viewProjection_;

	bool isDebugCameraActive_ = false;


	/*--------------
	* camera
	--------------*/

	std::unique_ptr<FollowCamera> followCamera=nullptr;


	std::unique_ptr<LockOn> lockOn_;


};
