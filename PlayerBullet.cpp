#include "PlayerBullet.h"
#include "Input.h"
#include "Model.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position) {

	// NULLポインタチェック
	assert(model);
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("tama.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() {

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	// モデル描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}