#include "enemy/enemy.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Model.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "enemy/enemyBullet.h"
#include <cassert>
#include <player/Player.h>
#include "GameScene.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle, Vector3 translation) {
	// NULLポインタチェック
	assert(model);
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_ = translation;

	//// 弾を発射
	//Attack();

	// フェーズ初期化
	ApproachInitialize();

	state = new EnemyStateApproach();


}

Enemy::~Enemy() {
}

void Enemy::Move() {
	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.y -= velocity_.y;
	worldTransform_.translation_.z -= velocity_.z;
}

//// staticで宣言したメンバ関数ポインタテーブルの実態
// void (Enemy::*Enemy::phasePFuncTable[])() = {&Enemy::ApproachUpdate, &Enemy::LeaveUpdate};

void Enemy::Update() {
	//state->Update(this);

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//attackTimer--;

	//if (attackTimer <= 0) {

	//	// 攻撃処理
	//	Attack();

	//	// 発射タイマーを初期化
	//	attackTimer = kFireInterval;
	//}

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

void Enemy::Attack() {

	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 1.0f;
	
	Vector3 start = GetWorldPosition();
	Vector3 end = player_->GetWorldPosition();
	Vector3 diff;

	diff.x = end.x - start.x;
	diff.y = end.y - start.y;
	diff.z = end.z - start.z;

	diff = Normalize(diff);

	diff.x *= kBulletSpeed;
	diff.y *= kBulletSpeed;
	diff.z *= kBulletSpeed;
	
	Vector3 velocity(diff.x, diff.y, diff.z);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::ApproachInitialize() { attackTimer = 0; }

void Enemy::OnCollision() {}

Vector3 Enemy::GetWorldPosition() { 
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::ChangeState(BaseEnemyState* newState) { state = newState; }

void EnemyStateApproach::Update(Enemy* pEnemy) {
	pEnemy->SetVelocity(0, 0, 0.1f);
	// 移動 (ベクトルを加算)
	
	pEnemy->Move();
	// 既定の位置に達したら離脱
	if (pEnemy->GetWorldTransform().z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave());
		pEnemy->ApproachInitialize();
	}
}

void EnemyStateLeave::Update(Enemy* pEnemy) {
	pEnemy->SetVelocity(0.1f, -0.1f, 0);
	// 移動 (ベクトルを加算)
	pEnemy->Move();
}

void  Enemy::SetPlayer(Player* player) { player_ = player; }