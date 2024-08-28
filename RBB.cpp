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
    RotatingRect::Rect2D test(300,300,100,100);
    RotatingRect::Rect2D test2(200,200,100,100);

    float x = 0;
    float y = 0;
    float theta_1 = 0;
    float theta_2 = 360;
    int theta_1_sign = 1;
    int theta_2_sign = 1;

    //test.rotate_rect(theta_1);
    float move_speed = 10;
    //test2.rotate_rect(theta_2);

    struct move {
        bool left;
        bool right;
        bool up;
        bool down;
        move(){left = false; right = false; up = false; down = false;}
    };
    move input;
    int just_pressed = -1;
    int last_pressed = 0;
    bool collision_detected = false;
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
                input.up = true;
                just_pressed = 1;
                break;
                case SDLK_DOWN:
                input.down = true;
                just_pressed = 2;
                break;
                case SDLK_LEFT:
                input.left = true;
                just_pressed = 3;
                break;
                case SDLK_RIGHT:
                input.right = true;
                just_pressed = 4;
                break;
            }
            break;

        case SDL_KEYUP:
            switch(events.key.keysym.sym)
            {
                case SDLK_UP:
                input.up = false;
                break;
                case SDLK_DOWN:
                input.down = false;
                break;
                case SDLK_LEFT:
                input.left = false;
                break;
                case SDLK_RIGHT:
                input.right =false;
                break;
            }
            break;
	    }           	
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);	
        if (!collision_detected || last_pressed != just_pressed){

            if (input.right)
            {
                x += 1;
            } 
            if (input.left)
            {
                x -= 1;
            }
            if (input.up){
                y -= 1;
            } 
            if (input.down)
            {
                y += 1;
            }
        }


            test2.set_position(x,y);
        

        
        test.rotate_rect(theta_1);
        test2.rotate_rect(theta_2);

        if (test2.Rect2DIntersect(test) )
            theta_2_sign *= -1;
        
        
        if (test.Rect2DIntersect(test2))
        {
            collision_detected = true;
            theta_1_sign *= -1;
        } else{
            collision_detected = false;
        }


        test.draw_rect(renderer);
        test2.draw_rect(renderer);
  
        usleep(0.01667 * pow(10,6) );
      
        theta_1 += 2*theta_1_sign;
        if (theta_1 > 360) theta_1 = 0;

        theta_2 -= 1*theta_2_sign;
        if (theta_2 < 0) theta_2 = 360;

    
	    SDL_RenderPresent(renderer);

        if (just_pressed != last_pressed)
        {
            last_pressed = just_pressed;
        }
    }

    

}