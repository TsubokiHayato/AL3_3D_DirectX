#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	

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

	modelPlayer_Head.reset(Model::CreateFromOBJ("float_Head", true));
	modelPlayer_Body.reset(Model::CreateFromOBJ("float_Body", true));
	modelPlayer_LeftArm.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelPlayer_RightArm.reset(Model::CreateFromOBJ("float_R_arm", true));

	/*--------------
	* ワールド・ビュー
	--------------*/
	viewProjection->Initialize();

	/*---------
	* Chara
	--------*/


	// 自キャラの生成
	player = std::make_unique<Player>();
	// 自キャラの初期化
	player->Initialize(modelPlayer_Head.get(), modelPlayer_Body.get(), modelPlayer_LeftArm.get(), modelPlayer_RightArm.get(),viewProjection);



	/*---------
	  SkyDome
	---------*/
	modelSkyDome.reset(Model::CreateFromOBJ("SkyDome", true));

	skyDome = std::make_unique<SkyDome>();

	skyDome->Initialize(modelSkyDome.get(), viewProjection);

	/*---------
	  plane
	---------*/
	modelPlane.reset(Model::CreateFromOBJ("plane", true));

	plane = std::make_unique<Plane>();

	plane->Initialize(modelPlane.get(), viewProjection);




	/*-----------
	 DEBUG_CAMERA
	-----------*/
	debugCamera_ = std::make_unique < DebugCamera>(1280, 720);

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();
	followCamera->SetTarget(&player->GetWorldTransform()); 

}

void GameScene::Update() {


	/*-----------
	DebugCamera
	-----------*/
#ifdef _DEBUG

	if (input_->TriggerKey(DIK_Q)) {
		isDebugCameraActive_ = true;
	}
#endif

	if (isDebugCameraActive_) {

		debugCamera_->Update();
		viewProjection->matView = debugCamera_->GetView();
		viewProjection->matProjection = debugCamera_->GetProjection();

		//
		viewProjection->TransferMatrix();
	} else {
		//
		viewProjection->UpdateMatrix();
	}

	followCamera->Update();

	viewProjection = followCamera->GetViewProjection();
	viewProjection->matProjection = followCamera->GetProjection();

	/*----------
	     3D
	----------*/
	// 自キャラの更新
	player->Update();
	skyDome->Update();
	plane->Update();
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

	skyDome->Draw();

	plane->Draw();

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
