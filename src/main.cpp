#include <iostream>

#include <raylib.h>
#include <raymath.h>
#include <vector>

#define IS_FULLSCREEN true
#define IS_WINDOWED false
#define IS_RESIZABLE true
#define LOG_INFO true

#define SCREEN_TITLE "Project: 3d Rotating Cube"
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define SCREEN_FPS 60
#define BG_COLOR BLACK
#define MIN_WIDTH 360
#define MIN_HEIGHT 240

#define IS_EVENTS_FOCUSED_ONLY true

float angleAlpha = 0;
float angleBeta = 0;
float angleSpeed = 2 * PI * 0.01f;
float dz = 4;
float dzSpeed = 0.1f;
float lineThickness = 2;

int w = SCREEN_WIDTH;
int h = SCREEN_HEIGHT;

Color bgColor = BG_COLOR;
Color fgColor = {static_cast<unsigned char>(255 - bgColor.r),
                 static_cast<unsigned char>(255 - bgColor.g),
                 static_cast<unsigned char>(255 - bgColor.b),
                 255}; // Opposite of background color

using namespace std;
void draw(float dt, vector<Vector3> nodes, vector<vector<int>> edges);
void handleEvents();
void handleWindowResize();
Vector2 screen(Vector2 pos);
void point(Vector2 pos);
Vector2 project(Vector3 pos3d);
Vector3 rotate(Vector3 pos3d, float angle);
Vector3 RotateXY(Vector3 p, float angleX, float angleY);
Vector3 translateZ(Vector3 pos3d, float transZ);

int main()
{

    SetTargetFPS(SCREEN_FPS);
    SetTraceLogLevel(LOG_ERROR);
    if (IS_RESIZABLE)
    {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    }

    InitWindow(w, h, SCREEN_TITLE);

    SetWindowMinSize(MIN_WIDTH, MIN_HEIGHT);
    SetWindowMonitor(GetCurrentMonitor());

    if (IS_WINDOWED)
    {
        int monitor = GetCurrentMonitor();
        w = GetMonitorWidth(monitor);
        h = GetMonitorHeight(monitor);
        SetWindowSize(w, h);
    }

    if (IS_WINDOWED && IS_FULLSCREEN)
        ToggleFullscreen();

    cout << "width: " << w << "\theight: " << h << "\n";

    float s = 1;
    vector<Vector3> nodes = {
        {-s, s, s},
        {s, s, s},
        {s, -s, s},
        {-s, -s, s},

        {-s, s, -s},
        {s, s, -s},
        {s, -s, -s},
        {-s, -s, -s},
    };

    vector<vector<int>> edges = {
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7}};

    while (!WindowShouldClose())
    {

        float dt = GetFrameTime();
        BeginDrawing();
        ClearBackground(bgColor);
        // start

        if (IsWindowResized())
            handleWindowResize();
        if (IS_EVENTS_FOCUSED_ONLY)
        {
            if (IsWindowFocused())
                handleEvents();
        }
        else
            handleEvents();

        draw(dt, nodes, edges);
        // point(screen(project({.5, .5, .5})));

        // end
        EndDrawing();
    }
    return 0;
}

void draw([[maybe_unused]] float dt, vector<Vector3> nodes, vector<vector<int>> edges)
{

    // draw edges
    for (Vector3 &node : nodes)
    {
        point(screen(project(translateZ(RotateXY(node, angleAlpha, angleBeta), dz))));
    }

    // draw vertices
    for (auto edge : edges)
    {
        for (size_t i = 0; i < edge.size(); ++i)
        {
            auto &line = edge[i];
            auto &nextLine = edge[(i + 1) % edge.size()];

            Vector2 start = screen(project(translateZ(RotateXY(nodes[(size_t)line], angleAlpha, angleBeta), dz)));
            Vector2 end = screen(project(translateZ(RotateXY(nodes[(size_t)nextLine], angleAlpha, angleBeta), dz)));

            DrawLineEx(start, end, lineThickness, GREEN);
        }
    }
}

void handleEvents()
{

    // Vertical Angle
    if (IsKeyDown(KEY_W))
        angleAlpha += angleSpeed;
    if (IsKeyDown(KEY_S))
        angleAlpha -= angleSpeed;
    // Horizontal Angle
    if (IsKeyDown(KEY_D))
        angleBeta -= angleSpeed;
    if (IsKeyDown(KEY_A))
        angleBeta += angleSpeed;
    // Z translation
    if (IsKeyDown(KEY_E))
        dz += dzSpeed;
    if (IsKeyDown(KEY_Q))
        dz -= dzSpeed;
    dz = Clamp(dz, 1, 1024);
    // Reset
    if (IsKeyDown(KEY_SPACE))
    {
        angleAlpha = 0;
        angleBeta = 0;
        dz = 4;
    }
}

void handleWindowResize()
{

    w = GetScreenWidth();
    h = GetScreenHeight();
}

Vector2 screen(Vector2 pos)
{
    float s = float(w) * .80f;

    // DrawRectangleLines((w / 2) - (s / 2), (h / 2) - (s / 2), s, s, RED);
    return {(float(w) / 2.f) - (s / 2.f) + (pos.x + 1) / 2.f * s,
            (float(h) / 2.f) - (s / 2.f) + (1 - (pos.y + 1) / 2.f) * s};
}

void point(Vector2 pos)
{
    float s = 10;
    DrawRectangle(static_cast<int>(pos.x - float(s / 2.f)),
                  static_cast<int>(pos.y - float(s / 2.f)),
                  int(s),
                  int(s),
                  GREEN);
}

Vector2 project(Vector3 pos3d)
{
    return {
        pos3d.x / pos3d.z,
        pos3d.y / pos3d.z,
    };
}

Vector3 RotateXY(Vector3 p, float angleX, float angleY)
{
    // --- rotate around X axis ---
    float cosX = cosf(angleX);
    float sinX = sinf(angleX);

    float y1 = p.y * cosX - p.z * sinX;
    float z1 = p.y * sinX + p.z * cosX;

    // --- rotate around Y axis ---
    float cosY = cosf(angleY);
    float sinY = sinf(angleY);

    float x2 = p.x * cosY + z1 * sinY;
    float z2 = -p.x * sinY + z1 * cosY;

    return {x2, y1, z2};
}

Vector3 translateZ(Vector3 pos3d, float transZ)
{
    return {pos3d.x, pos3d.y, pos3d.z + transZ};
}