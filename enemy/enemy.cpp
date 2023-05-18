#include "Input.h"
#include "Model.h"
#include "enemy/enemy.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_ = {0, 5, 100};
}

void Enemy::Update() {

	// 
	worldTransform_.translation_.z -= velocity_.z;

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection_) {
	// モデル描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}