#include "EnemyState.h"
#include "Input.h"
#include "Model.h"
#include "enemy/enemy.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "ImGuiManager.h"
#include <cassert>



void EnemyStateApproach::Update(Enemy* pEnemy) {
	pEnemy->Enemy::SetVelocity(0, 0, 1);
	// 移動 (ベクトルを加算)
	pEnemy->Move();
	// 既定の位置に達したら離脱
	if (pEnemy->GetWorldTransform().z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave());
	}
}

void EnemyStateLeave::Update(Enemy* pEnemy) {
	pEnemy->SetVelocity(0.5, 0.5, 0);

	// 移動 (ベクトルを加算)
	pEnemy->Move();
}