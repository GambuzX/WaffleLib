#include <lcom/lcf.h>
#include "../window/window.h"

extern WindowList wnd_list;
bool painter_input_handler(Element *el, unsigned type, void *data, Window *wnd);

void create_painter(){

    uint32_t wnd_width = 500, wnd_height = 700;
    uint32_t wnd_id = create_window(wnd_width, wnd_height, 0x008A8A8A, "Paint", &painter_input_handler);

    void *image = malloc(500*500*4);
    memset(image, 0xFF, 500*500*4);
    window_add_element(wnd_id, CANVAS, 0, 0, wnd_width, wnd_width, (void*)&image, NULL);

    window_add_element(wnd_id, SLIDER, 50, wnd_width+50, 255, 30, NULL, "red"); 
    window_add_element(wnd_id, SLIDER, 50, wnd_width+50+30+5, 255, 30, NULL, "green"); 
    window_add_element(wnd_id, SLIDER, 50, wnd_width+50+30*2+10, 255, 30, NULL, "blue"); 


    void *color_display = malloc((30*2+10)*(30*2+10)*4);
    memset(color_display, 0, (30*2+10)*(30*2+10)*4);
    window_add_element(wnd_id, IMAGE, 50+255+20, wnd_width+50+(10*2+10)/2, 30*2+10, 30*2+10, &color_display, "color"); 
}

void do_horizontal_brush(Element *canvas, uint32_t *pixels, uint32_t brush_size, uint32_t pos, uint32_t color){

    /* Only paints pixels on the same line */
    uint32_t line = pos/canvas->width;

    uint32_t first_of_line = line*canvas->width;
    uint32_t last_of_line = (line+1)*canvas->width;
    
    /* TODO What if pos = 0? */
    if(first_of_line < (pos - brush_size))
        first_of_line = pos - brush_size;

    if(last_of_line > (pos + brush_size))
        last_of_line = (pos + brush_size);

    for(uint32_t i = first_of_line; i < last_of_line; i++)
        pixels[i] = color;
}

void paint_with_brush(Element *canvas, uint32_t brush_size, uint32_t x, uint32_t y, uint32_t color){

    uint32_t pos = x + (y*canvas->width);

    if(pos > (canvas->width * canvas->height))
        return;

    uint32_t *pixels = canvas->attr.canvas.space;

    if(brush_size == 1)
        pixels[pos] = color;
    else{
        do_horizontal_brush(canvas, pixels, brush_size, pos, color);
        brush_size--;

        for(int i = 1; brush_size > 1; brush_size--, i++){
            do_horizontal_brush(canvas, pixels, brush_size, pos+i*canvas->width, color);
            do_horizontal_brush(canvas, pixels, brush_size, pos-i*canvas->width, color);
        }
    }
}

bool painter_input_handler(Element *el, unsigned type, void *data, Window *wnd){
    printf("", el, type, data, wnd);

    if(type == CANVAS_MSG){
        uint32_t color = *(uint32_t*)find_by_id(wnd, "color")->attr.image.space;

        uint32_t x =  wnd_list.cursor.x - wnd->x - el->x, y = wnd_list.cursor.y - wnd->y - el->y;
        paint_with_brush(el, 5, x, y, color); 
        struct packet *pp = data;

        if(pp->delta_x == 0 && pp->delta_y == 0)
            return false;


        /* None of them are 0 */
        uint32_t steps_x = abs(pp->delta_x);
        uint32_t steps_y = abs(pp->delta_y);
        bool upwards = (pp->delta_y > 0); 
        bool right = (pp->delta_x > 0); 

        for(; steps_x || steps_y; ){
                if(steps_x){
                    if(right) x++;
                    else x--;
                    steps_x--;
                }
                if(steps_y){
                    if(upwards) y--;
                    else y++;
                    steps_y--;
                }
            
                paint_with_brush(el, 5, x, y, color); 
        }
    }
    else if(type == SLIDER_MSG){
        uint32_t *pixels = (uint32_t*)find_by_id(wnd, "color")->attr.image.space;
        uint32_t color = *pixels;
        if(!strcmp(el->identifier, "red")){
            uint8_t red = (uint8_t)el->attr.slider.pos;
            color &= 0xFF00FFFF;
            color |= red<<16;
        }
        else if(!strcmp(el->identifier, "blue")){
            uint8_t blue = (uint8_t)el->attr.slider.pos;
            color &= 0xFFFFFF00;
            color |= blue;
        }
        else{
            uint8_t green = (uint8_t)el->attr.slider.pos;
            color &= 0xFFFF00FF;
            color |= green<<8;
        }
        
        for(uint32_t i = 0; i<(30*2+10)*(30*2+10); i++)
            pixels[i] = color;
    }

    return false;
}