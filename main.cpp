using namespace std;
#include <iostream>
#include "raylib.h"
#include <vector>
#include <math.h>
#include <string>
#include <cstdlib>
#include "Asteroide.h"
#include "Bala.h"

int main() {
    InitWindow(800, 600, "FULL GUNS MINERY");
    SetTargetFPS(60);
    Texture2D asteroide = LoadTexture("asteroide.png"), nave = LoadTexture("cohete.png");
    Vector2 navePosition = {((float)GetScreenWidth() - ( (float)nave.width * 0.1f )) / 2, (float)GetScreenHeight() /10*8 };
    int vida = 3, bal = 0, puntaje = 0, mejorPuntaje;
    bool vivo = true,disp = false, mejorPunt = false;

    // Cargar mejor puntaje desde archivo
    FILE* guardado = fopen("highscore.txt", "r");
    if (guardado != NULL) {
        fscanf(guardado, "%d", &mejorPuntaje);
        fclose(guardado);
    } else {
        mejorPuntaje = 0;
    }

    // Creación de balas
    vector<Bala> balas;
    for (int i = 0; i < 10; i++) {
        Bala b;
        b.posicion = {0, 0};
        b.activa = false;
        balas.push_back(b);
    }

    // Creación de asteroides
    vector<Asteroide> asteroides;
    for (int i = 0; i < 10; i++) {
        Asteroide a;
        a.textura = asteroide;
        a.posicion = {(float)GetRandomValue(100, GetScreenWidth()-100),   // X
            (float)GetRandomValue(-200, -100)};    // Y
        a.rotacion = (float)GetRandomValue(0, 360);
        a.velocidad = (float)GetRandomValue(100, 200) / 100.0f; // velocidad entre 1.0 y 2.0
        a.escala = (float)GetRandomValue(5, 15) / 100.0f; // tamaño 0.05x a 0.15x
        asteroides.push_back(a);
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Dibujo de los Asteroides
        for (int a = 0; a < asteroides.size(); a++) {
            // Movimiento hacia abajo
            asteroides[a].posicion.y += asteroides[a].velocidad;

            // Rotación hacia izquierda o derecha
            if (a % 2 == 0) {
                asteroides[a].rotacion -= 0.4f;
            } else {
                asteroides[a].rotacion += 0.4f;
            }

            // Si se sale de la pantalla, reaparece arriba con nueva X
            if (asteroides[a].posicion.y > GetScreenHeight() + 50) {
            asteroides[a].posicion.y = (float)GetRandomValue(-200, -50);
            asteroides[a].posicion.x = (float)GetRandomValue(0, GetScreenWidth());
            }

            // Dibujo con tamaño y rotación
            DrawTexturePro(asteroides[a].textura, 
                       { 0.0f, 0.0f, (float)asteroides[a].textura.width, (float)asteroides[a].textura.height}, 
                       { asteroides[a].posicion.x + (asteroides[a].textura.width * asteroides[a].escala) / 2, // x
                        asteroides[a].posicion.y + (asteroides[a].textura.height * asteroides[a].escala) / 2, // y
                        asteroides[a].textura.width * asteroides[a].escala, asteroides[a].textura.height * asteroides[a].escala }, 
                       {(float)asteroides[a].textura.width * asteroides[a].escala/ 2 , (float)asteroides[a].textura.height * asteroides[a].escala/ 2}, asteroides[a].rotacion, WHITE);
        }
        
        // Movimiento de la nave
        if (vivo) {
            if (IsKeyDown(KEY_RIGHT)) navePosition.x += 6.0f;
            if (IsKeyDown(KEY_LEFT)) navePosition.x -= 6.0f;
            if (IsKeyDown(KEY_UP)) navePosition.y -= 4.0f;
            if (IsKeyDown(KEY_DOWN)) navePosition.y += 4.0f;
            if (navePosition.x < -22) navePosition.x = -22;
            if (navePosition.x > GetScreenWidth() - nave.width*0.11f) navePosition.x = GetScreenWidth() - nave.width*0.11f;
            if (navePosition.y < 48) navePosition.y = 48;
            if (navePosition.y > GetScreenHeight() - ( (float)nave.height * 0.05f )) navePosition.y = GetScreenHeight() - ( (float)nave.height * 0.05f );
        
            // Dibujo de la nave
            DrawTexturePro(nave, 
                   { 0.0f, 0.0f, (float)nave.width, (float)nave.height }, 
                   { navePosition.x, navePosition.y, (float)nave.width * 0.1f, (float)nave.height * 0.1f }, 
                   { 0.0f, 0.0f }, -44.5f, WHITE);

            // Colisiones nave-asteroides
            for (int a = 0; a < asteroides.size(); a++) {
                if (CheckCollisionCircles(
                    { navePosition.x + ( (float)nave.width * 0.05f ), navePosition.y + ( (float)nave.height * 0.05f ) },
                    ( (float)nave.width * 0.05f ),
                    { asteroides[a].posicion.x + (asteroides[a].textura.width * asteroides[a].escala) / 2,
                    asteroides[a].posicion.y + (asteroides[a].textura.height * asteroides[a].escala) / 2 },
                    ((asteroides[a].textura.width * asteroides[a].escala) / 2) - 2)) {
                
                    vida -= 1; // Reducir vida por colisión
                    // Reiniciar asteroide
                    asteroides[a].posicion.y = (float)GetRandomValue(-200, -100);
                    asteroides[a].posicion.x = (float)GetRandomValue(100, GetScreenWidth()-100);
                }
            }
            // Disparo de la nave
            if (IsKeyPressed(KEY_SPACE)) {
                disp = true;
            }
            if (disp) {
                balas[bal].posicion.x = navePosition.x + ( (float)nave.width * 0.075f );
                balas[bal].posicion.y = navePosition.y;
                balas[bal].activa = true;
                bal++;
                if (bal >= balas.size()) bal = 0;
                disp = false;
            }
            for (int i = 0; i < balas.size(); i++) {
                if (balas[i].activa) {
                DrawCircle(balas[i].posicion.x, balas[i].posicion.y, 5, YELLOW);
                balas[i].posicion.y -= 10;
                }
                if (balas[i].posicion.y < 0) balas[bal].activa = false;
                if (i >= balas.size()) i = 0;
            }
            // Colisiones balas-asteroides
            for (int i = 0; i < balas.size(); i++) {
                for (int a = 0; a < asteroides.size(); a++) {
                    if (balas[i].activa &&
                        CheckCollisionCircles(
                            { balas[i].posicion.x, balas[i].posicion.y }, 5,
                            { asteroides[a].posicion.x + (asteroides[a].textura.width * asteroides[a].escala) / 2,
                            asteroides[a].posicion.y + (asteroides[a].textura.height * asteroides[a].escala) / 2 },
                            ((asteroides[a].textura.width * asteroides[a].escala) / 2) - 2)) {
                        // Reiniciar asteroide
                        asteroides[a].posicion.y = (float)GetRandomValue(-200, -100);
                        asteroides[a].posicion.x = (float)GetRandomValue(100, GetScreenWidth()-100);
                        // Desactivar bala
                        balas[i].activa = false;
                        // Aumentar puntaje
                        puntaje += 10;
                        // Vida extra
                        if (puntaje >= 10000) {
                            vida += 1; // Vida extra cada 10,000 puntos
                            puntaje -= 10000; // Restar los 10,000 puntos usados para la vida extra
                        }
                    }
                }
            }
        }


        // Mostrar coordenadas de la nave (para testeo)
        //DrawText(TextFormat("Posición X: %i", (int)navePosition.x), 10, 50, 20, RAYWHITE);
        //DrawText(TextFormat("Posición y: %i", (int)navePosition.y), 10, 30, 20, RAYWHITE);
        // Mostrar vida y puntaje
        DrawText(TextFormat("Vida: %i", vida), 10, GetScreenHeight() - 50, 20, RAYWHITE);
        DrawText(TextFormat("Puntaje: %i", puntaje), 10, 10, 20, RAYWHITE);
        DrawText(TextFormat("Mejor Puntaje: %i", mejorPuntaje), 10, GetScreenHeight() - 30, 20, RAYWHITE);
        DrawText("FULL GUNS MINERY", GetScreenHeight()/2, 10, 20, RAYWHITE);

        // GAMEOVER
        if (vida <= 0) {
            vivo = false;

            DrawText("GAME OVER", GetScreenHeight() / 2, GetScreenHeight() / 2 - 40, 40, RED);
            DrawText(TextFormat("Puntaje Final: %i", puntaje), GetScreenHeight() / 2, GetScreenHeight() / 2, 30, WHITE);

            // Mejor puntaje
            if (puntaje > mejorPuntaje) {
                mejorPuntaje = puntaje;
                mejorPunt = true;

                // Guardar mejor puntaje en archivo
                FILE* guardado = fopen("highscore.txt", "w");
                if (guardado != NULL) {
                    fprintf(guardado, "%d", mejorPuntaje);
                    fclose(guardado);
                }
            }
            if (mejorPunt) {
                DrawText("NEW HIGH SCORE!", GetScreenHeight() / 2, GetScreenHeight() / 2 + 40, 30, GOLD);
            }
            
            // Reiniciar juego con tecla Enter
            DrawText("Presiona ENTER para reiniciar", GetScreenHeight() / 2, GetScreenHeight() / 2 + 80, 20, LIGHTGRAY);
            if (IsKeyPressed(KEY_ENTER)) {
                // Reiniciar variables
                vida = 3;
                puntaje = 0;
                navePosition = {((float)GetScreenWidth() - ( (float)nave.width * 0.1f )) / 2, (float)GetScreenHeight() /10*8 };
                vivo = true;
                mejorPunt = false;
                bal = 0;
                // Reiniciar asteroides
                for (int i = 0; i < asteroides.size(); i++) {
                    asteroides[i].posicion = {(float)GetRandomValue(100, GetScreenWidth()-100),   // X
                        (float)GetRandomValue(-200, -100)}  ;    // Y
                    asteroides[i].rotacion = (float)GetRandomValue(0, 360);
                    asteroides[i].velocidad = (float)GetRandomValue(100, 200) / 100.0f; // velocidad entre 1.0 y 2.0   
                    asteroides[i].escala = (float)GetRandomValue(5, 15) / 100.0f; // tamaño 0.05x a 0.15x
                }
            }
        }
        EndDrawing();
    }
    
    fclose(guardado);
    UnloadTexture(LoadTexture("cohete.png"));
    UnloadTexture(LoadTexture("asteroide.png"));
    CloseWindow();
    return 0;
}