#include "Input.h"
#include "Model.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "enemy/enemyBullet.h"
#include "player/Player.h"
#include <cassert>
#include <cmath>
#include <iostream>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	// NULLポインタチェック
	assert(model);
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("Red.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	// z方向に伸びた形状
	worldTransform_.scale_.x = 0.3f;
	worldTransform_.scale_.y = 0.3f;
	worldTransform_.scale_.z = 1.0f;

	// Y軸周り角度（Θy）
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	float velocityXZ = sqrt((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));

	worldTransform_.rotation_.x = std::atan2(-velocity_.y, velocityXZ);

	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Update() {
	assert(player_);

	// 敵弾から自キャラへのベクトル計算
	Vector3 toPlayer;
	toPlayer.x = player_->GetWorldPosition().x - GetWorldPosition().x;
	toPlayer.y = player_->GetWorldPosition().y - GetWorldPosition().y;
	toPlayer.z = player_->GetWorldPosition().z - GetWorldPosition().z;

	float t = 0.001f;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = SLerp(
	    toPlayer,
	    {worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
	     worldTransform_.matWorld_.m[3][2]},
	    t);

	velocity_.x *= 0.7f;
	velocity_.y *= 0.7f;
	velocity_.z *= 0.7f;

	// Y軸周り角度（Θy）
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float velocityXZ = sqrt((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));

	worldTransform_.rotation_.x = std::atan2(-velocity_.y, velocityXZ);

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;


	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	// モデル描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision() { isDead_ = true; }

Vector3 EnemyBullet::GetWorldPosition() { 
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void EnemyBullet::SetPlayer(Player* player) { player_ = player; }