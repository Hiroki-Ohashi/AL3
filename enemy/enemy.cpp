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

void Enemy::ApproachUpdate() {
	// 移動 (ベクトルを加算)
	worldTransform_.translation_.z -= velocity_.z;
	// 既定の位置に達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate() {
	// 移動 (ベクトルを加算)
	if (phase_ == Phase::Leave) {
		worldTransform_.translation_.y += velocity_.y;
		worldTransform_.translation_.x -= velocity_.x;
	}
}

// staticで宣言したメンバ関数ポインタテーブルの実態
void (Enemy::*Enemy::phasePFuncTable[])() = {&Enemy::ApproachUpdate, &Enemy::LeaveUpdate};

void Enemy::Update() {

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	// メンバ関数ポインタに入っている関数を呼び出す
	(this->*phasePFuncTable[static_cast<size_t>(phase_)])();

	/*switch (phase_) { 
	case Phase::Approach:
	default:
		ApproachUpdate();
		break;
	case Phase::Leave:
		LeaveUpdate();
		break;
	}*/
}

void Enemy::Draw(const ViewProjection& viewProjection_) {
	// モデル描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}