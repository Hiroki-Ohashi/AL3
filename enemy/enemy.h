#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include "Input.h"

class Enemy {
public:
	void Initialize(Model* model, uint32_t textureHandle);

	void ApproachUpdate();

	void LeaveUpdate();

	void Update();

	void Draw(const ViewProjection& viewProjection);

	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_ = {0.5, 0.5, 1};
	// フェーズ
	Phase phase_ = Phase::Approach;
	// メンバ関数ポインタのテーブル
	static void (Enemy::*phasePFuncTable[])();
};