#include "PowerUp.h"
#include <cmath>
#include <math.h>


PowerUp::PowerUp(PowerUpType t, Vector2 pos, float mult, float dur, float initialVy)
    : posicion(pos), activa(true), type(t), multiplier(mult), duration(dur), radius(12.0f), vy(initialVy) {
    switch (type) {
        case PowerUpType::FIRE_RATE: color = GOLD; break;
        default: color = GOLD; break;
    }
}

void PowerUp::Update() {
    if (!activa) return;
    float dt = GetFrameTime();
    posicion.y += vy * dt;

    // Desactivarse si sale de la pantalla por abajo
    if (posicion.y - radius > GetScreenHeight()) {
        activa = false;
    }
}

void PowerUp::Draw() {
    if (!activa) return;
    // Dibujar una estrella de 5 puntas rellena en color dorado
    const int points = 5;
    const int total = points * 2;
    Vector2 verts[10];
    const float pi = 3.14159265358979323846f;
    float outer = radius;
    float inner = radius * 0.5f;
    for (int i = 0; i < total; ++i) {
        float angle = (float)i * (2.0f * pi / (float)total) - pi/2.0f; // empezar apuntando hacia arriba
        float r = (i % 2 == 0) ? outer : inner;
        verts[i].x = posicion.x + (float)cos(angle) * r;
        verts[i].y = posicion.y + (float)sin(angle) * r;
    }

    // Rellenar la estrella triangulando con el centro
    for (int i = 0; i < total; ++i) {
        int j = (i + 1) % total;
        DrawTriangle(posicion, verts[i], verts[j], color);
    }

    // Contorno blanco para resaltar
    for (int i = 0; i < total; ++i) {
        int j = (i + 1) % total;
        DrawLineV(verts[i], verts[j], WHITE);
    }
}

Vector2 PowerUp::GetPosition() const {
    return posicion;
}

float PowerUp::GetRadius() const {
    return radius;
}
