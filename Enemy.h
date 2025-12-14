#pragma once
#include "Entity.h"
#include "raylib.h"

class Enemy : public Entity {
protected:
    Texture2D textura;
    float velocidad;
    float rotacion;
    float rotSpeed;
    float escala;

public:
    Enemy(Texture2D tex);
    virtual void Reset() = 0;
    virtual int GetScoreValue() const = 0;
};
