#include <SDL2/SDL.h>
#include <stdbool.h>
#include<SDL2/SDL_image.h>
#define voidstarize(a) (void*)&a
#define unvoidstarize(a , type) *((type*)a)
typedef struct information
{
    int tag;
    SDL_Surface *image;
    SDL_Rect pos;
    bool entered;
    bool visible;
    bool enterable;
    bool intersected;
    void *data;
} information;
typedef struct Drawable
{
    information *obj;
    void *on_click;
    void *on_mouse_enter;
    void *on_mouse_exit;
    void *on_intersect;
    struct Drawable *next;
    struct Drawable *prev;
} Drawable;
typedef Drawable* sdlObject;

void draw(sdlObject head , SDL_Surface *window_surface);
void add_object(sdlObject head , information *obj , void *on_click , void *on_mouse_enter , void *on_mouse_exit ,void *on_intersect);
information create_new_information(char *img_path, int x , int y , void *info);
void remove_object(sdlObject head , information *obj);
void free_list(sdlObject head);
SDL_Surface *load_image(char const *path);
void clear_surface(SDL_Surface *window_surface);
void Init();
void update(SDL_Surface* window_surface , SDL_Window *window);
sdlObject new_node(information *obj , void *on_click , void *on_mouse_enter, void *on_mouse_exit , void *on_intersect);
void draw_and_update(SDL_Surface *window_surface , SDL_Window *window, sdlObject head);
void check_for_clicks(sdlObject head , int x, int y);
void check_for_enters(sdlObject head, int mousex , int mousey);
bool intersect(sdlObject obj1 , sdlObject obj2);
void check_for_intersects(sdlObject head);
void free_nodes(sdlObject head);