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

	// ビュープロジェクションの初期化

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	playerTh_ = TextureManager::Load("player.png");

	enemyTh_ = TextureManager::Load("white1x1.png");

	// 3Dモデルの生成
	model_ = Model::Create();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// レールカメラの生成
	railCamera_ = new RailCamera();
	// レールカメラの初期化
	railCamera_->Initialize({0, 0, 0}, {0, 0, 0});

	// レティクルのテクスチャ
	TextureManager::Load("Reticle.png");

	// 自キャラの生成
	player_ = new Player();
	Vector3 playerPosition(0, 0, 30);
	// 自キャラの初期化
	player_->Initialize(model_, playerTh_, playerPosition);
	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

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

	if (input_->PushKey(DIK_A)) {
		
	}

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

	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
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
	if (player_->IsDead() == false) {
		player_->Draw(viewProjection_);
	}

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

	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

 void GameScene::CheckAllCollisions() {
	// 判定衝突AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;

 #pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		float p2eBX = (posB.x - posA.x) * (posB.x - posA.x);
		float p2eBY = (posB.y - posA.y) * (posB.y - posA.y);
		float p2eBZ = (posB.z - posA.z) * (posB.z - posA.z);

		float pRadius = 1;
		float eBRadius = 1;

		float L = (pRadius + eBRadius) * (pRadius + eBRadius);

		if (p2eBX + p2eBY + p2eBZ <= L) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

 #pragma endregion

 #pragma region 自弾と敵キャラの当たり判定
	// 敵キャラと自弾すべての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		for (Enemy* enemy : enemys_) {

			// 敵キャラの座標
			posA = enemy->GetWorldPosition();

			// 自弾の座標
			posB = bullet->GetWorldPosition();

			float e2pBX = (posB.x - posA.x) * (posB.x - posA.x);
			float e2pBY = (posB.y - posA.y) * (posB.y - posA.y);
			float e2pBZ = (posB.z - posA.z) * (posB.z - posA.z);

			const int eRadius = 1;
			const int pBRadius = 1;

			float L = (eRadius + pBRadius);

			if (e2pBX + e2pBY + e2pBZ <= L) {
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
 #pragma endregion

 #pragma region 自弾と敵弾の当たり判定
	// 敵弾と自弾すべての当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		for (EnemyBullet* enemyBullet : enemyBullets) {
			// 敵弾の座標
			posA = playerBullet->GetWorldPosition();
			// 自弾の座標
			posB = enemyBullet->GetWorldPosition();

			float pB2eBX = (posB.x - posA.x) * (posB.x - posA.x);
			float pB2eBY = (posB.y - posA.y) * (posB.y - posA.y);
			float pB2eBZ = (posB.z - posA.z) * (posB.z - posA.z);

			float pBRadius = 1;
			float eBRadius = 1;

			float L = (pBRadius + eBRadius) * (pBRadius + eBRadius);

			if (pB2eBX + pB2eBY + pB2eBZ <= L) {
				// 自弾の衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
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

	 // 待機処理
	if (isWait_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			// 待機完了
			isWait_ = false;
		}
		return;
	}

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
			float x = (float)std::atof(word.c_str());

			// Y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// Z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			EnemySpown(Vector3(x, y, z));
		}

		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWait_ = true;
			waitTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
 }

void GameScene::EnemySpown(Vector3 translation) {
	// 敵キャラの生成
	Enemy* enemy_ = new Enemy();
	// 敵キャラの初期化
	enemy_->Initialize(model_, enemyTh_, translation);
	// 敵キャラにゲームシーンを渡す
	enemy_->SetGameScene(this);
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
