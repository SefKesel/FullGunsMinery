#include "Bala.h"
#include <cmath>

Bala::Bala() {
    activa = false;
    velocidad = 6.0f;
    direccion = {0, -1};
    color = YELLOW;
}

void Bala::Disparar(Vector2 pos, Vector2 dir, Color c) {
    posicion = pos;
    color = c;

    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (len != 0) {
        direccion = {dir.x / len, dir.y / len};
    }

    activa = true;
}

void Bala::Update() {
    if (!activa) return;

    posicion.x += direccion.x * velocidad;
    posicion.y += direccion.y * velocidad;

    if (posicion.y < -10 || posicion.y > GetScreenHeight() + 10 ||
        posicion.x < -10 || posicion.x > GetScreenWidth() + 10) {
        activa = false;
    }
}

void Bala::Draw() {
    if (activa)
        DrawCircleV(posicion, 4, color);
}

Vector2 Bala::GetPosition() const {
    return posicion;
}

float Bala::GetRadius() const {
    return 4.0f;
}
