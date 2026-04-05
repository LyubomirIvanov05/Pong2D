#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct Ball {
  float x;
  float y;
  bool ballMoveLeft;
  bool ballMoveRight;
  bool ballMoveUp;
  bool ballMoveDown;
  int radius;
};

struct Player {
  float x;
  float y;
  int w;
  int h;
  bool MoveUp;
  bool MoveDown;
};

struct Computer {
  float x;
  float y;
  int w;
  int h;
  bool MoveUp;
  bool MoveDown;
};


void BallCollision(struct Ball *ball) {
  // CHECK FOR COLLISION BETWEEN BALL AND WIDNOW
  if (ball -> x <= 0) {
    ball -> x = 0;
    ball -> ballMoveLeft = false;
    ball -> ballMoveRight = true;
  }
  if (ball -> x > WINDOW_WIDTH - ball -> radius * 2) {
    ball -> x = WINDOW_WIDTH - ball -> radius * 2;
    ball -> ballMoveRight = false;
    ball -> ballMoveLeft = true;
  }
  if (ball -> y <= 0) {
    ball -> y = 0;
    ball -> ballMoveUp = false;
    ball -> ballMoveDown = true;
  }
  if (ball -> y > WINDOW_HEIGHT - ball -> radius * 2) {
    ball -> y = WINDOW_HEIGHT - ball -> radius * 2;
    ball -> ballMoveDown = false;
    ball -> ballMoveUp = true;
  }
}

void BallMovement(struct Ball *ball, float velocity, Uint32 delta_time) {

    // MOVE BALL
    if (ball -> ballMoveUp) {
      ball -> y -= velocity * delta_time;
    }
    if (ball -> ballMoveDown) {
      ball -> y += velocity * delta_time;
    }
    if (ball -> ballMoveLeft) {
      ball -> x -= velocity * delta_time;
    }
    if (ball -> ballMoveRight) {
      ball -> x += velocity * delta_time;
    }
}

void PlayerMovement(struct Player *player, float velocity, Uint32 delta_time) {
   // MOVE PLAYER
   if (player -> MoveUp) {
      player -> y -= velocity * delta_time;
    }
    if (player -> MoveDown) {
      player -> y += velocity * delta_time;
    }
}

void PlayerCollision(struct Player *player) {
  if (player -> y <= 0) {
    player -> y = 0;
  }
  if (player -> y + player -> h > WINDOW_HEIGHT){
    player -> y = WINDOW_HEIGHT - player -> h;
  }
}

void ComputerMovement(struct Computer *computer, float computer_velocity, Uint32 delta_time, struct Ball *ball) {
  if(ball->x >= WINDOW_WIDTH / 2 && ball->x <= computer->x){
    if(ball -> y + ball -> radius > computer -> y + computer -> h / 2) {
      computer -> y += computer_velocity * delta_time;
    }

    if(ball -> y + ball -> radius < computer -> y + computer -> h / 2) {
      computer -> y -= computer_velocity * delta_time;
    }
  }
}

void ComputerCollision(struct Computer *computer) {
  if(computer -> y <= 0) {
    computer -> y = 0;
  }
  if(computer -> y + computer -> h > WINDOW_HEIGHT) {
    computer -> y = WINDOW_HEIGHT - computer -> h;
  }
}

void ComputerAndBallCollision(struct Computer *computer, struct Ball *ball) {
  if(ball->x + ball->radius * 2 >= computer -> x && ball -> y >= computer -> y && ball->y <= computer->y + computer->h) {
    ball->x = computer->x - ball->radius * 2;
    ball->ballMoveRight = false;
    ball->ballMoveLeft = true;
  }
}

void PlayerAndBallCollision(struct Player *player, struct Ball *ball) {
    // BALL AND RECTANGLE COLLISIONS
    if (ball -> x <= player -> x + ball -> radius * 2 && ball -> y >= player -> y && ball -> y <= player -> y + player -> h) {
      ball -> x = player -> x + 20;
      ball -> ballMoveLeft = false;
      ball -> ballMoveRight = true;
    }

}


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
  float velocity = 0.3;
  float computer_velocity = 0.1;
  SDL_Event event;
  Uint32 last_time = SDL_GetTicks();

  struct Ball ball;
  ball.x = WINDOW_WIDTH / 2;
  ball.y = WINDOW_HEIGHT / 2;
  ball.ballMoveDown = false;
  ball.ballMoveUp = false;
  ball.ballMoveLeft = false;
  ball.ballMoveRight = false;
  ball.radius = 10;

  struct Player player;
  player.x = 50;
  player.y = 50;
  player.w = 20;
  player.h = 200;
  player.MoveUp = false;
  player.MoveDown = false;

  struct Computer computer;
  computer.x = WINDOW_WIDTH - 50;
  computer.y = 50;
  computer.w = 20;
  computer.h = 200;
  computer.MoveUp = false;
  computer.MoveDown = false;


  int randomY = (rand() % 2) + 1;
  int randomX = (rand() % 2) + 1;
  if (randomX == 1) {
    ball.ballMoveLeft = true;
  } else {
    ball.ballMoveRight = true;
  }

  if (randomY == 1) {
    ball.ballMoveUp = true;
  } else {
    ball.ballMoveDown = true;
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
        player.MoveDown = true;
      }
      if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_s) {
        player.MoveDown = false;
      }
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_w) {
        // GO UP
        player.MoveUp = true;
      }
      if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_w) {
        player.MoveUp = false;
      }
    }

    // 2. Update game state
    // TODO: move paddles, move ball, check collisions

   
    PlayerMovement(&player, velocity, delta_time);
    PlayerCollision(&player);
    BallMovement(&ball, velocity, delta_time);
    BallCollision(&ball);
    PlayerAndBallCollision(&player, &ball);
    ComputerMovement(&computer, computer_velocity, delta_time, &ball);
    ComputerCollision(&computer);
    ComputerAndBallCollision(&computer, &ball);
  
    // 3. Render
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black background
    SDL_RenderClear(renderer);

    // TODO: draw paddles and ball here using SDL_RenderFillRect()
    // PLAYER
    SDL_Rect rect;
    rect.x = player.x;
    rect.y = player.y;
    rect.w = player.w;
    rect.h = player.h;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    // BALL
    SDL_Rect ball_rect;
    ball_rect.x = ball.x;
    ball_rect.y = ball.y;
    ball_rect.w = ball.radius * 2;
    ball_rect.h = ball.radius * 2;

    SDL_SetRenderDrawColor(renderer, 125, 200, 16, 75);
    SDL_RenderFillRect(renderer, &ball_rect);

    //COMPUTER
    SDL_Rect computer_rect;
    computer_rect.x = computer.x;
    computer_rect.y = computer.y;
    computer_rect.w = computer.w;
    computer_rect.h = computer.h;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    SDL_RenderFillRect(renderer, &computer_rect);

    SDL_RenderPresent(renderer); // show what we drew
  }

  // Clean up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
