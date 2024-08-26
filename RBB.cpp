#include <iostream>
#include "unistd.h"
#include "RotatingRect.hpp"

int main(){

    // setup some sdl
    int width = 800;
    int height = 800;
    int bpp = 0;
    SDL_Surface* screen;
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Event events;

    bool quit;
    float theta = 0;
    RotatingRect::Rect2D test(400,400,400,200);
    RotatingRect::Rect2D test2(200,200,200,300);

    float x = 200;
    float y = 350;
    float theta_1 = 0;
    float theta_2 = 360;
    int theta_1_sign = 1;
    int theta_2_sign = 1;

    test.rotate_rect(theta_1);
    float move_speed = 10;
    test2.rotate_rect(theta_2);
    while (!quit)
    {
        SDL_PollEvent(&events);

        switch(events.type)
	    {
		case SDL_QUIT:
			quit = true;
			break;
        case SDL_KEYDOWN:
			switch(events.key.keysym.sym){
			
                case SDLK_UP:
                y -= move_speed;
                break;
                case SDLK_DOWN:
                y += move_speed;
                break;
                case SDLK_LEFT:
                x -= move_speed;
                break;
                case SDLK_RIGHT:
                x += move_speed;
                break;
            }
            break;
	    }           	
 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);	

        
        test2.set_position(x,y);
        
        test.rotate_rect(theta_1);
        test2.rotate_rect(theta_2);

        if (RotatingRect::Rect2DIntersect(test2,test) ){
            SDL_SetRenderDrawColor(renderer,255,0,0,255);
            theta_2_sign *= -1;
        } else{
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        test2.draw_rect(renderer);
        
        if (RotatingRect::Rect2DIntersect(test,test2))
        {
            SDL_SetRenderDrawColor(renderer,0,255,0,255);
            theta_1_sign *= -1;
        } else{
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        test.draw_rect(renderer);
        
        usleep(0.01667 * pow(10,6) );
      
        theta_1 += 1*theta_1_sign;
        if (theta_1 > 360) theta_1 = 0;

        theta_2 -= 1*theta_2_sign;
        if (theta_2 < 0) theta_2 = 360;

    
	    SDL_RenderPresent(renderer);
    }

    

}