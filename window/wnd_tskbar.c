#include <lcom/lcf.h>
#include "window.h"
#include "../font/letters.h"
#include "interrupts/rtc.h"
#include "vbe.h"
#include "context_menu.h"

extern WindowList wnd_list;
extern bool pressed_the_secret_button;


int draw_taskbar_clock() {
    /* Read current time from RTC */
    uint8_t second, minute, hour;
    if (rtc_read_time(&second, &minute, &hour) != OK) {
    	return 1;
    }
    /* Format the string to print */
    char clock[sizeof(char) * N_CLOCK_SYMBOLS];
    sprintf(clock, "%02x:%02x:%02x", hour, minute, second);

    /* Draw the clock background */
    uint32_t bg_pos = get_x_res()-wnd_list.taskbar.clock.width;
    pj_draw_rectangle(bg_pos, 0, wnd_list.taskbar.clock.width, wnd_list.taskbar.height, wnd_list.taskbar.clock.background_color);
    /* Print the string */
    return printHorizontalWord(clock, bg_pos + wnd_list.taskbar.clock.width/2 - N_CLOCK_SYMBOLS*FONT_WIDTH/2, 0 , wnd_list.taskbar.clock.symbol_color);
}

void draw_taskbar(){

    /* Draw the bar itself */
    pj_draw_rectangle(0, 0, wnd_list.taskbar.width, wnd_list.taskbar.height, wnd_list.taskbar.color); 
    pj_draw_hline(0, wnd_list.taskbar.height, wnd_list.taskbar.width, 0x00E0E0E0);


    /* Draw the button */
    pj_draw_rectangle(0, 0, wnd_list.taskbar.menu.b_width, wnd_list.taskbar.height, 
            mouse_over_coords(0,0, wnd_list.taskbar.menu.b_width, wnd_list.taskbar.height) ||
            wnd_list.taskbar.menu.b_pressed ? 
            wnd_list.taskbar.menu.b_overlay_color : wnd_list.taskbar.menu.b_color); 

    
    printHorizontalWord(wnd_list.taskbar.menu.b_text, wnd_list.taskbar.menu.b_width/2 - strlen(wnd_list.taskbar.menu.b_text)*FONT_WIDTH/2, 0, 0);

    /* Draw the windows */
    /* TODO remove manual pad */
    uint32_t window_tskbar_pad = 20;
    uint32_t default_window_tskbar_width = (get_x_res() - (wnd_list.taskbar.menu.b_width - wnd_list.taskbar.clock.width) - window_tskbar_pad*MAX_NUM_WINDOWS)/MAX_NUM_WINDOWS;


    for(uint32_t i = 0; i< wnd_list.taskbar.num_created_windows; i++){
        if(i >= MAX_NUM_WINDOWS)
            break;

        Window *cur_wnd = wnd_list.taskbar.window_creation_list[i];
        uint32_t x = wnd_list.taskbar.menu.b_width + window_tskbar_pad/2 + i*(default_window_tskbar_width+window_tskbar_pad);

        bool mouse_over = mouse_over_coords(x, 0, x+default_window_tskbar_width, wnd_list.taskbar.height);
        if(cur_wnd == wnd_list.first)
            pj_draw_rectangle(x, 0, default_window_tskbar_width, wnd_list.taskbar.height, 0x005A5A5A);
        else
            pj_draw_rectangle(x, 0, default_window_tskbar_width, wnd_list.taskbar.height, (mouse_over ? 0x00AAAAAA : 0x008A8A8A));
        printHorizontalWord(cur_wnd->attr.frame_text, x+default_window_tskbar_width/2-strlen(cur_wnd->attr.frame_text)*FONT_WIDTH/2, 0, 0);

    }


    if(wnd_list.taskbar.menu.b_pressed){
        draw_context_menu(wnd_list.taskbar.menu.context, 0, wnd_list.taskbar.height);
    }

	draw_taskbar_clock();

}

Window *pressed_window_taskbar(){

    uint32_t window_tskbar_pad = 20;
    uint32_t default_window_tskbar_width = (get_x_res() - (wnd_list.taskbar.menu.b_width - wnd_list.taskbar.clock.width) - window_tskbar_pad*MAX_NUM_WINDOWS)/MAX_NUM_WINDOWS;
    for(uint32_t i = 0; i< wnd_list.taskbar.num_created_windows; i++){

        Window *cur_wnd = wnd_list.taskbar.window_creation_list[i];
        uint32_t x = wnd_list.taskbar.menu.b_width + window_tskbar_pad/2 + i*(default_window_tskbar_width+window_tskbar_pad);
       if(mouse_over_coords(x, 0, x+default_window_tskbar_width, wnd_list.taskbar.height))
           return cur_wnd;
    }

    return NULL;
}

bool has_taskbar_button_been_pressed(){
    wnd_list.taskbar.menu.b_pressed = mouse_over_coords(0,0, wnd_list.taskbar.menu.b_width, wnd_list.taskbar.height); 
    return wnd_list.taskbar.menu.b_pressed;
}


void leave_graphic(){
    pressed_the_secret_button = true;
}

void init_taskbar_menu(){

    wnd_list.taskbar.menu.b_text = "Start";
    wnd_list.taskbar.menu.b_width = get_x_res()/20;
    wnd_list.taskbar.menu.b_color = 0x008A8A8A;
    wnd_list.taskbar.menu.b_overlay_color = 0x005A5A5A;
    wnd_list.taskbar.menu.b_pressed = false;

    /* Context Menu */
    wnd_list.taskbar.menu.context = create_context_menu(20); /* Should be enough by now */
    if(wnd_list.taskbar.menu.context == NULL)
        return;

    ContextMenu *menu = wnd_list.taskbar.menu.context;
    add_context_menu_entry(menu, "Applications", false, NULL);
    add_context_menu_entry(menu, "Settings", false, NULL);
    add_context_menu_entry(menu, "Leave", true, (void*)leave_graphic);

    ContextMenu *settings_sub = create_context_menu(5);
    if(settings_sub == NULL)
        return;
    
    add_context_menu_entry(settings_sub, "Desktop", true, NULL);
    add_context_menu_entry(settings_sub, "System", true, NULL);
    add_context_menu_entry(settings_sub, "Startup", true, NULL);
    add_context_menu_entry(settings_sub, "Data Server", true, NULL);

    ContextEntries *settings = get_entry_by_name(menu, "Settings");
    set_sub_menu(settings, settings_sub);

}