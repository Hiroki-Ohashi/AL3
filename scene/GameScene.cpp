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

	for (Block* block : blocks_) {
		delete block;
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

	damageTh_ = TextureManager::Load("daruma2.png");

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
	Vector3 playerPosition(-36, 20, 0);
	// 自キャラの初期化
	player_->Initialize(model_, playerTh_, playerPosition);
	
	LoadBlockPopData();

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

	UpdateBlockPopCommands();


	for (Block* block : blocks_) {
		block->Update();
	}	

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

	for (Block* block : blocks_) {
		block->Draw(viewProjection_);
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

 #pragma region 自キャラとブロックの当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラとブロックすべての当たり判定
	for (Block* block : blocks_) {
		// ブロックの座標
		posB = block->GetWorldPosition();

		/*float p2eX = (posB.x - posA.x) * (posB.x - posA.x);
		float p2eY = (posB.y - posA.y) * (posB.y - posA.y);
		float p2eZ = (posB.z - posA.z) * (posB.z - posA.z);

		float pRadius = 1.0f;
		float eRadius = 1.0f;

		float L = (pRadius + eRadius) * (pRadius + eRadius);

		if (p2eX + p2eY + p2eZ <= L) {
		    if (block->GetType() == 1) {
		        player_->OnCollisionY();
		    }

		    if (block->GetType() == 2) {
		        player_->OnCollisionX();
		    }
		}*/

		float leftA = posA.x;
		float rightA = posA.x + 2.0f;
		float topA = posA.y;
		float bottomA = posA.y + 2.0f;
		float nearA = posA.z;
		float farA = posA.z + 2.0f;

		float leftB = posB.x;
		float rightB = posB.x + 2.0f;
		float topB = posB.y;
		float bottomB = posB.y + 2.0f;
		float nearB = posB.z;
		float farB = posB.z + 2.0f;

		if (leftA <= rightB && leftB <= rightA &&
			topA <= bottomB && topB <= bottomA &&
			nearA <= farB && nearB <= farA) {

			if (block->GetType() == 1) {
				player_->OnCollisionY();
			}

			if (block->GetType() == 2) {
				player_->OnCollisionX();
			}
		}
	}

 #pragma endregion
 }


void GameScene::LoadBlockPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/blockPop.csv");
	assert(file.is_open());

	// ファイルも内容を文字列ストリームにコピー
	blockPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateBlockPopCommands() {

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(blockPopCommands, line)) {
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
			pos2X = (float)std::atof(word.c_str());

			// Y座標
			getline(line_stream, word, ',');
			pos2Y = (float)std::atof(word.c_str());

			// Z座標
			getline(line_stream, word, ',');
			pos2Z = (float)std::atof(word.c_str());
		}

		// TYPEコマンド
		if (word.find("TYPE") == 0) {
			// type
			getline(line_stream, word, ',');
			float type = (float)std::atof(word.c_str());

			// ブロックを発生させる
			BlockSpown(Vector3(pos2X, pos2Y, pos2Z), type);

			break;
		}
	}
}

void GameScene::BlockSpown(Vector3 translation, float type) {
	// ブロックの生成
	Block* block_ = new Block();
	// ブロックの初期化
	if (type < 3) {
		block_->Initialize(model_, enemyTh_, translation);
	}
	// ブロックのタイプ設定
	block_->SetType(type);
	AddBlock(block_);
}

void GameScene::AddBlock(Block* block) {
	// リストに登録する
	blocks_.push_back(block);
}