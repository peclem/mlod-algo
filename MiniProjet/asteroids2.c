/*******************************************************************************************
*
*   raylib - classic game: asteroids survival
*
*   Sample game developed by Ian Eito, Albert Martos and Ramon Santamaria
*
*   This game has been created using raylib v1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include <math.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define PLAYER_BASE_SIZE    20.0f
#define PLAYER_SPEED        6.0f
#define PLAYER_MAX_SHOOTS   10

#define METEORS_SPEED       2
#define MAX_MEDIUM_METEORS  8
#define MAX_SMALL_METEORS   16

#define METEOR_SPEED_FACTOR 0.6f
#define BONUS_TIME          100
#define BONUS_MAX_COOLDOWN  500
#define BONUS_MIN_COOLDOWN  250

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct Player {
    Vector2 position;
    Vector2 speed;
    float acceleration;
    float rotation;
    Vector3 collider;
    Color color;
    int cooldown;
    int bonus;
} Player;

typedef struct Meteor {
    Vector2 position;
    Vector2 speed;
    float radius;
    bool active;
    Color color;
} Meteor;

typedef struct Bonus {
    Vector2 position;
    float radius;
    bool active;
    Color color;
    int cooldown;
    int time;
} Bonus;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;

static int framesCounter = 0;

static bool gameOver = false;
static bool pause = false;

static int removedAsteroids = 4;
static float asteroidsSpeed = 0.6f;

// NOTE: Defined triangle is isosceles with common angles of 70 degrees.
static float shipHeight = 0.0f;

static Player player = { 0 };
static Meteor mediumMeteor[MAX_MEDIUM_METEORS] = { 0 };
static Meteor smallMeteor[MAX_SMALL_METEORS] = { 0 };
static Bonus bonus = { 0 };

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "classic game: asteroids survival");

    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
    }
#endif
    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGame();         // Unload loaded data (textures, sounds, models...)

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(void)
{
    int posx, posy;
    int velx, vely;
    bool correctRange = false;

    pause = false;

    framesCounter = 0;
    
    removedAsteroids = 4;
    asteroidsSpeed = 0.6f;

    shipHeight = (PLAYER_BASE_SIZE/2)/tanf(20*DEG2RAD);

    // Initialization player
    player.position = (Vector2){screenWidth/2, screenHeight/2 - shipHeight/2};
    player.speed = (Vector2){0, 0};
    player.acceleration = 0;
    player.rotation = 0;
    player.collider = (Vector3){player.position.x + sin(player.rotation*DEG2RAD)*(shipHeight/2.5f), player.position.y - cos(player.rotation*DEG2RAD)*(shipHeight/2.5f), 12};
    player.color = LIGHTGRAY;
    player.cooldown = 0;


    posx = GetRandomValue(0, screenWidth-50);
    posy = GetRandomValue(0, screenHeight-50);
    bonus.position = (Vector2){posx, posy};
    bonus.color = GREEN;
    bonus.cooldown = 300;
    bonus.time = 0;
    bonus.radius = 20;


    for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
    {
        posx = GetRandomValue(0, screenWidth);

        while (!correctRange)
        {
            if (posx > screenWidth/2 - 150 && posx < screenWidth/2 + 150) posx = GetRandomValue(0, screenWidth);
            else correctRange = true;
        }

        correctRange = false;

        posy = GetRandomValue(0, screenHeight);

        while (!correctRange)
        {
            if (posy > screenHeight/2 - 150 && posy < screenHeight/2 + 150)  posy = GetRandomValue(0, screenHeight);
            else correctRange = true;
        }

        correctRange = false;
        velx = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
        vely = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);

        while (!correctRange)
        {
            if (velx == 0 && vely == 0)
            {
                velx = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
                vely = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
            }
            else correctRange = true;
        }
        mediumMeteor[i].position = (Vector2){posx, posy};
        mediumMeteor[i].speed = (Vector2){velx, vely};
        mediumMeteor[i].radius = 20;
        mediumMeteor[i].active = true;
        mediumMeteor[i].color = GREEN;
    }

    for (int i = 0; i < MAX_SMALL_METEORS; i++)
    {
        posx = GetRandomValue(0, screenWidth);

        while (!correctRange)
        {
            if (posx > screenWidth/2 - 150 && posx < screenWidth/2 + 150) posx = GetRandomValue(0, screenWidth);
            else correctRange = true;
        }

        correctRange = false;

        posy = GetRandomValue(0, screenHeight);

        while (!correctRange)
        {
            if (posy > screenHeight/2 - 150 && posy < screenHeight/2 + 150)  posy = GetRandomValue(0, screenHeight);
            else correctRange = true;
        }

        correctRange = false;
        velx = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
        vely = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);

        while (!correctRange)
        {
            if (velx == 0 && vely == 0)
            {
                velx = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
                vely = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
            }
            else correctRange = true;
        }
        smallMeteor[i].position = (Vector2){posx, posy};
        smallMeteor[i].speed = (Vector2){velx, vely};
        smallMeteor[i].radius = 10;
        smallMeteor[i].active = true;
        smallMeteor[i].color = YELLOW;
    }
}


float distance(float x1, float y1, float x2, float y2)
{
    return sqrt( ( (x2-x1) * (x2-x1) ) + ( (y2-y1) * (y2-y1) ) );
}
///explode : supprime les asteroides dans un certain rayon autour du vaisseau
void explode()
{  
    int count = 0;
    for (int i = 0; i < MAX_MEDIUM_METEORS - removedAsteroids; i++)
    {
        float x = mediumMeteor[i].position.x;
        float y = mediumMeteor[i].position.y;
        float distancePlayerAsteroid = distance(x, y, player.position.x, player.position.y);
        if (distancePlayerAsteroid < 100)
        {
            /// la fonction distance agit ici comme norme pour la vitesse egalement.
            mediumMeteor[i].speed.x = distance(mediumMeteor[i].speed.x, mediumMeteor[i].speed.y, 0, 0) * (x - player.position.x)/distancePlayerAsteroid;
            mediumMeteor[i].speed.y = distance(mediumMeteor[i].speed.x, mediumMeteor[i].speed.y, 0, 0) * (y - player.position.y)/distancePlayerAsteroid;
            count += 1;
        }
    }
    for (int i = 0; i < MAX_SMALL_METEORS - (removedAsteroids * 2); i++)
    {
        float x = smallMeteor[i].position.x;
        float y = smallMeteor[i].position.y;
        float distancePlayerAsteroid = distance(x, y, player.position.x, player.position.y);
        if (distancePlayerAsteroid < 100)
        {
            smallMeteor[i].speed.x = distance(smallMeteor[i].speed.x, smallMeteor[i].speed.y, 0, 0) * (x - player.position.x)/distancePlayerAsteroid;
            smallMeteor[i].speed.y = distance(smallMeteor[i].speed.x, smallMeteor[i].speed.y, 0, 0) * (y - player.position.y)/distancePlayerAsteroid;
            count += 1;
        }
    }
    player.cooldown = 300;
    if (count >= 4)
    {
        for (int i = 0; i < MAX_MEDIUM_METEORS - removedAsteroids; i++)
        {
            float x = mediumMeteor[i].position.x;
            float y = mediumMeteor[i].position.y;
            float distancePlayerAsteroid = distance(x, y, player.position.x, player.position.y);
            if (distancePlayerAsteroid >= 100)
            {
                mediumMeteor[i].speed.x = distance(mediumMeteor[i].speed.x, mediumMeteor[i].speed.y, 0, 0) * (x - player.position.x)/distancePlayerAsteroid;
                mediumMeteor[i].speed.y = distance(mediumMeteor[i].speed.x, mediumMeteor[i].speed.y, 0, 0) * (y - player.position.y)/distancePlayerAsteroid;
            }
        }
    
        for (int i = 0; i < MAX_SMALL_METEORS - (removedAsteroids * 2); i++)
        {
            float x = smallMeteor[i].position.x;
            float y = smallMeteor[i].position.y;
            float distancePlayerAsteroid = distance(x, y, player.position.x, player.position.y);
            if (distancePlayerAsteroid >= 100)
            {
                smallMeteor[i].speed.x = distance(smallMeteor[i].speed.x, smallMeteor[i].speed.y, 0, 0) * (x - player.position.x)/distancePlayerAsteroid;
                smallMeteor[i].speed.y = distance(smallMeteor[i].speed.x, smallMeteor[i].speed.y, 0, 0) * (y - player.position.y)/distancePlayerAsteroid;
            }
        }
    }
}

// Update game (one frame)
void UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause)
        {
            framesCounter++;
            
            if (removedAsteroids > 0)
            {
                removedAsteroids =  4 -  (framesCounter / 1200);
                asteroidsSpeed = 0.6f + 0.3 * (float)(framesCounter / 1200);
                for (int i = MAX_SMALL_METEORS - 8; i < MAX_SMALL_METEORS - (removedAsteroids * 2); i++)
                {
                    smallMeteor[i].active = true;
                }
                for (int i = MAX_MEDIUM_METEORS - 4; i < MAX_MEDIUM_METEORS - removedAsteroids; i++)
                {
                    mediumMeteor[i].active = true;
                }
            }

            // Player logic
            if (IsKeyDown(KEY_SPACE) && player.cooldown == 0) explode();

            if (player.cooldown > 0) player.cooldown -=1;

            if (bonus.cooldown > 0) bonus.cooldown -=1;
            if (bonus.time > 0) bonus.time -=1;

            if (CheckCollisionCircles((Vector2){player.collider.x, player.collider.y}, player.collider.z, bonus.position, bonus.radius) && bonus.cooldown == 0)
            {
                bonus.cooldown = GetRandomValue(BONUS_MIN_COOLDOWN, BONUS_MAX_COOLDOWN);
                bonus.time = BONUS_TIME;
                int posx = GetRandomValue(0, screenWidth-50);
                int posy = GetRandomValue(0, screenHeight-50);
                bonus.position = (Vector2){posx, posy};
            }
            
            
            // Rotation
            if (IsKeyDown(KEY_LEFT)) player.rotation -= 5;
            if (IsKeyDown(KEY_RIGHT)) player.rotation += 5;

            // Speed
            player.speed.x = sin(player.rotation*DEG2RAD)*PLAYER_SPEED;
            player.speed.y = cos(player.rotation*DEG2RAD)*PLAYER_SPEED;

            // Controller
            if (IsKeyDown(KEY_UP))
            {
                if (player.acceleration < 1) player.acceleration += 0.04f;
            }
            else
            {
                if (player.acceleration > 0) player.acceleration -= 0.02f;
                else if (player.acceleration < 0) player.acceleration = 0;
            }
            if (IsKeyDown(KEY_DOWN))
            {
                if (player.acceleration > 0) player.acceleration -= 0.04f;
                else if (player.acceleration < 0) player.acceleration = 0;
            }

            // Movement
            player.position.x += (player.speed.x*player.acceleration);
            player.position.y -= (player.speed.y*player.acceleration);

            // Wall behaviour for player
            if (player.position.x > screenWidth + shipHeight) player.position.x = -(shipHeight);
            else if (player.position.x < -(shipHeight)) player.position.x = screenWidth + shipHeight;
            if (player.position.y > (screenHeight + shipHeight)) player.position.y = -(shipHeight);
            else if (player.position.y < -(shipHeight)) player.position.y = screenHeight + shipHeight;

            // Collision Player to meteors
            player.collider = (Vector3){player.position.x + sin(player.rotation*DEG2RAD)*(shipHeight/2.5f), player.position.y - cos(player.rotation*DEG2RAD)*(shipHeight/2.5f), 12};

            for (int a = 0; a < MAX_MEDIUM_METEORS - removedAsteroids; a++)
            {
                if (CheckCollisionCircles((Vector2){player.collider.x, player.collider.y}, player.collider.z, mediumMeteor[a].position, mediumMeteor[a].radius) && mediumMeteor[a].active) gameOver = true;
            }

            for (int a = 0; a < MAX_SMALL_METEORS - (removedAsteroids * 2); a++)
            {
                if (CheckCollisionCircles((Vector2){player.collider.x, player.collider.y}, player.collider.z, smallMeteor[a].position, smallMeteor[a].radius) && smallMeteor[a].active) gameOver = true;
            }


            // Meteor logic
            float meteorSlowFactor;
            if(bonus.time > 0)meteorSlowFactor = METEOR_SPEED_FACTOR;
            else meteorSlowFactor = 1;

            for (int i = 0; i < MAX_MEDIUM_METEORS - removedAsteroids; i++)
            {
                if (mediumMeteor[i].active)
                {
                    // movement
                    mediumMeteor[i].position.x += mediumMeteor[i].speed.x * meteorSlowFactor * asteroidsSpeed;
                    mediumMeteor[i].position.y += mediumMeteor[i].speed.y * meteorSlowFactor * asteroidsSpeed;

                    // wall behaviour
                    if  (mediumMeteor[i].position.x > screenWidth + mediumMeteor[i].radius) mediumMeteor[i].position.x = -(mediumMeteor[i].radius);
                    else if (mediumMeteor[i].position.x < 0 - mediumMeteor[i].radius) mediumMeteor[i].position.x = screenWidth + mediumMeteor[i].radius;
                    if (mediumMeteor[i].position.y > screenHeight + mediumMeteor[i].radius) mediumMeteor[i].position.y = -(mediumMeteor[i].radius);
                    else if (mediumMeteor[i].position.y < 0 - mediumMeteor[i].radius) mediumMeteor[i].position.y = screenHeight + mediumMeteor[i].radius;
                }
            }

            for (int i = 0; i < MAX_SMALL_METEORS - (removedAsteroids * 2); i++)
            {
                if (smallMeteor[i].active)
                {
                    // movement
                    smallMeteor[i].position.x += smallMeteor[i].speed.x * meteorSlowFactor * asteroidsSpeed;
                    smallMeteor[i].position.y += smallMeteor[i].speed.y * meteorSlowFactor * asteroidsSpeed;

                    // wall behaviour
                    if  (smallMeteor[i].position.x > screenWidth + smallMeteor[i].radius) smallMeteor[i].position.x = -(smallMeteor[i].radius);
                    else if (smallMeteor[i].position.x < 0 - smallMeteor[i].radius) smallMeteor[i].position.x = screenWidth + smallMeteor[i].radius;
                    if (smallMeteor[i].position.y > screenHeight + smallMeteor[i].radius) smallMeteor[i].position.y = -(smallMeteor[i].radius);
                    else if (smallMeteor[i].position.y < 0 - smallMeteor[i].radius) smallMeteor[i].position.y = screenHeight + smallMeteor[i].radius;
                }
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
        }
    }
}

// Draw game (one frame)
void DrawGame(void)
{
    BeginDrawing();

        ClearBackground(RAYWHITE);

        if (!gameOver)
        {
            // Draw spaceship
            Vector2 v1 = { player.position.x + sinf(player.rotation*DEG2RAD)*(shipHeight), player.position.y - cosf(player.rotation*DEG2RAD)*(shipHeight) };
            Vector2 v2 = { player.position.x - cosf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE/2), player.position.y - sinf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE/2) };
            Vector2 v3 = { player.position.x + cosf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE/2), player.position.y + sinf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE/2) };
            DrawTriangle(v1, v2, v3, MAROON);

            // Draw meteor
            for (int i = 0;i < MAX_MEDIUM_METEORS - removedAsteroids; i++)
            {
                if (mediumMeteor[i].active) DrawCircleV(mediumMeteor[i].position, mediumMeteor[i].radius, GRAY);
                else DrawCircleV(mediumMeteor[i].position, mediumMeteor[i].radius, Fade(LIGHTGRAY, 0.3f));
            }

            for (int i = 0;i < MAX_SMALL_METEORS - (removedAsteroids * 2); i++)
            {
                if (smallMeteor[i].active) DrawCircleV(smallMeteor[i].position, smallMeteor[i].radius, DARKGRAY);
                else DrawCircleV(smallMeteor[i].position, smallMeteor[i].radius, Fade(LIGHTGRAY, 0.3f));
            }

            if (bonus.cooldown == 0) DrawCircleV(bonus.position, bonus.radius, bonus.color);

            DrawText(TextFormat("TIME: %.02f", (float)framesCounter/60), 10, 10, 20, BLACK);

            DrawText(TextFormat("shield cooldown: %.02f", (float)player.cooldown/60), screenWidth -250, 10, 20, BLACK);

            if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);
        }
        else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 - 50, 20, GRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
}

// Unload game variables
void UnloadGame(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}