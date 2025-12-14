#include "EnemyNormal.h"

EnemyNormal::EnemyNormal(Texture2D tex) : Enemy(tex) {
    Reset();
}

void EnemyNormal::Reset() {
    posicion = {(float)GetRandomValue(50, 750), -50};
    velocidad = GetRandomValue(150, 250) / 100.0f; // velocidad normal
    rotacion = 0.0f;
}

void EnemyNormal::Update() {
    posicion.y += velocidad;

    // ligera rotación
    rotacion += rotSpeed * 0.5f;

    if (posicion.y > GetScreenHeight())
        Reset();
}

void EnemyNormal::Draw() {
    DrawTexturePro(
        textura,
        {0, 0, (float)textura.width, (float)textura.height},
        {posicion.x,
         posicion.y,
         textura.width * escala,
         textura.height * escala},
        {(textura.width * escala) / 2.0f,
         (textura.height * escala) / 2.0f},
        rotacion,
        WHITE
    );
}

float EnemyNormal::GetRadius() const {
    return (textura.width * escala) / 2.0f;
}
