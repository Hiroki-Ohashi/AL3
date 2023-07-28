#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) { 

	// NULLポインタチェック
	assert(model);
	model_ = model;

	worldTransform_.Initialize(); 
	worldTransform_.translation_ = {0, 0, 0};
	worldTransform_.scale_ = {1000, 1000, 1000};

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

}

void Skydome::Update() { worldTransform_.TransferMatrix(); }

void Skydome::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);
}
