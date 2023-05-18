#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include "Input.h"

class Enemy {
public : 
	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(const ViewProjection& viewProjection);


private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_ = {0, 0, 1};
};