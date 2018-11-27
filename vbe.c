#include <lcom/lcf.h>
#include <vbe.h>
#include <math.h>
#include <stdlib.h>
#include "util.h"

static vbe_mode_info_t vbe_mode_info;
static uint8_t* mapped_mem;

void *retry_lm_alloc(size_t size, mmap_t *mmap){
    void *result = NULL;
    for(unsigned i = 0; i < 5 ; i++){
        result = lm_alloc(size, mmap);
        if(result != NULL)
            break;
        sleep(1);
    }

    return result;
}

int vbe_get_mode_info_2(uint16_t mode, vbe_mode_info_t * vmi_p) {
    struct reg86u r;
    mmap_t mmap;

    /* Reset the struct values */
    memset(&r, 0, sizeof(r));

    /* Allocate memory block in low memory area */
    if (retry_lm_alloc(sizeof(vbe_mode_info_t), &mmap) == NULL) {
    	printf("(%s): lm_alloc() failed\n", __func__);
    	return VBE_LM_ALLOC_FAILED;
    }

    /* Build the struct */
    r.u.b.ah = VBE_FUNC; 
    r.u.b.al = RETURN_VBE_MODE_INFO;
    r.u.w.cx = mode;
    r.u.w.es = PB2BASE(mmap.phys);
    r.u.w.di = PB2OFF(mmap.phys);
    r.u.b.intno = VIDEO_CARD_SRV;

    /* BIOS Call */
    if( sys_int86(&r) != FUNC_SUCCESS ) {
        lm_free(&mmap);
        printf("(%s): sys_int86() failed \n", __func__);
        return VBE_SYS_INT86_FAILED;
    }

    /* Verify the return for errors */
    if (r.u.w.ax != FUNC_RETURN_OK) {
        lm_free(&mmap);
        printf("(%s): sys_int86() return in ax was different from OK \n", __func__);
        return VBE_INVALID_RETURN;    	
    }

    /* Copy the requested info to vbe_mode_info */
    memcpy(vmi_p, mmap.virt, sizeof(vbe_mode_info_t));

    /* Free allocated memory */
    lm_free(&mmap);

    return VBE_OK;
}

int set_video_mode(uint16_t mode){

    struct reg86u r;

    /* Reset the struct values */
    memset(&r, 0, sizeof(r));

    /* Build the struct */
    r.u.b.ah = VBE_FUNC; 
    r.u.b.al = SET_VBE_MODE; 
    r.u.w.bx = LINEAR_FRAME_BUFFER | mode;
    r.u.b.intno = VIDEO_CARD_SRV;

    /* BIOS Call */
    if( sys_int86(&r) != FUNC_SUCCESS ) {
        printf("(%s): sys_int86() failed \n", __func__);
        return VBE_SYS_INT86_FAILED;
    } 
    return VBE_OK;
}

void* (vg_init)(uint16_t mode){

    /* Initialize lower memory region */
    if(lm_init(true) == NULL){
        printf("(%s) Couldnt init lm\n", __func__);
        return NULL;
    }

    int res = 0;
    if((res = vbe_get_mode_info_2(mode, &vbe_mode_info)) != OK ){
        printf("(%s) Couldnt get mode info\n", __func__);
        return NULL;
    }

    struct minix_mem_range mr; /* physical memory range */
    unsigned int vram_base = vbe_mode_info.PhysBasePtr; /* VRAM’s physical addresss */

    unsigned int vram_size = vbe_mode_info.XResolution * vbe_mode_info.YResolution * vbe_mode_info.BitsPerPixel ; /* VRAM’s size, but you can use
    the frame-buffer size, instead */

    void *video_mem; /* frame-buffer VM address */

    /* Allow memory mapping */

    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if( OK != (res = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", res);
    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED)
        panic("couldn’t map video memory");

    /* Store the mapped memmory pointer in mapped_mem */
    mapped_mem = video_mem;

    /* Set video mode */
    if(set_video_mode(mode) != OK)
        return NULL;

    return video_mem;    
}


int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

    /* Check if out of bounds */
    if (x >= get_x_res() || y >= get_y_res()) {			
        printf("(%s) Invalid coordinates: x=%d, y=%d", __func__, x, y);
        return VBE_INVALID_COORDS;		
    }

    /* Direct color */
    if (get_memory_model() == DIRECT_COLOR_MODE) {

        /* Calculate size in bytes each pixel occupies */
        uint8_t pixel_size = calculate_size_in_bytes(get_bits_per_pixel());

        for (uint32_t i = 0; i < len; i++){
            /* Check if x is outside of range */
            if (x+i >= get_x_res())
            return VBE_OK;

            /* Color the pixel */
            uint32_t y_coord = y * get_x_res() * pixel_size;
            uint32_t x_coord = (x + i) * pixel_size;  
            memcpy(mapped_mem + y_coord + x_coord, &color, pixel_size);
        }
    }
    /* Indexed color */
    else if (get_memory_model() == INDEXED_COLOR_MODE) {
        memset(mapped_mem + y * get_x_res() + x, (uint8_t)color, (len > (get_x_res() - x) ? get_x_res() - x : len));
    }
    else {
        printf("(%s) Unsuported color mode, __func__\n");
        return VBE_INVALID_COLOR_MODE;
    }

    return VBE_OK;
}


int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

    /* Check if out of bounds */
    if (x >= get_x_res() || y >= get_y_res()){
        printf("(%s) Invalid coordinates: x=%d, y=%d\n", __func__, x, y);
        return VBE_INVALID_COORDS; 
    }

    /* Write a line for the whole height */
    for (uint32_t i = 0; i < height; i++) {
        // Check if out of screen range
        if (y+i >= get_y_res()) break;
            if (vg_draw_hline(x, y + i, width, color) != OK) {
                printf("(%s) There was a problem drawing a h line\n", __func__);
                return VBE_DRAW_LINE_FAILED;
        }
    }
    return VBE_OK;
}

void draw_pixmap_on(const char *pixmap, uint16_t x, uint16_t y, int width, int height, uint8_t *buffer){

    /* Iterate lines */
    for(int i = 0; i < height; i++){
        /* Y is out of bounds */
        if((i+y) >= get_y_res())
            break;

        /* Iterate columns */
        for(int j = 0; j<width; j++){         
            /* X is out of bounds */
            if((j+x) >= get_x_res())
                break;
            /* Draw the pixmap pixel */
            buffer[(y+i)*get_x_res() + x + j] = pixmap[i*width + j];
        }
    }
}

void (draw_pixmap)(const char *pixmap, uint16_t x, uint16_t y, int width, int height){
    draw_pixmap_on(pixmap, x, y, width, height, mapped_mem);
}

void (clear_buffer)(uint8_t *buffer, uint8_t color){
    memset(buffer, color, get_x_res() * get_y_res());
}


void swap_buffers(uint8_t *buffer){
    memcpy(mapped_mem, buffer, get_x_res() * get_y_res());
}

uint32_t get_pattern_color(uint32_t first, uint8_t row, uint8_t col, uint8_t step, uint8_t no_rectangles){

    uint32_t color = 0;
    /* Direct color mode */
    if (get_memory_model() == DIRECT_COLOR_MODE) {

    	/* Separate the original color components */
        uint32_t orig_red = (first >> get_red_field_position()) & set_bits_mask(get_red_mask_size());
        uint32_t orig_green = (first >> get_green_field_position()) & set_bits_mask(get_green_mask_size());
        uint32_t orig_blue = (first >> get_blue_field_position()) & set_bits_mask(get_blue_mask_size());

        /* Calculate new color components */
        uint32_t red = (orig_red + col * step) % (1 << get_red_mask_size());
        uint32_t green = (orig_green + row * step) % (1 << get_green_mask_size());
        uint32_t blue = (orig_blue + (col + row) * step) % (1 << get_blue_mask_size());

        /* Build the new color */
        color = (red << get_red_field_position()) | (green << get_green_field_position()) | (blue << get_blue_field_position());

    }
    /* Indexed color mode */
    else if (get_memory_model() == INDEXED_COLOR_MODE) {
        /* Calculate index color */
        color = (first + (row * no_rectangles + col) * step) % (1 << get_bits_per_pixel());
    }

    return color;

}

int draw_pattern(uint16_t width, uint16_t height, uint8_t no_rectangles, uint32_t first, uint8_t step){

	/* Iterate lines */
	for (uint8_t row = 0; row < no_rectangles; row++) {
		/* Iterate columns */
		for (uint8_t col = 0; col < no_rectangles; col++) {
			/* Draw rectangle */
			if (vg_draw_rectangle(col * width, row * height, width, height, get_pattern_color(first, row, col, step, no_rectangles)) != OK) {
				printf("(%s) There was a problem drawing the rectangle\n", __func__);
				return VBE_NOT_OK;
			}
		}
	}

    return OK;
}

uint8_t get_bits_per_pixel() { return vbe_mode_info.BitsPerPixel; }
uint16_t get_x_res() { return vbe_mode_info.XResolution; }
uint16_t get_y_res() { return vbe_mode_info.YResolution; }
uint8_t get_memory_model() { return vbe_mode_info.MemoryModel; }
uint8_t get_red_mask_size() { return vbe_mode_info.RedMaskSize; }
uint8_t get_red_field_position() { return vbe_mode_info.RedFieldPosition; }
uint8_t get_blue_mask_size() { return vbe_mode_info.BlueMaskSize; }
uint8_t get_blue_field_position() { return vbe_mode_info.BlueFieldPosition; }
uint8_t get_green_mask_size() { return vbe_mode_info.GreenMaskSize; }
uint8_t get_green_field_position() { return vbe_mode_info.GreenFieldPosition; }
uint8_t get_rsvd_mask_size() { return vbe_mode_info.RsvdMaskSize; }
uint8_t get_rsvd_field_position() { return vbe_mode_info.RsvdFieldPosition; }
