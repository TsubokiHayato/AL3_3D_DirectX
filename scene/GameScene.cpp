#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include<ImGuiManager.h>


GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete player;
	delete enemy;
	
	// 3Dモデル削除

	delete modelPlayer;
	delete modelEnemy;

	
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	

	

	/*-------------------
	       　Axis
	    -------------------*/
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	/*--------------
	* ワールド・ビュー
	--------------*/

	viewProjection_.farZ = 1145;

	viewProjection_.Initialize();

	
	
	/*---------
	* Chara
	--------*/

	// 自キャラの生成
	player = new Player;
	// ポーションの画像
	textureHandle = TextureManager::Load("Recovery_agents.png");

	modelPlayer = Model::Create();
	// 自キャラの初期化
	player->Initialize(modelPlayer,textureHandle ,&viewProjection_);

	enemy = new Enemy;
	modelEnemy = Model::Create();
	// Enemyの初期化
	enemy->Initialize(modelEnemy, &viewProjection_);

	/*-----------
	 DEBUG_CAMERA
	-----------*/
	debugCamera_ = new DebugCamera(1280, 720);

	

	
}

void GameScene::Update() {

	/*-----------
	DebugCamera
	-----------*/
#ifdef _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
#endif

	if (isDebugCameraActive_) {

		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetView();
		viewProjection_.matProjection = debugCamera_->GetProjection();

		//
		viewProjection_.TransferMatrix();
	} else {
		//
		viewProjection_.UpdateMatrix();
	}

	/*----------
	     3D
	----------*/
	// 自キャラの更新
	player->Update();
	//enemyの更新
	enemy->Update();
	

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	///

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	///

	/*-----------
	     3D
	-----------*/
	// 自キャラ
	player->Draw();
	//Enemy
	enemy->Draw();

	
	

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
