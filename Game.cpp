#include "Game.h"
#include <cstdlib>
#include <cstdio>

Game::Game() {
    naveTex = LoadTexture("cohete.png");

    texAstNormal = LoadTexture("asteroid_normal.png");
    texAstFast   = LoadTexture("asteroid_fast.png");
    texAstZigZag = LoadTexture("asteroid_zigzag.png");
    enemyShipTex = LoadTexture("enemyShip.png");
    
    state = GameState::MENU;
    debugHitbox = false;
    highscore = 0;
    LoadHighScore();
    Reset();
}

Game::~Game() {
    UnloadTexture(naveTex);

    UnloadTexture(texAstNormal);
    UnloadTexture(texAstFast);
    UnloadTexture(texAstZigZag);

    UnloadTexture(enemyShipTex);
}

void Game::Reset() {
    score = 0;
    nextExtraLifeScore = 10000;
    nextEnemyShipScore = 2000;

    nave = std::make_unique<Nave>(naveTex);

    enemigos.clear();
    enemyShips.clear();

    // Normales
    for (int i = 0; i < 2; i++)
        enemigos.push_back(std::make_unique<EnemyZigZag>(texAstNormal));

    // Fast
    for (int i = 0; i < 2; i++)
        enemigos.push_back(std::make_unique<EnemyFast>(texAstFast));

    // ZigZag
    for (int i = 0; i < 2; i++)
        enemigos.push_back(std::make_unique<EnemyZigZag>(texAstZigZag));

}

void Game::Update() {
    switch (state) {
        case GameState::MENU:     UpdateMenu();     break;
        case GameState::PLAYING:  UpdatePlaying();  break;
        case GameState::GAMEOVER: UpdateGameOver(); break;
    }
}

void Game::Draw() {
    switch (state) {
        case GameState::MENU:     DrawMenu();     break;
        case GameState::PLAYING:  DrawPlaying();  break;
        case GameState::GAMEOVER: DrawGameOver(); break;
    }
}

void Game::UpdateMenu() {
    if (IsKeyPressed(KEY_ENTER)) {
        Reset();
        state = GameState::PLAYING;
    }
}

void Game::UpdatePlaying() {
    if (IsKeyPressed(KEY_F1))
        debugHitbox = !debugHitbox;

    nave->Update();

    for (auto& e : enemigos)
        e->Update();

    // Actualizar naves enemigas
    for (auto& ship : enemyShips) {
        ship->SetPlayerPosition(nave->posicion);
        ship->Update();
    }

    // Colisión nave vs asteroides
    for (auto& e : enemigos) {
        if (nave->CheckCollision(e.get())) {
            nave->vida--;
            e->Reset();

            if (nave->vida <= 0)
                state = GameState::GAMEOVER;
        }
    }

    // Balas jugador vs asteroides
    for (auto& b : nave->balas) {
        if (!b.activa) continue;

        for (auto& e : enemigos) {
            if (b.CheckCollision(e.get())) {
                b.activa = false;
                e->Reset();
                score += 100;

                if (score > highscore) {
                    highscore = score;
                    SaveHighScore();
                }

                if (score >= nextExtraLifeScore) {
                    nave->vida++;
                    nextExtraLifeScore += 1000;
                }
                break;
            }
        }
    }

    // Spawn de nave enemiga cada 3000 puntos
    if (score >= nextEnemyShipScore) {
        enemyShips.push_back(std::make_unique<EnemyShip>(enemyShipTex));
        nextEnemyShipScore += 2000;
    }

    // Balas jugador vs nave enemiga
    for (auto& b : nave->balas) {
        if (!b.activa) continue;
        for (auto it = enemyShips.begin(); it != enemyShips.end(); ) {
            if (b.CheckCollision(it->get())) {
                b.activa = false;
                (*it)->vida--;
                score += 300;

                if (score > highscore) {
                    highscore = score;
                    SaveHighScore();
                }

                if ((*it)->vida <= 0)
                    it = enemyShips.erase(it);
                else
                    ++it;
                break;
            } else {
                ++it;
            }
        }
    }

    // Balas enemigas vs jugador
    for (auto& ship : enemyShips) {
        for (auto& b : ship->balas) {
            if (b.activa && b.CheckCollision(nave.get())) {
                b.activa = false;
                nave->vida--;

                if (nave->vida <= 0)
                    state = GameState::GAMEOVER;
            }
        }
    }
}

void Game::UpdateGameOver() {
    if (IsKeyPressed(KEY_ENTER)) {
        Reset();
        state = GameState::PLAYING;
    }
}

void Game::DrawMenu() {
    DrawText("FULL GUNS MINERY", 200, 200, 40, WHITE);
    DrawText("ENTER PARA JUGAR", 240, 260, 20, GRAY);
    DrawText(TextFormat("HIGHSCORE: %d", highscore), 240, 300, 20, GRAY);
}

void Game::DrawPlaying() {
    nave->Draw();

    for (auto& e : enemigos)
        e->Draw();

    for (auto& ship : enemyShips)
        ship->Draw();

    DrawText(TextFormat("VIDA: %d", nave->vida), 10, 10, 20, WHITE);
    DrawText(TextFormat("SCORE: %d", score), 10, 35, 20, WHITE);
    DrawText(TextFormat("HIGH: %d", highscore), 10, 60, 20, WHITE);
    if (debugHitbox) {
        // Nave del jugador
        DrawCircleLines(
            (int)nave->GetPosition().x,
            (int)nave->GetPosition().y,
            nave->GetRadius(),
            GREEN
        );

        // Asteroides
        for (auto& e : enemigos) {
        DrawCircleLines(
            (int)e->GetPosition().x,
            (int)e->GetPosition().y,
            e->GetRadius(),
            YELLOW
        );
        }

        // Naves enemigas
        for (auto& ship : enemyShips) {
            DrawCircleLines(
                (int)ship->GetPosition().x,
                (int)ship->GetPosition().y,
                ship->GetRadius(),
                RED
            );
        }
    }
}

void Game::DrawGameOver() {
    DrawText("GAME OVER", 260, 220, 40, RED);
    DrawText("ENTER PARA REINICIAR", 230, 280, 20, GRAY);
    DrawText(TextFormat("HIGHSCORE: %d", highscore), 260, 320, 20, WHITE);
}

void Game::LoadHighScore() {
    FILE* f = fopen("highscore.dat", "r");
    if (f) {
        if (fscanf(f, "%d", &highscore) != 1)
            highscore = 0;
        fclose(f);
    } else {
        highscore = 0;
    }
}

void Game::SaveHighScore() {
    FILE* f = fopen("highscore.dat", "w");
    if (f) {
        fprintf(f, "%d\n", highscore);
        fclose(f);
    }
}
