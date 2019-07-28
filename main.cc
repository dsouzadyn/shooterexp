#include <raylib.h>

#define RAYMATH_IMPLEMNTATION
#include <raymath.h>

#define MAX_SHOTS 5
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define MAX_OBJECTS 5

class Entity {
	public:
		Vector2 position;
		Vector2 direction;
		Vector2 speed;
		bool isActive = false;
};

class Projectile : public Entity {
	public:
		Color color = BLACK;
		float radius = 5.0f;
};

class Ball : public Entity {
	public:
		Color color = DARKBLUE;
		float radius = 30.0f;
};

class Pointer : public Entity {
	public:
		Color color = RED;
		float radius = 5.0f;
};

class Object : public Entity {
	public:
		Color color = RED;
		float radius = 10.0f;
};

bool Vector2Equal(Vector2 a, Vector2 b) {
	return (a.x == b.x && a.y == b.y);
}

static Ball ball;
static Pointer pointer;
static Projectile projectile[MAX_SHOTS];
static Object object[MAX_OBJECTS];
static int score;
static bool gameover;

void InitGame();
void UpdateGame();
void DrawGame();
void DrawGameOver();

int main() {

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Shoot Test");

	InitGame();

	while(!WindowShouldClose()) {
		if(!gameover) {
			UpdateGame();
			DrawGame();
		} else {
			DrawGameOver();
		}
	}

	CloseWindow();
	return 0;
}

void InitGame() {
	score = 0;
	gameover = false;
	ball.position = Vector2 { 100.0f, 100.0f };
	ball.speed = Vector2 { 2.0f, 2.0f };
	ball.isActive = true;
	
	object[0].position = Vector2 { 100.0f, 300.0f };
	object[1].position = Vector2 { 500.0f, 400.0f };
	object[2].position = Vector2 { 300.0f, 360.0f };
	object[3].position = Vector2 { 400.0f, 280.0f };
	object[4].position = Vector2 { 620.0f, 100.0f };

	for(int i = 0; i < MAX_OBJECTS; i++) {
		object[i].speed = Vector2{ 1.0f, 1.0f };
		object[i].isActive = true;
	}
	for (int i = 0; i < MAX_SHOTS; i++) {
		projectile[i].speed = Vector2{ 5.0f, 5.0f };
	}
	SetTargetFPS(60);
}

void UpdateGame() {

	pointer.position = GetMousePosition();
	for(int o = 0; o < MAX_OBJECTS; o++) {
		if (object[o].isActive) {
			bool collision = false;
			for (int i = 0; i < MAX_SHOTS; i++) {
				if (projectile[i].isActive) {
					collision = CheckCollisionPointCircle(projectile[i].position, object[o].position, object[o].radius);
					if (collision) { 
						object[o].isActive = false;
						score += 100;
					}
				}
			}
			if (ball.isActive) {
				collision = CheckCollisionPointCircle(object[o].position, ball.position, ball.radius);
				if (collision) {
					ball.isActive = false;
					gameover = true;
				}
			}
		}
	}

	for(int o = 0; o < MAX_OBJECTS; o++) {
		if (object[o].isActive) {
			object[o].direction = Vector2Subtract(ball.position, object[o].position);
			if(!Vector2Equal(object[o].direction, Vector2Zero())) {
				object[o].direction = Vector2Normalize(object[o].direction);
			}

			object[o].position = Vector2Add(object[o].position,
					Vector2MultiplyV(object[o].speed, object[o].direction));
		}
	}

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		for (int i = 0; i < MAX_SHOTS; i++) {
			if (!projectile[i].isActive) {
				projectile[i].isActive = true;
				projectile[i].position = ball.position;
				projectile[i].direction = Vector2Subtract(pointer.position, projectile[i].position);
				if (!Vector2Equal(projectile[i].direction, Vector2Zero())) {
					projectile[i].direction = Vector2Normalize(projectile[i].direction);
				}
				projectile[i].position = Vector2Add(ball.position, 
						Vector2MultiplyV(projectile[i].direction, Vector2{ ball.radius, ball.radius}));
				break;
			}
		}
	}

	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
		ball.position.x -= ball.speed.x;
	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
		ball.position.x += ball.speed.x;
	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
		ball.position.y -= ball.speed.y;
	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
		ball.position.y += ball.speed.y;

	for (int i = 0; i < MAX_SHOTS; i++) {
		if (projectile[i].isActive) {
			projectile[i].position = Vector2Add(projectile[i].position, 
					Vector2MultiplyV(projectile[i].speed, projectile[i].direction));
			if (projectile[i].position.x > SCREEN_WIDTH + projectile[i].radius)
				projectile[i].isActive = false;
			else if (projectile[i].position.x < 0 - projectile[i].radius)
				projectile[i].isActive = false;
			if (projectile[i].position.y > SCREEN_HEIGHT + projectile[i].radius)
				projectile[i].isActive = false;
			else if (projectile[i].position.y < 0 - projectile[i].radius)
				projectile[i].isActive = false;
		}
	}

}

void DrawGame() {
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawCircleV(ball.position, ball.radius, ball.color);
	DrawCircleV(pointer.position, pointer.radius, pointer.color);
	for(int o = 0; o < MAX_OBJECTS; o++) {
		if (object[o].isActive)
			DrawCircleV(object[o].position, object[o].radius, object[o].color);
	}
	for (int i = 0; i < MAX_SHOTS; i++) {
		if (projectile[i].isActive) {
			DrawCircleV(projectile[i].position, projectile[i].radius, projectile[i].color);
		}
	}
	DrawText(FormatText("Score: %08i", score), 10, 10, 20, RED);
	DrawFPS(SCREEN_WIDTH - 100, 10);
	EndDrawing();
}

void DrawGameOver() {
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawText(FormatText("Game Over, Your score is %i", score), 250, 200, 20, GREEN);
	DrawFPS(SCREEN_WIDTH - 100, 10);
	EndDrawing();
}
