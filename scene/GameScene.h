#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <sstream>

#include "RailCamera.h"
#include "Skydome.h"
#include "player/Player.h"
#include "Block.h"

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

	/// <summary>
	///	衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>

	/// <summary>
	/// ブロックデータの読み込み
	/// </summary>
	void LoadBlockPopData();

	/// <summary>
	/// ブロック発生コマンドの更新
	/// </summary>
	void UpdateBlockPopCommands();

	void BlockSpown(Vector3, float);

	/// <summary>
	///	敵:敵弾を追加する
	/// </summary>
	void AddBlock(Block* block);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// テクスチャハンドル
	uint32_t playerTh_;
	uint32_t enemyTh_;
	uint32_t damageTh_;

	// スプライト
	Sprite* sprite_ = nullptr;

	// 3Dモデルデータ
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 自キャラ
	Player* player_ = nullptr;

	// ブロック
	std::list<Block*> blocks_;

	// 天球
	Skydome* skyDome_ = nullptr;
	// 3Dモデル
	Model* modelSkydome_ = nullptr;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	// 待機タイマー
	int32_t waitTimer_;
	// 待機フラグ
	bool isWait_ = false;

	float posX;
	float posY;
	float posZ;

	float pos2X;
	float pos2Y;
	float pos2Z;

	// 敵発生コマンド
	std::stringstream blockPopCommands;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
