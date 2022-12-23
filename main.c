#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "gbase/graphic.h"

/*
//Functions for mouse click should be like this :
    void mouse_clicked(sdlObject obj){ //obj is the object that was clicked
        ...
    }

//Functions for mouse enter and exit should be like this :
    void mouse_entered(sdlObject obj){ //obj is the object that was entered by mouse
        ...
    }
    void mouse_exit(sdlObject obj){ //obj is the object that was exited by mouse
        ...
    }

//Functions for intersection should be like this :
    void intersected(sdlObject obj, sdlObject intersected_with){ //obj is our object and intersected_with is the object that intersected with our object
        ...
    }
*/



sdlObject scene;
sdlObject scene2;
sdlObject scene3;

void image_clicked(sdlObject obj){
    printf("You clicked on me! %d\n" , obj->obj->tag);
    scene = scene3;
}
void image_mouse_entered(sdlObject obj){
    printf("Entered %d\n" , obj->obj->tag);
}
void image_mouse_exit(sdlObject obj){
    printf("Exited %d\n" , obj->obj->tag);
}

void image_clicked_2(sdlObject obj){
    printf("You clicked on me! %d\n" , obj->obj->tag);
}
void image_mouse_entered_2(sdlObject obj){
    printf("Entered %d\n" , obj->obj->tag);
}
void image_mouse_exit_2(sdlObject obj){
    printf("Exited %d\n" , obj->obj->tag);
}

void image_intersect(sdlObject obj , sdlObject intersected_with){
    printf("Intersected with : %d\n" , intersected_with->obj->tag);
    obj->obj->visible = false;
}

void image_intersect_2(sdlObject obj , sdlObject intersected_with){
    printf("Intersected with : %d\n" , intersected_with->obj->tag);
}

int main(int argsv, char *args[]){
    //--------------Initialize SDL-----------------
    SDL_Window *window;SDL_Surface *surface;SDL_Event event;int quit = 0;Init();
    window = SDL_CreateWindow("Hello World!", 100, 100, 1360, 780, SDL_WINDOW_SHOWN);
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    //--------------Create Objects-----------------
    information lol = create_new_information("test.png" , 0 , 200 , NULL);
    information lol2 = create_new_information("img.jpeg" , 400 , 0 , NULL);
    sdlObject scene2 = new_node(&lol , image_clicked , image_mouse_entered , image_mouse_exit, image_intersect);
    add_object(scene2 , &lol2 , image_clicked_2 , image_mouse_entered_2 , image_mouse_exit_2, image_intersect_2);
    

    information _lol = create_new_information("test.png" , 400 , 600 , NULL);
    information _lol2 = create_new_information("test.png" , 400 , 400 , NULL);
    scene3 = new_node(&_lol , image_clicked , image_mouse_entered , image_mouse_exit, image_intersect);
    add_object(scene3 , &_lol2 , image_clicked_2 , image_mouse_entered_2 , image_mouse_exit_2, image_intersect_2);
    

    //--------------Main Loop-----------------
    scene = scene2;
    int mousex=-100 , mousey=-100;
    while(!quit){
        check_for_enters(scene, mousex, mousey);
        check_for_intersects(scene);
        

        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_MOUSEMOTION:
                    mousex = event.motion.x;
                    mousey = event.motion.y;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT)
                        check_for_clicks(scene, mousex, mousey);
                break;
            }
        }
        lol.pos.x++;
        draw_and_update(window_surface , window , scene);
    }
    //--------------Free Memory-----------------
    SDL_DestroyWindow(window);
    SDL_Quit();
    free_nodes(scene2);
    free_nodes(scene3);
    
    return 0;
}