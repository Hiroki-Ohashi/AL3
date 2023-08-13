#pragma once
#include "BaseEnemyState.h"

class Enemy;

class EnemyStateLeave : public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);
};
