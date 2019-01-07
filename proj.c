// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include "interrupts/mouse.h"
#include "interrupts/keyboard.h"
#include "interrupts/timer_user.h"
#include "interrupts/rtc.h"
#include "interrupts/serial_port.h"
#include "interrupts/queue.h"
#include "com_protocol.h"
#include "screensaver/screensaver.h"
#include "window/window.h"
#include "font/letters.h"

/**
 * @mainpage
 * Bem vindos ao nosso window server/system, carreguem em "Modules" para aprender mais!
 */
// Any header files included below this line should have been created by you
bool pressed_the_secret_button = false;
extern queue *send_fifo; 

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/proj/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
    return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}


int (proj_main_loop)(int argc, char *argv[]) {

    sys_enable_iop(SELF);
    /* Suprimir warnings */
    if(argc == 69 && argv != NULL)
        printf("Started\n");

    /* Initialize graphics mode */

    srand(time(NULL));

    /* Initialize video mode */
    if(vg_init(R1152x864_DIRECT) == NULL){
        printf("(%s) vg_init failed..quitting\n", __func__);
        return 1;
    }

    /* Initialize the font */
    if(init_letters() != OK) {
        printf("(%s) Error initializing the font\n", __func__);
        vg_exit();
        return 1;
    }

    /* Initialize the screensaver elements */
    if(initialize_screensaver() != OK) {
        printf("(%s) Error initializing the screensaver\n", __func__);
        vg_exit();
        return 1;
    }

    /* Configure the UART */
    if (ser_configure_settings(CP_WORD_LENGTH, CP_STOP_BIT, CP_PARITY, CP_BIT_RATE, CP_RCV_DATA_INT, CP_TRANS_EMPTY_INT, CP_LINE_STATUS_INT) != OK) {
        printf("(%s) Error configuring the UART settings\n", __func__);
        vg_exit();
        return 1;
    }

    /* Subscribe mouse Interrupts */
    uint8_t bitNum;
    if(mouse_subscribe_int(&bitNum) != OK) {
        printf("(%s) There was a problem enabling mouse interrupts\n", __func__);
        vg_exit();
        return 1; 
    } 
    uint32_t mouse_irq_set = BIT(bitNum);


    /* Enable data reporting on the mouse*/
    mouse_enable_dr();

	/* Activate the scroll wheel */
	set_scroll();

    /* Subscribe Keyboard Interrupts */
    if(keyboard_subscribe_int(&bitNum) != OK) {
        printf("(%s) There was a problem enabling timer interrupts\n", __func__);
        vg_exit();
        return 1;
    }
    uint32_t keyboard_irq_set = BIT(bitNum);

    /* Subscribe Timer 0 Interrupts */
    if(timer_subscribe_int(&bitNum) != OK) {
        printf("(%s) There was a problem enabling timer interrupts\n", __func__);
        vg_exit();
        return 1;
    }
    uint32_t timer_irq_set = BIT(bitNum);

    /* Subscribe UART Interrupts */
    if (ser_subscribe_int(&bitNum) != OK) {
        printf("(%s) There was a problem enabling uart interrupts\n", __func__);
        vg_exit();
        return 1;
    }
    uint32_t uart_irq_set = BIT(bitNum);

    /* Variables to hold results */
    int ipc_status;
    message msg;
    uint32_t r = 0;
    
    uint8_t mouse_packet[4];
	struct packet pp;

    uint8_t scancodes[SCANCODES_BYTES_LEN];

    int maxFrames = 2;
    int curFrame = 1;

    /* Initialize the internal status of the windows */
    if (init_internal_status() != OK) {
        printf("(%s) error initializing internal status\n", __func__);
        vg_exit();
        return 1;
    }

    /* Subscribe RTC Interrupts */
    uint32_t idle_time = 0; // time in interrupts
    if(rtc_subscribe_int(&bitNum) != OK){
        printf("Goddamit i could not enable interrupts for the rtc\n");
        return 1;
    }
    uint32_t rtc_irq_set = BIT(bitNum);

    /* Enable RTC Update interrupts */
    rtc_enable_update_int();
    rtc_int_handle_asm();

    while(!pressed_the_secret_button) {
        /* Get a request message.  */
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                if ( msg.m_notify.interrupts & mouse_irq_set) { /* subscribed interrupt */

                    mouse_ih();
                    if ((r = assemble_mouse_packet(mouse_packet))) {
                        parse_mouse_packet(mouse_packet, &pp);
                        window_mouse_handle(&pp);
						if(r == 4)
							window_scroll_handle((int8_t)mouse_packet[3]);
                    }
                    idle_time = 0;
                }

                if( msg.m_notify.interrupts & timer_irq_set){
                    if((++curFrame)%maxFrames == 0){
                        if (idle_time > SCREENSAVER_IDLE_TIME)
                            screensaver_draw();
                        else{
							window_draw();
						}
						swap_buffers();
                    }
                    idle_time += 1;
                }

                if (msg.m_notify.interrupts & keyboard_irq_set) {
                    keyboard_ih();
                    r = opcode_available(scancodes);
                    idle_time = 0;
                    window_kbd_handle(scancodes, r);

                }

                if( msg.m_notify.interrupts & rtc_irq_set){
                    rtc_int_handle_asm();
                }

                if (msg.m_notify.interrupts & uart_irq_set) {
                    ser_ih();
					idle_time = 0;
                }
                break;
            default:
                return 1;
                break; /* no other notifications expected: do nothing */
            }
        }
    }

	/* deal with uart messages */
    while(!is_queue_empty(send_fifo)) {
        /* Get a request message.  */
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                if (msg.m_notify.interrupts & uart_irq_set) {
                    ser_ih();
					idle_time = 0;
                }
                break;
            default:
                return 1;
                break; /* no other notifications expected: do nothing */
            }
        }
    }

    /* Free the memory allocated for the screensaver */
    free_screensaver();

    /* Free the memory allocated for the fifo queues */
    free_fifo_queues();

    /* Disable rtc update Interrupts */
    if (rtc_disable_update_int() != OK) {
        printf("(%s) error disabling rtc update interrupts\n", __func__);
        vg_exit();
        return 1;
    }

    /* Unsubscribe UART Interrupts */
    if (ser_unsubscribe_int() != OK) {
        printf("(%s) error unsubscribing uart interrupts\n", __func__);
        vg_exit();
        return 1;
    }

    /* Unsubscribe rtc Interrupts */
    if (rtc_unsubscribe_int() != OK) {
        printf("(%s) error unsubscribing rtc interrupts\n", __func__);
        vg_exit();
        return 1;
    }

    /* Unsubscribe timer Interrupts */
    if (timer_unsubscribe_int() != OK) {
        printf("(%s) error unsubscribing timer interrupts\n", __func__);
        vg_exit();
        return 1;
    }

    /* Unsubscribe KBC Interrupts */
    if(mouse_unsubscribe_int() != OK) {
        printf("(%s) error unsubscribing mouse interrupts\n", __func__);
        vg_exit();
        return 1;
    }
    
    /* Unsubscribe keyboard interrupts */
    if(keyboard_unsubscribe_int() != OK) {
        printf("(%s) error unsubscribing keyboard interrupts\n", __func__);
        vg_exit();
        return 1;
    }

    /* Returns to default Minix text mode */
    vg_exit();
    
    return 0;
}
