#include "EnemyFast.h"

EnemyFast::EnemyFast(Texture2D tex) : Enemy(tex) {
    Reset();
}

void EnemyFast::Reset() {
    posicion = {(float)GetRandomValue(50, 750), -50};
    velocidad = GetRandomValue(350, 500) / 100.0f; // rápido
    rotacion = 0.8f;
}

void EnemyFast::Update() {
    posicion.y += velocidad;
    rotacion += rotSpeed;

    if (posicion.y > GetScreenHeight())
        Reset();
}

void EnemyFast::Draw() {
    DrawTexturePro(
        textura,
        {0, 0, (float)textura.width, (float)textura.height},
        {posicion.x,
         posicion.y,
         textura.width * escala,
         textura.height * escala},
        { (textura.width * escala) / 2.0f,
         (textura.height * escala) / 2.0f},
        rotacion,
        WHITE
    );
}

float EnemyFast::GetRadius() const {
    return (textura.width * escala) / 2.0f;
}
