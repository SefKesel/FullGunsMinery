#include "raylib.h"
#include "Game.h"

int main() {
    InitWindow(800, 600, "FULL GUNS MINERY");
    SetTargetFPS(60);

    Game game;

    while (!WindowShouldClose()) {
        game.Update();
        BeginDrawing();
        ClearBackground(BLACK);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}