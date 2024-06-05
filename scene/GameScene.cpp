#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	// 3Dモデル削除
	delete model;

	delete player;

	delete debugCamera_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	/*-------------------
	       　テクスチャー
	    -------------------*/
	//ポーションの画像
	textureHandle = TextureManager::Load("Recovery_agents.png");

	model = Model::Create();
	/*--------------
	* ワールド・ビュー
	--------------*/
	viewProjection_.Initialize();

	/*---------
	* Chara
	--------*/

	// 自キャラの生成
	player = new Player;
	// 自キャラの初期化
	player->Initialize(model, textureHandle, &viewProjection_);

	/*--------------
	  デバックカメラ
	--------------*/
	// デバックカメラの生成
	//debugCamera_ = new DebugCamera(1280, 720);

	
}

void GameScene::Update() {

	/*----------
	     3D
	----------*/
	// 自キャラの更新
	player->Update();

	/*--------------
	  デバックカメラ
	--------------*/

	debugCamera_->Update();

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
	if (input_->TriggerKey(DIK_R)) {
		isDebugCameraActive_ = false;
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
	//自キャラ
	player->Draw();

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
