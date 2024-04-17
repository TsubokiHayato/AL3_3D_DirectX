#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include <ImGuiManager.h>

#include <PrimitiveDrawer.h>

#include <AxisIndicator.h>

GameScene::GameScene() {}

GameScene::~GameScene() { 

	// 3Dモデル削除
	delete model;

	// 2Dスプライト削除
	delete sprite;

	// カメラ削除
	delete debugCamera;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	/*-------------------
	   初期化: テクスチャー
	-------------------*/

	// ポーション
	textureHandle = TextureManager::Load("Recovery_agents.png");

	// 2Dスプライトの作成
	sprite = Sprite::Create(textureHandle, {100, 50});

	// 3Dモデルの作成
	model = Model::Create();

	// ワールドトランスフォームの初期化
	worldTransform.Initialize();

	// ビュープロジェクションの初期化
	viewProjection.Initialize();

	/*-----------------
	     初期化: 音楽
	-----------------*/
	soundDataHandle = audio_->LoadWave("fanfare.wav");
	audio_->PlayWave(soundDataHandle);
	voiceHandle = audio_->PlayWave(soundDataHandle, true);

	/*-----------------------
	     初期化: デバッグ
	-----------------------*/

	// ライン描画が参考するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection);

	// デバッグカメラの生成
	//                         (画面横幅,	画面縦幅)
	debugCamera = new DebugCamera(1280, 720);

	/*-----------------------
	     初期化: 軸方向標示
	-----------------------*/

	// 軸方向標示の標示を有効する
	AxisIndicator::GetInstance()->SetVisible(true);

	// 軸方向標示が参考するビュープロジェクションを指定する(アドレスなし)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera->GetViewProjection());
}

void GameScene::Update() {

	/*-------------
	  デバッグ関連
	-------------*/
	ImGui::Begin("debug1");
	// デバッグテキスト表示
	ImGui::Text("Kamata Tarou %d.%d.%d", 2050, 12, 31);

	// float入力ボックス
	ImGui::InputFloat3("inputFloat3", inputFloat3);

	// floatスライダー
	ImGui::SliderFloat3("sliderFloat3", inputFloat3, 0.0f, 1.0f);

	ImGui::End();

	debugCamera->Update();

	/*----------
	     2D
	----------*/
	// スプライトの今の座標を取得
	Vector2 pos = sprite->GetPosition();

	// 座標の値を変更
	pos.x += 2.0f;
	pos.y += 1.0f;

	// 移動した座標をスプライトに反映
	sprite->SetPosition(pos);

	/*----------
	    music
	----------*/

	// spaceKeyを押したら
	if (input_->TriggerKey(DIK_SPACE)) {

		// 音楽を止める
		audio_->StopWave(voiceHandle);
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
	/*----------
	     2D
	----------*/
	sprite->Draw();

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

	// 3Dポーションを作成
	model->Draw(worldTransform, viewProjection, textureHandle);

	// 3Dポーションとデバッグカメラを連動
	model->Draw(worldTransform, debugCamera->GetViewProjection(), textureHandle);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	/*------------
	  ラインを描画
	------------*/

	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
