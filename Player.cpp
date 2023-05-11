#include "Player.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインタチェック
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.Initialize();
}
void Player::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}
void Player::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
