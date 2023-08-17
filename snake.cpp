#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <deque>

int main(){
	enum Direction{
		DOWN,
		UP,
		LEFT,
		RIGHT
	};
	int dir;
	int size = 1;
	bool running = true;
	SDL_Rect square = {10, 10, 10, 10};
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	
	std::deque<SDL_Rect> rq;
	std::vector<SDL_Rect> apples;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);

	//Create apples
	//
	for(int i = 0; i < 100; i++){
		apples.emplace_back(rand()%50*10, rand()%50*10, 10, 10);
	}
	
	int secs = 0;
	while(running){
		// input
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT)
				running = false;
			if(e.type == SDL_KEYDOWN){
				if(e.key.keysym.sym == SDLK_DOWN) {dir = DOWN;}
				if(e.key.keysym.sym == SDLK_UP) {dir = UP;}
				if(e.key.keysym.sym == SDLK_LEFT) {dir = LEFT;}
				if(e.key.keysym.sym == SDLK_RIGHT) {dir = RIGHT;}
			}
		}

		switch(dir){
			case DOWN:
				square.y += 10;
				break;
			case UP:
				square.y -= 10;
				break;
			case LEFT:
				square.x -= 10;
				break;
			case RIGHT:
				square.x += 10;
				break;
		}

		// Collision
		//
		std::for_each(apples.begin(), apples.end(), [&](auto& apple){
			if(square.x == apple.x && square.y == apple.y){
				size += 10;
				apple.x = -10;
				apple.y = -10;
			}
		});

		std::for_each(rq.begin(), rq.end(), [&](auto& snake_segment){
			if(square.x == snake_segment.x && square.y == snake_segment.y){
				size = 1;		
			}
		});

		rq.push_front(square);
		while(rq.size() > size)
			rq.pop_back();

		// render head
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		// draw body
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		std::for_each(rq.begin(), rq.end(), [&](auto& snake_segment){
				SDL_RenderFillRect(renderer, &snake_segment);
		});

		// draw apples
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		std::for_each(apples.begin(), apples.end(), [&](auto& apple){
			SDL_RenderFillRect(renderer, &apple);	
		});

		SDL_RenderPresent(renderer);
		SDL_Delay(25);
	}
}
