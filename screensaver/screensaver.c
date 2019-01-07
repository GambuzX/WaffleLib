#include <lcom/lcf.h>
//#include <stdlib.h>
#include <math.h>
#include "screensaver.h"
#include "vbe.h"
#include "window/window.h"

#include "waffle_xpm.h"
#include "orange_juice_xpm.h"
#include "egg_xpm.h"
#include "bacon_xpm.h"
#include "screensaver_background.h"

#include "util.h"

static uint8_t * waffle[8], *egg[1], *orange_juice[1], *bacon[1];
static uint8_t *screensaver_back;
static uint8_t bytes_per_pixel;
static ScreensaverEle * screensaver_elements[SCREENSAVER_NUMBER_OF_ELEMENTS];
static int currentElements = 0;

int initialize_screensaver() {
    /* Check if already was initialized */
    static bool hasInit = false;
    if (hasInit)
        return OK;

    /* Get bytes per pixel from vbe */
    bytes_per_pixel = get_bytes_per_pixel();

    /* Load the waffle xpms */
    xpm_image_t img;
    uint8_t * sprite = xpm_load(waffle_xpm_1, XPM_8_8_8_8, &img);
    if (sprite == NULL)
        return 1;
    waffle[0] = sprite;

    sprite = xpm_load(waffle_xpm_2, XPM_8_8_8_8, &img);
    if (sprite == NULL)
        return 1;
    waffle[1] = sprite;

    sprite = xpm_load(waffle_xpm_3, XPM_8_8_8_8, &img);
    if (sprite == NULL)
        return 1;
    waffle[2] = sprite;

    sprite = xpm_load(waffle_xpm_4, XPM_8_8_8_8, &img);
    if (sprite == NULL)
        return 1;
    waffle[3] = sprite;

    sprite = xpm_load(waffle_xpm_5, XPM_8_8_8_8, &img);
    if (sprite == NULL)
        return 1;
    waffle[4] = sprite;

    sprite = xpm_load(waffle_xpm_6, XPM_8_8_8_8, &img);
    if (sprite == NULL)
        return 1;
    waffle[5] = sprite;

    sprite = xpm_load(waffle_xpm_7, XPM_8_8_8_8, &img);
    if (sprite == NULL)
        return 1;
    waffle[6] = sprite;

    sprite = xpm_load(waffle_xpm_8, XPM_8_8_8_8, &img);
    if (sprite == NULL)
        return 1;
    waffle[7] = sprite;


    /* Load the egg xpm */
    sprite = xpm_load(egg_xpm, XPM_8_8_8_8, &img);
    if (sprite == NULL)
        return 1;
    egg[0] = sprite;
    
    /* Load the orange juice xpm */
    sprite = xpm_load(orange_juice_xpm, XPM_8_8_8_8, &img);
    if (sprite == NULL)
        return 1;
    orange_juice[0] = sprite;

    /* Load the bacon xpm */
    sprite = xpm_load(bacon_xpm, XPM_8_8_8_8, &img);
    if (sprite == NULL)
        return 1;
    bacon[0] = sprite;
    
    /* Load the screensaver background xpm */
    sprite = xpm_load(breakfast_background_xpm, XPM_8_8_8_8, &img);
    if (sprite == NULL)
        return 1;
    screensaver_back = sprite;

    /* Add elements to screensaver */
    add_element_to_screensaver(200, 200, WAFFLE_XPM_WIDTH, WAFFLE_XPM_HEIGHT, waffle, WAFFLE_N_FRAMES);
    add_element_to_screensaver(500, 200, EGG_XPM_WIDTH, EGG_XPM_HEIGHT, egg, EGG_N_FRAMES);
    add_element_to_screensaver(500, 600, BACON_XPM_WIDTH, BACON_XPM_HEIGHT, bacon, BACON_N_FRAMES);
    add_element_to_screensaver(720, 200, ORANGE_JUICE_XPM_WIDTH, ORANGE_JUICE_XPM_HEIGHT, orange_juice, ORANGE_JUICE_N_FRAMES);
    add_element_to_screensaver(900, 0, WAFFLE_XPM_WIDTH, WAFFLE_XPM_HEIGHT, waffle, WAFFLE_N_FRAMES);

    hasInit = true;

    return OK;
}

void free_screensaver() {
    free(waffle);
    free(egg);
    free(orange_juice);
    free(screensaver_back);

    for (int i = 0; i < currentElements; i++) {
        free(screensaver_elements[i]);
    }
}

void screensaver_draw() {

    draw_background(screensaver_back, SCREENSAVER_BACK_WIDTH, SCREENSAVER_BACK_HEIGHT);

    bool should_draw = false;
    bool objects_collided = false;
    int32_t j = 0;
    while(!should_draw && !objects_collided){
        should_draw = true;
        
        for (int i = 0; i < SCREENSAVER_NUMBER_OF_ELEMENTS; i++) {

            ScreensaverEle * scr_ele = screensaver_elements[i];
            if(scr_ele->final_pos) {
                continue;
            }


            if(scr_ele->x_move == 0){
                scr_ele->next_x = scr_ele->x;
            }
            else if(abs(scr_ele->x_move) > j){
                scr_ele->next_x = scr_ele->x + ( (scr_ele->x_move < 0) ? -1 : 1); 
                should_draw = false;
            }

            if(scr_ele->y_move == 0){
                scr_ele->next_y = scr_ele->y;
            }
            else if(abs(scr_ele->y_move) > j){
                scr_ele->next_y = scr_ele->y + ( (scr_ele->y_move < 0) ? -1 : 1); 
                should_draw = false;
            }

            /* element were not moved */
            if(scr_ele->next_x == scr_ele->x && scr_ele->next_y == scr_ele->y){
                scr_ele->final_pos = true;
                continue;
            }

            ScreensaverEle * collidingEle = check_collision_at_new_position(scr_ele);
            if(collidingEle){

                objects_collided = true;
                scr_ele->final_pos = true;
                collidingEle->final_pos = true;
                
                bool scr_neg_move_x = scr_ele->x_move < 0;
                bool scr_neg_move_y = scr_ele->y_move < 0;

                bool colliding_neg_move_x = collidingEle->x_move < 0;
                bool colliding_neg_move_y = collidingEle->y_move < 0;
                
                if(scr_neg_move_x == colliding_neg_move_x) {

                    if(scr_neg_move_x){
                        if(scr_ele->x < collidingEle->x){
                            collidingEle->x_move *= (-1);
                        }
                        else{
                            scr_ele->x_move *= (-1);
                        }
                    }
                    else{
                        if(scr_ele->x < collidingEle->x){
                            scr_ele->x_move *= (-1);
                        }
                        else{
                            collidingEle->x_move *= (-1);
                        }
                    }
                    
                }
                else{
                    scr_ele->x_move *= (-1);
                    collidingEle->x_move *= (-1);
                }

                if(scr_neg_move_y == colliding_neg_move_y){

                    if(scr_neg_move_y){
                        if(scr_ele->y > collidingEle->y){
                            scr_ele->y_move *= (-1);
                        }
                        else{
                            collidingEle->y_move *= (-1);
                        }

                    }
                    else{
                        if(scr_ele->y < collidingEle->y){
                            scr_ele->y_move *= (-1);
                        }
                        else{
                            collidingEle->y_move *= (-1);
                        }

                    }
                    
                }
                else{
                    scr_ele->y_move *= (-1);
                    collidingEle->y_move *= (-1);
                }
                continue;
            }

            bool fixed_x = false;
            bool fixed_y = false;

            /* Check borders at current position */
            if (scr_ele->next_x < 0){
                fixed_x = true;
                objects_collided = true;
                scr_ele->next_x = 0;
                scr_ele->x_move *= (-1);
            }
            else if ((scr_ele->next_x + scr_ele->width) > get_x_res()){
                fixed_x = true;
                objects_collided = true;
                scr_ele->next_x = get_x_res() - scr_ele->width;
                scr_ele->x_move *= (-1);
            }
            if (scr_ele->next_y < 0){
                fixed_y = true;
                objects_collided = true;
                scr_ele->next_y = 0;
                scr_ele->y_move *= (-1);
            }
            else if ((scr_ele->next_y + scr_ele->height) > get_y_res()){
                fixed_y = true;
                objects_collided = true;
                scr_ele->next_y = get_y_res() - scr_ele->height;
                scr_ele->y_move *= (-1);
            }

            if(!fixed_x)
                scr_ele->x = scr_ele->next_x;
            if(!fixed_y)
                scr_ele->y = scr_ele->next_y;

        }
        j++;

    }

    /* Draw all screensaver elements */
    for (int i = 0; i < SCREENSAVER_NUMBER_OF_ELEMENTS; i++) {
        ScreensaverEle * scr_ele = screensaver_elements[i];
        scr_ele->final_pos = false;
        draw_pixmap_direct_mode(scr_ele->sprite[scr_ele->curr_frame % scr_ele->n_sprites], scr_ele->x, scr_ele->y, scr_ele->width, scr_ele->height, 0, false);
        if(scr_ele->x_move > 0)
            scr_ele->curr_frame++;
        else
            scr_ele->curr_frame--;
    }
}


int add_element_to_screensaver(int16_t x, int16_t y, uint16_t width, uint16_t height, uint8_t ** sprite, uint8_t n_sprites) {

    static uint8_t nextID = 0;

    /* Check if maximum amount of elements has been reached */
    if (currentElements >= SCREENSAVER_NUMBER_OF_ELEMENTS)
        return 0;

    /* Allocate memory for new element */
    ScreensaverEle * new_element = malloc(sizeof(struct _screensaver_ele));
    if(new_element == NULL)
        return 1;

    /* Initialize element fields */
    new_element->id = (++nextID);
    new_element->x = x;
    new_element->y = y;
    new_element->next_x = x;    
    new_element->next_y = y;  
    new_element->width = width;
    new_element->height = height;
    new_element->final_pos = false;
    new_element->curr_frame = 0;
    new_element->n_sprites = n_sprites;

    /* Assure direction is not 0 */
    int vert_dir = rand() % 7 + 2;
    int hori_dir = rand() % 7 + 2;

    new_element->x_move = hori_dir;
    new_element->y_move = vert_dir;

    new_element->sprite = sprite;

    screensaver_elements[currentElements++] = new_element;
    return 0;
}

ScreensaverEle * check_collision_at_new_position(ScreensaverEle * ele) {

    int16_t new_x = ele->next_x;
    int16_t new_y = ele->next_y;

    /* Check all elements in the screensaver */
    for (int count = 0; count < SCREENSAVER_NUMBER_OF_ELEMENTS; count++) {
        ScreensaverEle * curr_ele = screensaver_elements[count];

        /* Assure it does not compare with itself */
        if (curr_ele->id == ele->id) continue;

        /* If sprites square boxes do not intercept, elements cant collide */
        if ( new_x + ele->width < curr_ele->x || curr_ele->x + curr_ele->width < new_x || new_y + ele->height < curr_ele->y || curr_ele->y + curr_ele->height < new_y)
            continue;

        //bool found_collision = false;

        /* For all pixels in the current element */
        for(int i = 0; i < curr_ele->height; i++){
            if (curr_ele->y + i < ele->y || curr_ele->y + i > ele->y + ele->height) continue;
            for(int j = 0; j < curr_ele->width; j++){
                if (curr_ele->x + j < ele->x || curr_ele->x + j > ele->x + ele->width) continue;

                uint32_t pixel_color;
                memcpy(&pixel_color, curr_ele->sprite[curr_ele->curr_frame % curr_ele->n_sprites] + (i * curr_ele->width + j) * bytes_per_pixel, bytes_per_pixel);
                if (pixel_color == TRANSPARENCY_COLOR_8_8_8_8)
                    continue;

                /* Pixel is not transparent, must check if it is colliding */
                if (pixel_collides(ele, curr_ele->x+j, curr_ele->y+i)){
                    //printf("ended collision_check\n");
                    return curr_ele;
                }
            }
        }
    }
    return NULL;
}

bool pixel_collides(ScreensaverEle * element, int16_t pixel_x, int16_t pixel_y) {

    int16_t new_x = element->next_x;
    int16_t new_y = element->next_y;

    /* Calculate relative positions */
    int16_t relative_x = pixel_x - new_x;
    int16_t relative_y = pixel_y - new_y;

    /* Check if out of bounds */
    if (relative_x >= element->width || relative_y >= element->height || relative_x < 0 || relative_y < 0)
        return false;
    
    /* Read pixel color */
    uint32_t pixel_color;
    memcpy(&pixel_color, element->sprite[element->curr_frame % element->n_sprites] + (relative_y * element->width + relative_x) * bytes_per_pixel, bytes_per_pixel);

    return pixel_color != TRANSPARENCY_COLOR_8_8_8_8;
}
