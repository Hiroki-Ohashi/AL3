#include "BaseEnemyState.h"
#include "EnemyStateLeave.h"
#include "enemy/enemy.h"

void EnemyStateLeave::Update(Enemy* pEnemy) {
	pEnemy->SetVelocity(0.5, 0.5, 0);

	// 移動 (ベクトルを加算)
	pEnemy->Move();
}