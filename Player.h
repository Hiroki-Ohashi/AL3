#pragma once
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "WorldTransform.h"
#include <cassert>

class Player {
public:
	/// <summary>
	/// 初期化
	/// </ summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 初期化
	/// </ summary>
	void Update();

	/// <summary>
	/// 初期化
	/// </ summary>
	void Draw(ViewProjection);

	void Attack();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;
	// 弾
	PlayerBullet* bullet_ = nullptr;
};