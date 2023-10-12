#pragma once

#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

class Player;
class GameScene;

class Block {
public:
	void Initialize(Model* model, uint32_t textureHandle, Vector3 translation);
	~Block();

	void Move();

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision();

	void SetPlayer(Player* player);

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	void SetScale(Vector3 scale) { worldTransform_.scale_ = scale; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	//// 行動フェーズ
	// enum class Phase {
	//	Approach, // 接近する
	//	Leave,    // 離脱する
	// };

	Vector3 GetWorldTransform() { return worldTransform_.translation_; }

private:
	// ワールド変換データ0
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 自キャラ
	Player* player_ = nullptr;
	// ゲームシーン
	GameScene* gameScene_ = nullptr;
	// 速度
	Vector3 velocity_;
	//// フェーズ
	// Phase phase_ = Phase::Approach;
	//// メンバ関数ポインタのテーブル
	// static void (Enemy::*phasePFuncTable[])();
};
