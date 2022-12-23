#include "graphic.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
typedef void (*void_f)(sdlObject);
#define call_function(a,...) ((void_f)a)(__VA_ARGS__)
typedef void (*void_f_2)(sdlObject,sdlObject);
#define call_function_2(a,...) ((void_f_2)a)(__VA_ARGS__)
void draw(sdlObject head , SDL_Surface *window_surface){
    sdlObject temp = head;
    while (temp != NULL)
    {
        if (temp->obj->visible)
        {
            SDL_BlitSurface(temp->obj->image, NULL, window_surface, &(temp->obj->pos));
        }
        temp = temp->next;
    }
}

sdlObject new_node(information *obj , void *on_click , void *on_mouse_enter , void *on_mouse_exit , void *on_intersect){
    sdlObject head = (sdlObject )malloc(sizeof(Drawable));
    head->next = NULL;
    head->prev = NULL;
    head->obj = obj;
    head->obj->tag = 0;
    head->on_click = on_click;
    head->on_mouse_enter = on_mouse_enter;
    head->on_mouse_exit = on_mouse_exit;
    head->on_intersect = on_intersect;
    return head;
}

void add_object(sdlObject head , information *obj , void *on_click , void *on_mouse_enter , void *on_mouse_exit ,void *on_intersect){
    sdlObject temp = head;
    int cnt = 0;
    while (temp->next != NULL)
    {
        temp = temp->next;
        cnt++;
    }
    temp->next = (sdlObject )malloc(sizeof(Drawable));
    temp->next->obj = obj;
    temp->next->obj->tag = cnt+1;
    temp->next->next = NULL;
    temp->next->on_click = on_click;
    temp->next->on_mouse_enter = on_mouse_enter;
    temp->next->on_mouse_exit = on_mouse_exit;
    temp->next->on_intersect = on_intersect;
}

void remove_object(sdlObject head , information *obj){
    sdlObject temp = head;
    while (temp->next != NULL)
    {
        if (temp->next->obj == obj)
        {
            sdlObject temp2 = temp->next;
            temp->next = temp->next->next;
            free(temp2);
            return;
        }
        temp = temp->next;
    }
}
void free_list(sdlObject head){
    sdlObject temp = head;
    while (temp->next != NULL)
    {
        sdlObject temp2 = temp->next;
        temp->next = temp->next->next;
        free(temp2);
    }
    free(head);
}

SDL_Surface *load_image(char const *path)
{
    SDL_Surface *image_surface = IMG_Load(path);
    if(!image_surface)
        return 0;
    return image_surface;
}
void clear_surface(SDL_Surface *window_surface){
    SDL_FillRect(window_surface, NULL, SDL_MapRGB(window_surface->format, 0, 0, 0));
}
void Init(){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
        exit(-1);
    }
    if(IMG_Init(IMG_INIT_PNG)<0){
        printf("sdl_png");
        exit(-1);
    }
}
information create_new_information(char *img_path, int x , int y , void *info){
    information lol={.entered = false,.visible = true,.pos = { .x = x , .y = y},.image = load_image(img_path), .data = (void*)info ,.enterable=true,.intersected=false};
    return lol;
}
void update(SDL_Surface* window_surface , SDL_Window *window){
    SDL_UpdateWindowSurface(window);
    SDL_Delay(5);
    clear_surface(window_surface);
        
}
void draw_and_update(SDL_Surface *window_surface , SDL_Window *window, sdlObject head){
    draw(head , window_surface);
    update(window_surface , window);
}

void check_for_clicks(sdlObject head , int x, int y){
    sdlObject seek = head;
    while(seek){
        if (seek->on_click){
            if(x <= (seek->obj->pos.x + seek->obj->pos.w) && x >= seek->obj->pos.x && y <= (seek->obj->pos.y + seek->obj->pos.h) && y>= seek->obj->pos.y){
                call_function(seek->on_click , seek);
            }
        }
        seek = seek->next;
    }
}
bool is_mouse_inside_Surface(int mousex , int mousey , SDL_Rect Rect){
    if(mousex >= Rect.x && mousex<=Rect.x + Rect.w && mousey >= Rect.y && mousey<=Rect.y + Rect.h)
        return true;
    return false;
}
int entered(int mousex, int mousey , sdlObject obj){
    bool inside;
    inside = is_mouse_inside_Surface(mousex, mousey , obj->obj->pos);
    if((!(obj->obj->entered)) && inside){
        obj->obj->entered = true;
        if(obj->obj->enterable){
            return 1;
        }
    }
    else if((obj->obj->entered) && (!inside)){
        obj->obj->entered = false;
        return -1;
    }
    return 0;
}

void check_for_enters(sdlObject head, int mousex , int mousey){
    sdlObject seek = head;
    while(seek!=NULL){
        int state = entered(mousex, mousey , seek);
        if(state == 1){
            if(seek->on_mouse_enter){
                call_function(seek->on_mouse_enter , seek);
            }
        }
        else if(state == -1){
            if(seek->on_mouse_exit){
                call_function(seek->on_mouse_exit , seek);
            }
        }
        seek = seek->next;
    }
}
bool intersect(sdlObject obj1 , sdlObject obj2){
    int x1,y1,w1,h1;
    int x2,y2,w2,h2;
    x1 = obj1->obj->pos.x;
    y1 = obj1->obj->pos.y;
    x2 = obj2->obj->pos.x;
    y2 = obj2->obj->pos.y;
    w1 = obj1->obj->pos.w;
    w2 = obj2->obj->pos.w;
    h1 = obj1->obj->pos.h;
    h2 = obj2->obj->pos.h;
    // check for intersection
    return !((x1+w1<x2 || x1 > x2+w2 || y1+h1 < y2 || y2+h2 < y1));
}

sdlObject get_intersect(sdlObject head, sdlObject obj){
    sdlObject seek = head;
    while(seek){
        if (seek->obj != obj->obj){
            if(intersect(obj, seek)){
                return seek;
            }
        }
        seek = seek->next;
    }
    return NULL;
}

void check_for_intersects(sdlObject head){
    sdlObject seek = head;
    while(seek){
        sdlObject seek2 = seek->next;
        while(seek2){
            //use intersected to check if the object is already intersected
            if(intersect(seek, seek2)){
                if(seek->on_intersect){
                    if(!seek->obj->intersected){
                        call_function_2(seek->on_intersect , seek, seek2);
                        seek->obj->intersected = true;
                    }
                }
                if(seek2->on_intersect){
                    if(!seek2->obj->intersected){
                        call_function_2(seek2->on_intersect , seek2 , seek);
                        seek2->obj->intersected = true;
                    }
                }
            }
            else{
                seek->obj->intersected = false;
                seek2->obj->intersected = false;
            }

            seek2 = seek2->next;
        }
        seek = seek->next;
    }
}

void free_nodes(sdlObject head){
    sdlObject temp = head;
    while(temp->next){
        sdlObject temp2 = temp->next;
        temp->next = temp->next->next;
        free(temp2);
    }
    free(head);
}