#include "enemy/enemy.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Model.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "enemy/enemyBullet.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_ = {0, 5, 100};

	// 弾を発射
	Attack();

	// フェーズ初期化
	ApproachInitialize();

	state = new EnemyStateApproach();
}

Enemy::~Enemy() {
	// bullet_の解放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Move() {
	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z -= velocity_.z;
}

//// staticで宣言したメンバ関数ポインタテーブルの実態
// void (Enemy::*Enemy::phasePFuncTable[])() = {&Enemy::ApproachUpdate, &Enemy::LeaveUpdate};

void Enemy::Update() {
	state->Update(this);

	// デスフラグんお立った弾を排除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	attackTimer--;

	if (attackTimer <= 0) {

		// 攻撃処理
		Attack();

		// 発射タイマーを初期化
		attackTimer = kFireInterval;
	}

	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

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
	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Enemy::SetVelocity(float x, float y, float z) {
	velocity_.x = x;
	velocity_.y = y;
	velocity_.z = z;
}

void Enemy::Attack() {
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録
	bullets_.push_back(newBullet);
}

void Enemy::ApproachInitialize() { attackTimer = 0; }

void Enemy::ChangeState(BaseEnemyState* newState) { state = newState; }

void EnemyStateApproach::Update(Enemy* pEnemy) {
	pEnemy->SetVelocity(0, 0, 0.5);
	// 移動 (ベクトルを加算)
	
	pEnemy->Move();
	// 既定の位置に達したら離脱
	if (pEnemy->GetWorldTransform().z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave());
		pEnemy->ApproachInitialize();
	}
}

void EnemyStateLeave::Update(Enemy* pEnemy) {
	pEnemy->SetVelocity(0.5, 0.5, 0);
	// 移動 (ベクトルを加算)
	pEnemy->Move();
}
