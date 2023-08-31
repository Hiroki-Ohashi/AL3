#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include <list>
#include "Input.h"
#include "PlayerBullet.h"
#include "MathFunction.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>
#include <Sprite.h>


class Player {
public:
	/// <summary>
	/// 初期化
	/// </ summary>
	void Initialize(Model* model, uint32_t textureHandle, Vector3 position);
	~Player();
	/// <summary>
	/// 初期化
	/// </ summary>
	void Update();

	/// <summary>
	/// 初期化
	/// </ summary>
	void Draw(ViewProjection);

	void DrawUI();

	void Attack();

	void OnCollision();

	void SetParent(const WorldTransform* parent);

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	Vector3 Get3DWorldPosition();

	bool IsDead() const { return isDead_; }

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

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
	std::list<PlayerBullet*> bullets_;
	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	// 2Dレティクル用スプライト
	Sprite* sprite2DReicle_ = nullptr;

	ViewProjection viewProjection;

		// デスフラグ
	bool isDead_ = false;
};
