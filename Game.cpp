#include "Game.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <cctype>

Game::Game() {
    naveTex = LoadTexture("cohete.png");

    texAstNormal = LoadTexture("asteroid_normal.png");
    texAstFast   = LoadTexture("asteroid_fast.png");
    texAstZigZag = LoadTexture("asteroid_zigzag.png");
    enemyShipTex = LoadTexture("enemyShip.png");
    
    state = GameState::MENU;
    debugHitbox = false;
    highscore = 0;
    highscoreName = "---";
    pendingHighscore = 0;
    currentName = "AAA";
    namePos = 0;
    backgroundTex = LoadTexture("fondo.png");
    bgOffset = 0.0f;
    bgSpeed = 80.0f; // pixels per second
    LoadHighScore();
    Reset();
}

Game::~Game() {
    UnloadTexture(naveTex);
    if (backgroundTex.width > 0) UnloadTexture(backgroundTex);

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
        enemigos.push_back(std::make_unique<EnemyNormal>(texAstNormal));

    // Fast
    for (int i = 0; i < 2; i++)
        enemigos.push_back(std::make_unique<EnemyFast>(texAstFast));

    // ZigZag
    for (int i = 0; i < 2; i++)
        enemigos.push_back(std::make_unique<EnemyZigZag>(texAstZigZag));

}

void Game::Update() {
    // Update background offset regardless of state (smooth scrolling)
    if (backgroundTex.width > 0) {
        float destScale = (float)GetScreenWidth() / (float)backgroundTex.width;
        int destH = (int)(backgroundTex.height * destScale);
        bgOffset += bgSpeed * GetFrameTime();
        if (bgOffset >= destH) bgOffset -= destH;
    }
    switch (state) {
        case GameState::MENU:     UpdateMenu();     break;
        case GameState::PLAYING:  UpdatePlaying();  break;
        case GameState::ENTER_NAME: UpdateEnterName(); break;
        case GameState::GAMEOVER: UpdateGameOver(); break;
    }
}

void Game::Draw() {
    switch (state) {
        case GameState::MENU:     DrawMenu();     break;
        case GameState::PLAYING:  DrawPlaying();  break;
        case GameState::ENTER_NAME: DrawEnterName(); break;
        case GameState::GAMEOVER: DrawGameOver(); break;
    }
}

void Game::UpdateEnterName() {
    // Cambia letra
    if (IsKeyPressed(KEY_UP)) {
        char &c = currentName[namePos];
        if (c >= 'A' && c < 'Z') c++;
        else c = 'A';
    }
    if (IsKeyPressed(KEY_DOWN)) {
        char &c = currentName[namePos];
        if (c > 'A' && c <= 'Z') c--;
        else c = 'Z';
    }

    // Mueve posición
    if (IsKeyPressed(KEY_LEFT)) {
        if (namePos > 0) namePos--;
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        if (namePos < 2) namePos++;
    }

    // Borrar letra (poner a 'A' y mover atrás)
    if (IsKeyPressed(KEY_BACKSPACE)) {
        currentName[namePos] = 'A';
        if (namePos > 0) namePos--;
    }

    // Confirma nombre
    if (IsKeyPressed(KEY_ENTER)) {
        // Guardar nuevo highscore
        highscore = pendingHighscore;
        highscoreName = currentName;
        SaveHighScore();
        pendingHighscore = 0;
        state = GameState::GAMEOVER;
    }
}

void Game::DrawEnterName() {
    DrawText("NEW HIGHSCORE!", 200, 160, 30, GOLD);
    DrawText(TextFormat("SCORE: %d", pendingHighscore), 260, 200, 20, WHITE);
    DrawText("ENTER NAME (3 LETRAS):", 180, 240, 20, WHITE);
    DrawText(currentName.c_str(), 320, 270, 40, WHITE);

    int x = 320 + namePos * 24;
    DrawLine(x, 310, x + 18, 310, WHITE);

    DrawText("Arriba/Abajo: letra    Izquierda/Derecha: mover    ENTER: guardar", 80, 350, 18, GRAY);
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
                score += e->GetScoreValue();

                if (score > highscore) {
                    // Nuevo highscore pendiente
                    if (pendingHighscore < score) pendingHighscore = score;
                }

                if (score >= nextExtraLifeScore) {
                    nave->vida++;
                    nextExtraLifeScore += 1000;
                }
                break;
            }
        }
    }

    // Spawn de nave enemiga cada 2000 puntos
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
                score += 150;

                if (score > highscore) {
                    if (pendingHighscore < score) pendingHighscore = score;
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
    // Si está pendiente un highscore, solicita el nombre (despues de la muerte)
    if (pendingHighscore > highscore) {
        currentName = "AAA";
        namePos = 0;
        state = GameState::ENTER_NAME;
        return;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        Reset();
        state = GameState::PLAYING;
    }
}

void Game::DrawMenu() {
    DrawText("FULL GUNS MINERY", 200, 200, 40, WHITE);
    DrawText("ENTER PARA JUGAR", 240, 260, 20, GRAY);
    DrawText(TextFormat("HIGHSCORE: %s %d", highscoreName.c_str(), highscore), 240, 300, 20, GRAY);
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
    DrawText(TextFormat("HIGHSCORE: %s %d", highscoreName.c_str(), highscore), 260, 320, 20, WHITE);
}

void Game::LoadHighScore() {
    FILE* f = fopen("highscore.dat", "r");
    if (f) {
        char namebuf[64] = "---";
        int read = fscanf(f, "%d %63s", &highscore, namebuf);
        if (read < 1) {
            highscore = 0;
            highscoreName = "---";
        } else if (read == 1) {
            highscoreName = "---";
        } else {
            // Se asegura de que el nombre tenga como máximo 3 letras mayúsculas
            for (int i = 0; namebuf[i]; ++i) namebuf[i] = toupper((unsigned char)namebuf[i]);
            highscoreName = std::string(namebuf).substr(0, 3);
        }
        fclose(f);
    } else {
        highscore = 0;
        highscoreName = "---";
    }
}

void Game::SaveHighScore() {
    FILE* f = fopen("highscore.dat", "w");
    if (f) {
        char namebuf[4] = "---";
        // Se asegura de que el nombre tenga como máximo 3 letras mayúsculas
        for (int i = 0; i < 3 && i < (int)highscoreName.size(); ++i)
            namebuf[i] = toupper((unsigned char)highscoreName[i]);
        fprintf(f, "%d %s\n", highscore, namebuf);
        fclose(f);
    }
}
