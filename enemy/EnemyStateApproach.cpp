#include "BaseEnemyState.h"
#include "EnemyStateApproch.h"
#include "enemy/enemy.h"
#include "EnemyStateLeave.h"

void EnemyStateApproach::Update(Enemy* pEnemy) {
	pEnemy->Enemy::SetVelocity(0, 0, 1);
	// 移動 (ベクトルを加算)
	pEnemy->Move();
	// 既定の位置に達したら離脱
	if (pEnemy->GetWorldTransform().z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave());
	}
}