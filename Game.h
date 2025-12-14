#pragma once
#include "raylib.h"

#include "GameState.h"
#include "Nave.h"
#include "Enemy.h"
#include "EnemyFast.h"
#include "EnemyZigZag.h"
#include "EnemyShip.h"
#include "EnemyNormal.h"

#include <memory>
#include <vector>
#include <string>

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
    Texture2D backgroundTex;
    float bgOffset;
    float bgSpeed;

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
    std::string highscoreName;

    // Nombre para nuevo highscore
    int pendingHighscore;
    std::string currentName;
    int namePos; // 0..2

    // Métodos internos
    void Reset();

    void UpdateMenu();
    void UpdatePlaying();
    void UpdateGameOver();

    void DrawMenu();
    void DrawPlaying();
    void DrawGameOver();
    void DrawEnterName();
    void DrawBackground();
    void UpdateEnterName();
    void DrawEnterName();
    // Highscore file IO
    void LoadHighScore();
    void SaveHighScore();
};
