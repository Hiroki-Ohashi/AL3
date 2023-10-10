#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include "Input.h"
#include "enemy/enemyBullet.h"

// 自機クラスの前方宣言

class Enemy;
class Player;
class GameScene;

class BaseEnemyState {
public:
	virtual void Update(Enemy* pEnemy) = 0;
};

class EnemyStateApproach : public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);
};

class EnemyStateLeave : public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);
};

class Enemy {
public:
	void Initialize(Model* model, uint32_t textureHandle, Vector3 translation);
	~Enemy();

	void Move();

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void ChangeState(BaseEnemyState* newState);

	void Attack();

	void ApproachInitialize();

	void OnCollision();


	void SetVelocity(float x, float y, float z);

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
	Vector3 GetVelocity() { return velocity_; }
	static const int kFireInterval = 60;

private:
	// ワールド変換データ0
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 速度
	Vector3 velocity_;
	// 発射タイマー
	int32_t attackTimer = 10;
	// 自キャラ
	Player* player_ = nullptr;
	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	//// フェーズ
	// Phase phase_ = Phase::Approach;
	//// メンバ関数ポインタのテーブル
	// static void (Enemy::*phasePFuncTable[])();

	// ステート
	BaseEnemyState* state;
};