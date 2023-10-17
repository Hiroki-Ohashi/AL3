#pragma once

#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

class Block {
public:
	void Initialize(Model* model, uint32_t textureHandle, Vector3 translation);
	~Block();

	void Move();

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision();

	void SetType(float type) { type_ = type; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();
	Vector3 GetWorldTransform() { return worldTransform_.translation_; }
	float GetType() { return type_; }

private:
	// ワールド変換データ0
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 速度
	Vector3 velocity_;
	// ブロックのタイプ
	// 下が0、上が1、左右が2
	float type_ = 0;
};
