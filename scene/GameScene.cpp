#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>
#include <imgui.h>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete player_;
	delete debugCamera_;
	delete skyDome_;
	delete modelSkydome_;
	delete railCamera_;

	// bullet_の解放
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}

	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 10000.0f;
	viewProjection_.Initialize();

	playerTh_ = TextureManager::Load("Red.png");

	enemyTh_ = TextureManager::Load("player.png");

	// 3Dモデルの生成
	model_ = Model::Create();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// レールカメラの生成
	railCamera_ = new RailCamera();
	// レールカメラの初期化
	railCamera_->Initialize({0, 0, -20}, {0, 0, 0});

	// 自キャラの生成
	player_ = new Player();
	Vector3 playerPosition(0, 0, 0);
	// 自キャラの初期化
	player_->Initialize(model_, playerTh_, playerPosition);
	// 自キャラとレールカメラの親子関係を結ぶ
	//player_->SetParent(&railCamera_->GetWorldTransform());

	LoadEnemyPopData();

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("tenkyu", true);

	// 天球の生成
	skyDome_ = new Skydome();
	// 天球の初期化
	skyDome_->Initialize(modelSkydome_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1260, 700);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {

	// 天球の更新
	skyDome_->Update();

	// 自キャラの更新
	player_->Update();

	UpdateEnemyPopCommands();

	

	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	
	// 弾更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	// デスフラグんお立った弾を排除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	CheckAllCollisions();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN)) {
		isDebugCameraActive_ = true;
	}
#endif

	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->SetFarZ(100000.0f);
		// デバッグカメラの更新
		debugCamera_->Update();

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		// viewProjection_.UpdateMatrix();

		// レールカメラの更新
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
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

	// 天球の描画
	skyDome_->Draw(viewProjection_);

	// 自キャラの描画
	player_->Draw(viewProjection_);

	// 敵キャラの描画
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	// 弾描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
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

 void GameScene::CheckAllCollisions() {
	// 判定衝突AとBの座標
	Vector3 posA, posB;

 #pragma region 自キャラと敵の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵すべての当たり判定
	for (Enemy* enemy : enemys_) {
		// 敵の座標
		posB = enemy->GetWorldPosition();

		float p2eX = (posB.x - posA.x) * (posB.x - posA.x);
		float p2eY = (posB.y - posA.y) * (posB.y - posA.y);
		float p2eZ = (posB.z - posA.z) * (posB.z - posA.z);

		float pRadius = 1;
		float eRadius = 1;
 
		float L = (pRadius + eRadius) * (pRadius + eRadius);



		if (p2eX + p2eY + p2eZ <= L) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
		}
	}

 #pragma endregion
 }

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルも内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

 void GameScene::UpdateEnemyPopCommands() {

	 // 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			getline(line_stream, word, ',');
			posX = (float)std::atof(word.c_str());

			// Y座標
			getline(line_stream, word, ',');
			posY = (float)std::atof(word.c_str());

			// Z座標
			getline(line_stream, word, ',');
			posZ = (float)std::atof(word.c_str());
		}

		// SCALEコマンド
		if (word.find("SCALE") == 0) {
			// scaleX
			getline(line_stream, word, ',');
			float scaleX = (float)std::atof(word.c_str());

			// scaleY
			getline(line_stream, word, ',');
			float scaleY = (float)std::atof(word.c_str());

			// scaleZ
			getline(line_stream, word, ',');
			float scaleZ = (float)std::atof(word.c_str());

			// 敵を発生させる
			EnemySpown(Vector3(posX, posY, posZ), Vector3(scaleX, scaleY, scaleZ));

			break;
		}
	}
 }

void GameScene::EnemySpown(Vector3 translation, Vector3 scale) {
	// 敵キャラの生成
	Enemy* enemy_ = new Enemy();
	// 敵キャラの初期化
	enemy_->Initialize(model_, enemyTh_, translation);
	// 敵キャラにゲームシーンを渡す
	enemy_->SetGameScene(this);
	enemy_->SetScale(scale);
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	AddEnemy(enemy_);
}

void GameScene::AddEnemy(Enemy* enemy) {
	// リストに登録する
	enemys_.push_back(enemy);
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}
