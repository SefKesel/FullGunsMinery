#pragma once
#include "Enemy.h"

class EnemyNormal : public Enemy {
public:
    EnemyNormal(Texture2D tex);

    void Update() override;
    void Draw() override;
    float GetRadius() const override;
    void Reset() override;

    int GetScoreValue() const override { return 50; }
};
