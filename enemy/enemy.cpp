#include "Input.h"
#include "Model.h"
#include "enemy/enemy.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "ImGuiManager.h"
#include <cassert>
#include "enemy/BaseEnemyState.h"
#include "EnemyStateApproch.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_ = {0, 5, 100};

	state = new EnemyStateApproach();
}

void Enemy::Move() { 
	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z -= velocity_.z;
}

//// staticで宣言したメンバ関数ポインタテーブルの実態
//void (Enemy::*Enemy::phasePFuncTable[])() = {&Enemy::ApproachUpdate, &Enemy::LeaveUpdate};

void Enemy::Update() {
	state->Update(this);

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//// メンバ関数ポインタに入っている関数を呼び出す
	//(this->*phasePFuncTable[0])();
	//(this->*phasePFuncTable[1])();

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

void Enemy::SetVelocity(float x, float y, float z) { 
	velocity_.x = x;
	velocity_.y = y;
	velocity_.z = z;
}

void Enemy::ChangeState(BaseEnemyState* newState) { state = newState; }
