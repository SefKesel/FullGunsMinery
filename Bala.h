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

    Bala();

    void Disparar(Vector2 pos, Vector2 dir, Color c);
    void Update();
    void Draw();

    Vector2 GetPosition() const override;
    float GetRadius() const override;
};
