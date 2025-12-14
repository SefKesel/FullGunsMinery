#pragma once
#include "raylib.h"

#include "GameState.h"
#include "Nave.h"
#include "Enemy.h"
#include "EnemyFast.h"
#include "EnemyZigZag.h"
#include "EnemyShip.h"

#include <memory>
#include <vector>

class Game {
public:
    Game();
    ~Game();

    void Update();
    void Draw();

private:
    GameState state;

    // Texturas
    Texture2D naveTex;

    Texture2D texAstNormal;
    Texture2D texAstFast;
    Texture2D texAstZigZag;

    Texture2D enemyShipTex;

    // Entidades
    std::unique_ptr<Nave> nave;
    std::vector<std::unique_ptr<Enemy>> enemigos;
    std::vector<std::unique_ptr<EnemyShip>> enemyShips;
    bool debugHitbox;
    
    // Score y progresión
    int score;
    int nextExtraLifeScore;
    int nextEnemyShipScore;
    int highscore;

    // Métodos internos
    void Reset();

    void UpdateMenu();
    void UpdatePlaying();
    void UpdateGameOver();

    void DrawMenu();
    void DrawPlaying();
    void DrawGameOver();
    // Highscore file IO
    void LoadHighScore();
    void SaveHighScore();
};
