#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// struct Paddle {
//   int width;
//   int height;
// }

int main(void) {
  // Initialize SDL's video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  // Create a window
  SDL_Window *window =
      SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if (!window) {
    fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  // Create a renderer (this is what you draw with)
  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  // Game loop
  int running = 1;
  SDL_Event event;
  float velocity = 0.3;
  float rect_y = 50;
  float rect_x = 50;
  bool moveUp = false;
  bool moveDown = false;
  Uint32 last_time = SDL_GetTicks();

  float ball_x = WINDOW_WIDTH / 2;
  float ball_y = WINDOW_HEIGHT / 2;
  bool ballMoveUp = false;
  bool ballMoveLeft = false;
  bool ballMoveRight = false;
  bool ballMoveDown = false;
  int radius = 10;

  int randomY = (rand() % 2) + 1;
  int randomX = (rand() % 2) + 1;
  if (randomX == 1) {
    ballMoveLeft = true;
  } else {
    ballMoveRight = true;
  }

  if (randomY == 1) {
    ballMoveUp = true;
  } else {
    ballMoveDown = true;
  }

  while (running) {
    // 1. Handle input
    Uint32 current_time = SDL_GetTicks();
    Uint32 delta_time = current_time - last_time;
    last_time = current_time;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = 0;
      }
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        running = 0;
      }
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s) {
        // GO DOWN
        moveDown = true;
      }
      if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_s) {
        moveDown = false;
      }
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_w) {
        // GO UP
        moveUp = true;
      }
      if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_w) {
        moveUp = false;
      }
    }

    // 2. Update game state
    // TODO: move paddles, move ball, check collisions

    // MOVE PLAYER
    if (moveUp) {
      rect_y -= velocity * delta_time;
    }
    if (moveDown) {
      rect_y += velocity * delta_time;
    }

    // MOVE BALL
    if (ballMoveUp) {
      ball_y -= velocity * delta_time;
    }
    if (ballMoveDown) {
      ball_y += velocity * delta_time;
    }
    if (ballMoveLeft) {
      ball_x -= velocity * delta_time;
    }
    if (ballMoveRight) {
      ball_x += velocity * delta_time;
    }

    // CHECK FOR COLLISION BETWEEN BALL AND WIDNOW
    if (ball_x - radius <= 0) {
      printf("COLLISION ON 1\n");
      ballMoveLeft = false;
      ballMoveRight = true;
    }
    if (ball_x + radius >= WINDOW_WIDTH - radius) {
      printf("COLLISION ON 2\n");
      printf("ball_x = %f \n", ball_x);
      ballMoveRight = false;
      ballMoveLeft = true;
    }
    if (ball_y - radius <= 0) {
      ballMoveUp = false;
      ballMoveDown = true;
      printf("COLLISION ON 3\n");
    }
    if (ball_y + radius >= WINDOW_HEIGHT - radius) {
      printf("COLLISION ON 4\n");
      printf("ball_y = %f \n", ball_y);
      ballMoveDown = false;
      ballMoveUp = true;
    }

    if (ball_x - radius == rect_x && ball_y - radius == rect_y) {
      printf("COLLISION DETECTED X \n");
    }

    // 3. Render
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black background
    SDL_RenderClear(renderer);

    // TODO: draw paddles and ball here using SDL_RenderFillRect()
    // PLAYER
    SDL_Rect rect;
    rect.x = rect_x;
    rect.y = rect_y;
    rect.w = 20;
    rect.h = 200;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    // BALL
    SDL_Rect ball;
    ball.x = ball_x;
    ball.y = ball_y;
    ball.w = radius * 2;
    ball.h = radius * 2;

    SDL_SetRenderDrawColor(renderer, 125, 200, 16, 75);
    SDL_RenderFillRect(renderer, &ball);

    SDL_RenderPresent(renderer); // show what we drew
  }

  // Clean up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
