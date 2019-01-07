#ifndef WINDOW_H
#define WINDOW_H

#define SLIDER_WIDTH 5

/**
 * @defgroup element elements module
 * @ingroup window 
 * @{
 */

/**
 * @brief Enumeration containing all the types of elements
 */
typedef enum {
    BUTTON,
    TEXT_BOX,

    RADIO_BUTTON,

    LIST_VIEW,
    CHECKBOX,
    TEXT,
    IMAGE,

    SLIDER,
    CANVAS,

    DATA,
    INVALID

}ElementType;

/**
 * @brief The structure representing an element of a window
 */
typedef struct _element{
    
    uint32_t id; /**< Internal id of the window */
    char *identifier; /**< ID used by the user */
    ElementType type; /**< Type of the element */
    uint16_t x; /**< x position of element */
    uint16_t y; /**< y position of element */

    uint16_t width; /**< width of the element */
	uint16_t height; /**< height of the element */

    union{
        struct _button_attr{ 
            char *text; /**< text of button */
            uint32_t color; /**< color of button */
			uint32_t overlay_color; /**< color when the button is overlayed */
        }button;

        struct _text_box_attr{
            char *text; /**< text of text box */
            uint32_t text_size; /**< Internal use */
            uint32_t background_color; /**< Color of textbox */
			uint32_t text_color; /**< Color of the text */
            bool selected; /**< Wheter the text box is selected */
        }text_box;

        struct _list_view_attr_real{
            char **entries; /**< Array containing all the entries */
            uint32_t num_entries; /**< Number of entries */

            uint32_t drawable_entries; /**< Number of drawable entries */
            bool scrollbar_active; /**< Whether the scrollbar is active */
            bool scrollbar_selected; /**< Whether the scrollbar is selected */
            uint32_t scrollbar_y; /**< Y position of the scrollbar */
            uint32_t scrollbar_height; /**<	Scrolbar height */
            uint32_t max_chars; /**< Maximum number of chars */

        }list_view;

        struct _checkbox_attr{
            char *text; /**< Text of checkbox */
            bool enabled; /**< Whether the checkbox is enabled */
        }checkbox;
	
        struct _text_attr{
            char *text; /**< Text */
            uint32_t color; /**< Color of text */
            bool active; /**< Whether the text is visible */
            bool vertical; /**< Whether the text is visible */
        }text;

        struct _image_attr{
            void *space; /**< Space of the image */
        }image;

        struct _canvas_attr{
            void *space; /**< Space of the canvas */
        }canvas;

        struct _slider_atr{
            uint32_t pos; /**< Position of the slider */
            bool selected; /**< Whether the slider is selected */
        }slider;

        struct _data_attr{
            void *space; /**< Space of data */
        }data;

    }attr;

    struct _element *next; /**< Next element */

}Element;
/** @} */

/**
 * @defgroup window window module
 * All windows are here
 * @{
 */

/**
 * @brief The window structure
 */
typedef struct _window{
    uint32_t id; /**< id of the window */
    int16_t x; /**< x position of the window */
	int16_t y; /**< y position of the window */

    uint16_t width; /**< width of the window */
	uint16_t height; /**< height of the height */
    uint16_t orig_width; /**< Original width of window, used for maximize */
	uint16_t orig_height; /**< Original height of window, used for maximize */
    uint32_t color; /**< Color of the window */
    bool (*handler)(Element *el, unsigned type, void *data, struct _window *wnd); /**< Window handler */
    bool minimized; /**< whether the window is minimized */
    bool maximized; /**< whether the window is maximized */
    uint32_t last_el_id; /**< id of the last element */
    Element *elements; /**< list of elements */

    struct{
        bool border; /**< whether the window has border */
        uint32_t border_width; /**< The width of the border */
        bool frame; /**< whether the window has a frame */
        char *frame_text; /**< text of the frame */
    }attr;/* To be filled */
    struct _window *prev; /**< previous window */
	struct _window *next; /**< next window */
}Window;
/** @} */


/**
 * @addtogroup context_menu
 * @{
 */

/**
 * @brief The structure representing the context menu
 */
struct _context_menu;

/**
 * @brief Represents an entry in a context menu
 */
typedef struct{
    char *text; /**< text of the entry */
    void (*callback)(); /**< callback if there's any */
    struct _context_menu *menu; /**< sub-menu */
}ContextEntries;

/**
 * @brief The structure representing the context menu
 */
typedef struct _context_menu{
    uint32_t longer_entry; /**< length of the longest entry */
    ContextEntries **entries; /**< list of the entries */
    uint32_t size; /**< size of the menu */
	uint32_t capacity; /**< capacity of the menu */
    uint32_t sub_menu_index; /**< index of the active sub_menu */
    struct _context_menu *active_sub; /**< address of the active sub-menu */
}ContextMenu;
/** @} */

/** 
 * @addtogroup window
 * @{
 */
/**
 * @brief Manages all windows
 */
typedef struct _wnd_lst{
    Window *first; /**< first window of the list */
    Window *last; /**< last window of the list */

    /* Cursor status */
    struct{
        int16_t x; /**< x position of cursor */
		int16_t y; /**< y position of cursor */
        uint16_t width; /**< width of the cursor */
		uint16_t height; /**< height of the cursor */
		void *image; /**< address of the image */
    }cursor;

    struct{
        uint16_t width; /**< width of the taskbar */
		uint16_t height; /**< height of the taskbar */
        uint32_t color; /**< color of the taskbar */
        
        struct _menu{
            /* b_* prefix means it's the button */
            uint16_t b_width; /**< button width */
            char *b_text; /**< text of the button */
            uint32_t b_color; /**< color of the button */
			uint32_t b_overlay_color; /**< overlay color of the button */
            bool b_pressed; /**< whether the button is pressed */

            ContextMenu *context; /**< menu de contexto */
        }menu;

        struct _clock{
            uint16_t width; /**< width of the clock */
            uint32_t symbol_color; /**< color of the clock */
			uint32_t background_color; /**< background color of the clock */
        }clock;

        Window **window_creation_list; /**< order of created windows */
        uint32_t num_created_windows; /**< number of created windows */
		uint32_t size_windows; /**< size of the windows */

    }taskbar;

    uint8_t * background_sprite; /**< sprite of the background */
    uint16_t bckg_width; /**< background width */
	uint16_t bckg_height; /**< background height */

    bool bckg_image; /**< background image */
    uint32_t bckg_color; /**< background color */

}WindowList;
/** @} */

#define BACKGROUND_COLOR 0x00008081



/**
 * @addtogroup window 
 * @{
 */

/**
 * @brief draws all windows
 */
void window_draw();

/**
 * @brief Handle mouse inputs
 */
void window_mouse_handle(const struct packet *pp);

/**
 * @brief Handles kbd input
 * @param scancode the scancodes
 * @param num number of items
 *
 */
void window_kbd_handle(const uint8_t *scancode, uint32_t num);

/**
 * @brief Creates a window
 * @param width width of the window
 * @param height height of the window
 * @param color of the window
 * @param name the name of the window
 * @param input_handler the input handler of the window
 * 
 * @return the window id, 0 if failed
 */
uint32_t create_window(uint16_t width, uint16_t height, uint32_t color, const char *name, bool (*input_handler)(Element *el, unsigned, void*, Window*));

/**
 * @brief inits the internal status of the windows
 *
 * @return 0 on success
 */
int init_internal_status();

/**
 * @brief Adds element to a window
 * @param id The id of the window to add the element to
 * @param type The type of the element
 * @param x position of the element
 * @param y position of the element
 * @param width width of the element
 * @param height element of the element
 * @param attr data of the attribute
 * @param identifier identifier of the attribute
 * @return id of the element, 0 if failed
 */
uint32_t window_add_element(uint32_t id, ElementType type, uint16_t x, uint16_t y, uint16_t width, uint16_t height, void * attr, char *identifier);

/**
 * @brief checks if window is focused
 * @param wnd the window
 * @return true if focused
 */
bool is_window_focused(const Window *wnd);

/**
 * @brief Checks if mouse is over coordinates
 * @param x initial x position
 * @param y initial y position
 * @param xf final x position
 * @param yf final y position
 * @return true if over
 *
 */
bool mouse_over_coords(uint16_t x, uint16_t y, uint16_t xf, uint16_t yf);

/**
 * @brief Builds an element internally
 * @param type of the element
 * @param x x position
 * @param y y position
 * @param width width of the element
 * @param height height of the element
 * @param attr data of the element
 * @param identifier identifier of the element
 * @return pointer to the element
 */

/**
 * @} */

/**
 * @addtogroup element
 * @{
 */

/**
 * @brief Draws elements of a window
 * @param wnd the window containing the elements
 */
void draw_elements(const Window *wnd);
/** @} */

/**
 * @addtogroup window
 * @{
 */

/**
 * @brief updates state of the internal machine
 * @param pp mouse input
 * @return status change
 */
uint32_t update_state(const struct packet *pp);

/** @} */

/**
 * @defgroup taskbar taskbar module
 * @ingroup window
 * @{
 */
/**
 * @brief Draws the taskbar
 */
void draw_taskbar();

/**
 * @brief Checks if the taskbar button has been pressed
 * @return true if pressed
 */
bool has_taskbar_button_been_pressed();

/**
 * @brief Inits the taskbar menu
 */
void init_taskbar_menu();

/**
 * @brief Draws the taskbar
 * @return 0 if success
 */
int draw_taskbar_clock();

/**
 * @brief Checks if a window was pressed on the taskbar
 * @return the pressed window */
Window *pressed_window_taskbar();

/** @} */

/**
 * @addtogroup window
 * @{
 */

/**
 * @brief Modifies text of textbox
 * @param element textbox element
 * @param scancode the keyboard scancode
 * @param num number of elements
 * @param wnd the window
 */
void modify_text_box(Element *element, const uint8_t *scancode, uint32_t num, Window *wnd);

/**
 * @brief frees a window
 */
void free_window();

/**
 * @brief Set a menu as a sub menu
 * @param entry the  submenu entries
 * @param menu the menu to be set as a submenu
 */
void set_sub_menu(ContextEntries *entry, ContextMenu *menu);

/**
 * @brief Gets entry by name
 * @param menu the menu
 * @param name the name to be searched
 * @return the entry if found, NULL otherwise
 */
ContextEntries *get_entry_by_name(ContextMenu *menu, const char *name);

/**
 * @brief Calls callback of menu entry
 * @param menu menu that was pressed
 * @param x position pressed
 * @param y position pressed
 * @return false if anything should be done
 */
bool call_entry_callback(ContextMenu *menu, uint32_t x, uint32_t y);

/**
 * @brief Finds element by id
 * @param wnd Window containing the element
 * @param identifier identifier of the element
 * @return element if found, NULL otherwise
 */
Element *find_by_id(Window *wnd, char *identifier);

/**
 * @brief Handles interaction of mouse with element
 * @param wnd window containing the elements
 * @param pressed whether the button was pressed or held
 * @param pp the mouse packet
 */
void mouse_element_interaction(Window *wnd, bool pressed, const struct packet *pp);

/**
 * @brief gets window pointer by id
 * @param id id of the window
 * @return pointer to window, NULL if failed
 */
Window *window_get_by_id(uint32_t id);

/**
 * @brief deactivates all sub menus
 * @param menu the menu containing submenu
 */
void deactivate_subs(ContextMenu *menu);

/**
 * @brief Deletes window
 * @param wnd the window
 */
void delete_window(Window *wnd);

/**
 * @brief Sends scroll input to the window
 * @param scroll the ammount of scroll
 */
void window_scroll_handle(int8_t scroll);


/**
 * @brief Moves a window to front
 * @param wnd the window
 */
void move_to_front(Window *wnd);
/** @} */


/**
 * @addtogroup element
 * @{
 */
/**
 * @brief Changes list view elements
 * @param element the list view
 * @param entries the entries
 * @param num the number of entries
 */
void set_list_view_elements(Element *element, char **entries, unsigned num);

/**
 * @brief Set text of element
 * @param el text element
 * @param new_text the new text
 */
void set_text(Element *el, char *new_text);
/** @} */

static const char *cursor[] = {
/* columns rows colors chars-per-pixel */
"12 20 3 1",
". c None",
"B c #000000",
"W c #FFFFFF",
/* pixels */
"B...........",
"BB..........",
"BWB.........",
"BWWB........",
"BWWWB.......",
"BWWWWB......",
"BWWWWWB.....",
"BWWWWWWB....",
"BWWWWWWWB...",
"BWWWWWWWWB..",
"BWWWWWWWWWB.",
"BWWWWWWBBBBB",
"BWWWBWWB....",
"BWWWBWWB....",
"BWWB.BWWB...",
"BWB..BWWB...",
"BB....BWWB..",
"B.....BWWB..",
"B......BWWB.",
"........BBB.",
};

static const struct _button_attr DEFAULT_BUTTON_ATTR = { "TEST", 0x007A7A7A, 0x00BABABA};
static const struct _text_box_attr DEFAULT_TEXT_BOX_ATTR = { NULL, 50, 0, 0xFFFFFFFF, true };

#define L_BUTTON 4 
#define M_BUTTON 2 
#define R_BUTTON 1 

typedef enum{
    L_DEAD = 1,
    L_PRESSED = 2,
    L_KEPT = 4,

    M_DEAD = 8,
    M_PRESSED = 16,
    M_KEPT = 32,

    R_DEAD = 64,
    R_PRESSED = 128,
    R_KEPT = 256

}Event;

#define N_CLOCK_SYMBOLS 8 /* Number of symbols in clock, hh:mm:ss */
#define MAX_NUM_WINDOWS 6


/**
 * @addtogroup element
 * @{
 */
/**
 * @brief Builds an element internally
 * @param type of the element
 * @param x x position
 * @param y y position
 * @param width width of the element
 * @param height height of the element
 * @param attr data of the element
 * @param identifier identifier of the element
 * @return pointer to the element
 */
Element *build_element(ElementType type, uint16_t x, uint16_t y, uint16_t width, uint16_t height, void *attr, char *identifier);

/**
 * @brief recalculates the list view elements
 * @param element the list view
 */
void recalculate_list_view(Element *element);

/**
 * @brief Structure used by the user to pass list view information
 */
struct _list_view_attr{
    char **entries;
    uint32_t num_entries;
};

/**
 * @brief Contains a keyboard message
 */
typedef struct _kbd_msg{
    uint32_t num;
    uint8_t scancode[3];
}kbd_msg;

/**
 * @brief Contains a list view message
 */
typedef struct _list_view_msg{
    uint32_t index;
}list_view_msg;

/**
 * @brief All types of messages
 */
enum MESSAGE_TYPE{
    KEYBOARD,
    MOUSE,
    LIST_VIEW_MSG,
    BUTTON_MSG,
    SLIDER_MSG,
    CHECKBOX_MSG,
    MAXIMIZE_MSG,
    CANVAS_MSG,
	FREE_MSG,
	CLOSE_MSG
};
/** @} */

static const char *folder[] = {
	/* columns rows colors chars-per-pixel */
	"75 75 4 1 ",
	"  c #880015",
	". c #FF7F27",
	"X c #22B14C",
	"o c None",
	/* pixels */
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooo                                                       ooooooooo",
	"ooooooooo   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX ooooooooo",
	"oooooooo  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  oooooooo",
	"oooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooo",
	"oooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooo",
	"oooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooo",
	"oooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooo",
	"oooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooo",
	"oooooooo  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooo",
	"ooooooo    XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooo",
	"ooooooo ..  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  oooooooo",
	"ooooooo ....  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX .. ooooooo",
	"ooooooo ......  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX    .. ooooooo",
	"ooooooo ........                                              ..... ooooooo",
	"ooooooo ........................................................... ooooooo",
	"ooooooo ........................................................... ooooooo",
	"ooooooo ........................................................... ooooooo",
	"ooooooo ........................................................... ooooooo",
	"ooooooo ........................................................... ooooooo",
	"ooooooo ........................................................... ooooooo",
	"ooooooo ........................................................... ooooooo",
	"ooooooo ........................................................... ooooooo",
	"ooooooo  .......................................................... ooooooo",
	"oooooooo .......................................................... ooooooo",
	"oooooooo .......................................................... ooooooo",
	"oooooooo  ......................................................... ooooooo",
	"ooooooooo ........................................................ oooooooo",
	"ooooooooo  ....................................................... oooooooo",
	"oooooooooo ......................................................  oooooooo",
	"oooooooooo ...................................................... ooooooooo",
	"ooooooooooo ..................................................... ooooooooo",
	"ooooooooooo ..................................................... ooooooooo",
	"ooooooooooo  ...................................................  ooooooooo",
	"oooooooooooo  .................................................. oooooooooo",
	"ooooooooooooo .................................................  oooooooooo",
	"ooooooooooooo  ...............................................  ooooooooooo",
	"oooooooooooooo  .............................................  oooooooooooo",
	"ooooooooooooooo ............................................  ooooooooooooo",
	"ooooooooooooooo  .........................................  ooooooooooooooo",
	"oooooooooooooooo   ......................................  oooooooooooooooo",
	"oooooooooooooooooo    .................................  oooooooooooooooooo",
	"ooooooooooooooooooooo                                   ooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
	"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
};
static const char *file[] = {
/* columns rows colors chars-per-pixel */
"75 75 4 1 ",
"  c #000000",
". c #00A2E8",
"X c #99D9EA",
"o c None",
/* pixels */
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooo      oooooooooooooooo       ooooooooooooooooooooooo",
"ooooooooooooooooooooooo XXXX                 ......    oooooooooooooooooooo",
"ooooooooooooooooooooooo XXXXXXXXXXXXXXXXXXXX  ........  ooooooooooooooooooo",
"oooooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXX  ........  oooooooooooooooooo",
"oooooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXX  ........  ooooooooooooooooo",
"oooooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXX  ........ ooooooooooooooooo",
"oooooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXX  .......  oooooooooooooooo",
"ooooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXX  ....... oooooooooooooooo",
"ooooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXX  ...... oooooooooooooooo",
"oooooooooooooooooooo  XXXXXXXXXXXXXXXXXXXXXXXXXXXXX ...... oooooooooooooooo",
"oooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX ..... oooooooooooooooo",
"oooooooooooooooooooo XXXXX XXXXXXXXXX XXXXXXXXXXXXXXX  ... oooooooooooooooo",
"oooooooooooooooooooo XXXXX X   XXX X    XXXXXXXXXXXXXX   . oooooooooooooooo",
"oooooooooooooooooooo XXXXX X X  XX X XXX XXXXXXXXXXXXXXX   oooooooooooooooo",
"oooooooooooooooooooo XXXXX X XX XX X XX  XXXXXXXXXXXXXXXXX oooooooooooooooo",
"oooooooooooooooooooo XXXXX X    XX X    XXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXX XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXX XXXXXXXXXXX  XXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXX X    X  XX    XXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXX X XX XX X  XX XXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXX X XX XX X XXX XXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXX X X  XX X  XX XXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXX X   XXX XX    XXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXX XXX   XX  XXXX XXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXX  X  X XXX XX X   XXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXX X XX XXX X  XXX XXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXX X  X XXX X XXXX XXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXX   XXX X  XXX XXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXXXXXX     XXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXX XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXX X   XXX XX    XXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXX X X  XX  X XXX XXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXX X XX XXX X XXX XXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXX X X  XXX X XXX XXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXX X   XXXX      XXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX oooooooooooooooo",
"ooooooooooooooooooo      XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  oooooooooooooooo",
"oooooooooooooooooooooooo                                  ooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
};

/**
 * @brief Desktop entry
 */
struct desktop_ee{
    bool folder;
    char *name;
};

#endif
