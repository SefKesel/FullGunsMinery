#pragma once
#include "raylib.h"
#include "Collider.h"

enum class PowerUpType {
    FIRE_RATE
};

class PowerUp : public Collider {
public:
    Vector2 posicion;
    bool activa;
    PowerUpType type;
    float multiplier;
    float duration;
    float radius;
    float vy; // velocidad vertical (px/s)
    Color color;

    PowerUp(PowerUpType t, Vector2 pos, float mult, float dur, float initialVy = 40.0f);

    void Update();
    void Draw();

    Vector2 GetPosition() const override;
    float GetRadius() const override;
};
