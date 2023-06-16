#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include "Input.h"

class Enemy;

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
	void Initialize(Model* model, uint32_t textureHandle);

	void Move();

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void ChangeState(BaseEnemyState* newState);

	void SetVelocity(float x, float y, float z);

	//// 行動フェーズ
	// enum class Phase {
	//	Approach, // 接近する
	//	Leave,    // 離脱する
	// };

	Vector3 GetWorldTransform() { return worldTransform_.translation_; }
	Vector3 GetVelocity() { return velocity_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 速度
	Vector3 velocity_;

	//// フェーズ
	// Phase phase_ = Phase::Approach;
	//// メンバ関数ポインタのテーブル
	// static void (Enemy::*phasePFuncTable[])();

	// ステート
	BaseEnemyState* state;
};