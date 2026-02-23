#pragma once
#include "raylib.h"
#include "Collider.h"

class Bala : public Collider {
public:
    Vector2 posicion;
    Vector2 direccion;
    float velocidad;
    bool activa;
    Color color;
    float radius;
    float baseRadius;

    Bala();

    void Disparar(Vector2 pos, Vector2 dir, Color c, float radiusMultiplier = 1.0f);
    void Update();
    void Draw();

    Vector2 GetPosition() const override;
    float GetRadius() const override;
};
