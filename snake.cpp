// PROJECT REPO -- https://github.com/ponkeee/raylib-snake.cpp
#include <iostream>
#include <vector>
#include <array>
#include "raylib.h"


int fps = 7; // since this is a grid-based game FPS also means speed but decreasing fps will increase input delay and even input misses 
int state = 0; // (scenes) 0 Game, 1 Loss, 2 Won
int currentLength = 1;
int rows = 50;
int cols = 50;
int wWidth = 800;
int wHeight = 800;
int PosX; // I was using normal ints instead of vectors or variables, because i literally have zero knowledge about how vectors and array methods are there in c++, most things i used was just searching and copy/pasting with a little bit of understanding for sure.
int PosY;
int colGrid = wWidth / cols;
int rowGrid = wHeight / rows;
int direction = 0; // 0 RIGHT, 1, LEFT, 2 DOWN, 3 UP
std::vector<std::array<int, 2>> previous;
int maxLength = 256; // note that more length = more ram usage.. also you can set maxLength to be ((row * cols) - 1) for full eating (snake becomes as big as the window goes)
int FoodX;
int FoodY;
std::vector<std::array<int, 2>> collision;

// Loss and Win scenes related variables
int fontSize = 32;
int sPadding = 10;


int main(void){
	srand(time(nullptr)); // so rand() function becomes different each time it launches not each compile time
	FoodX = rand() % (cols - 1); // (cols - 1) to limit it to one number lesser than cols to not make it render outside the canvas
	FoodY = rand() % (rows - 1);
    InitWindow(wWidth,wHeight, "snake");
    SetTargetFPS(fps);
      while (!WindowShouldClose()){ // game loop begins
      BeginDrawing();
        if(state == 0){ // game scene
        	if(currentLength == maxLength){
        		state = 2;
        	}
           ClearBackground(RAYWHITE);
           DrawRectangle(FoodX * colGrid, FoodY * rowGrid, colGrid, rowGrid, RED);
           DrawRectangle(PosX * colGrid, PosY * rowGrid,colGrid, rowGrid, GRAY); // drawing head of snake
			size_t start = previous.size() > size_t(currentLength)
			    ? previous.size() - currentLength
			    : 0;
			for (size_t i = start; i < previous.size(); i++) { // drawing tail
			    DrawRectangle(
			        previous[i][0] * colGrid,
			        previous[i][1] * rowGrid,
			        colGrid,rowGrid, BLACK);
			        collision.push_back({previous[i][0], previous[i][1]}); // pushes positions of the current tail to the collision vector so it can be checked
			}
			for(int i = 0; i < int(collision.size()); i++){ // checking collisions of snake tail
				if(collision[i][0] == PosX && collision[i][1] == PosY){
					state = 1;
				}
			}
			if(FoodX == PosX && FoodY == PosY){ 
				currentLength++; // when colliding with Food increase length
				FoodX = rand() % (cols - 1);
				FoodY = rand() % (rows - 1); // then change their x and y coords to a random place inside the cols/rows range
			}
			if(int(previous.size()) >= maxLength){previous.erase(previous.begin());} // making maxLength as a limit so it doesn't endlessly store arrays in the memory
           previous.push_back({PosX, PosY});
			switch (direction){ // handling movement based on direction
				case 0:
				PosX++;
				break;
				case 1:
				PosX--;
				break;
				case 2:
				PosY++;
				break;
				case 3:
				PosY--;
			}
           if(IsKeyPressed(KEY_D) && direction != 1){ // input handling begins
           	direction = 0;
           }
           else if(IsKeyPressed(KEY_A) && direction != 0){
           	direction = 1;
           }
           else if(IsKeyPressed(KEY_S) && direction != 3){
           	direction = 2;
           }
           else if (IsKeyPressed(KEY_W) && direction != 2){
           	direction = 3;
           }
           if(PosX >= cols){ // canvas loop (if snake got out of canvas get it back)
           	PosX = 0;
           }
           else if(PosX < 0){
           	PosX = cols;
           }
           else if(PosY >= rows){
           	PosY = 0;
           }
           else if(PosY < 0){
           	PosY = rows;
           }
           collision.clear();
           // debugging stuff
           // std::cout << FoodX << " , " << FoodY << std::endl; 
           // std::cout << std::endl << previous[previous.size() - 1][0] << "  " << previous[previous.size() - 1][1] << std::endl;
    }
    else if (state == 1){ // loss
    ClearBackground(RAYWHITE);
	DrawText("You lost!", (wWidth / 2) - fontSize, (wHeight / 2) - fontSize, fontSize, RED);
	DrawText("Press Y to retry", sPadding , wHeight - sPadding - fontSize ,fontSize, GRAY);
	DrawText("Press N or ESC to quit", sPadding, wHeight - sPadding - fontSize * 2, fontSize, GRAY);
	if(IsKeyPressed(KEY_Y)){currentLength = 1;state = 0;}
	else if(IsKeyPressed(KEY_N)){EndDrawing();break;} // breaks out of the game loop
    }
    else if (state == 2){ // win
    ClearBackground(RAYWHITE);
	DrawText("You won!", (wWidth / 2) - fontSize, (wHeight / 2) - fontSize, fontSize, GREEN);
	DrawText("Press Y to retry", sPadding , wHeight - sPadding - fontSize ,fontSize, GRAY);
	DrawText("Press N or ESC to quit", sPadding, wHeight - sPadding - fontSize * 2, fontSize, GRAY);
	if(IsKeyPressed(KEY_Y)){currentLength = 1;state = 0;}
	else if(IsKeyPressed(KEY_N)){EndDrawing();break;} // breaks out of the game loop
    }
    EndDrawing();
   }
    CloseWindow();
}
