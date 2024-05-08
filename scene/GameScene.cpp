#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>





GameScene::GameScene() {}

GameScene::~GameScene() {

	// 3Dモデル削除
	delete model;

	// delete player;

	delete modelBlock_;

	delete debugCamera_;

	delete modelSkydome_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	/*-------------------
	       　テクスチャー
	    -------------------*/
	// ポーションの画像
	textureHandle = TextureManager::Load("Recovery_agents.png");

	model = Model::Create();

	modelBlock_ = Model::Create();
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
	// 自キャラの初期化
	player->Initialize(model, textureHandle, &viewProjection_);

	//SkyDome作成
	skydome = new Skydome;

	
	modelSkydome_ = Model::CreateFromOBJ("sakaban", true);
	skydome->Initialize(modelSkydome_, &viewProjection_);

	//
	const uint32_t kNumBlockHorizonal = 20;
	const uint32_t kNumBlockVirtical = 10;

	//
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	//
	debugCamera_ = new DebugCamera(1280, 720);
	//
	worldTransformBlocks_.resize(kNumBlockHorizonal);

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizonal; ++j) {
			if ((i+j)%2==0){
					worldTransformBlocks_[i].resize(kNumBlockHorizonal);
			
			worldTransformBlocks_[i][j] = new WorldTransform();

			worldTransformBlocks_[i][j]->Initialize();

			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;	
				}
			

		}
	}

}

void GameScene::Update() {

	debugCamera_->Update();
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

	skydome->Update();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			worldTransformBlock->matWorld_ =worldTransformBlock->UpdateMatMatrix(worldTransformBlock);

			worldTransformBlock->TransferMatrix();
		}
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
	// 自キャラ
	player->Draw();
	skydome->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

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
