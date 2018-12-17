#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#define COM1_IRQ 4
#define COM1_BASE_ADDR 0x3F8

#define COM2_IRQ 3
#define COM2_BASE_ADDR 0x2F8

/* UART addresses */
#define RECEIVER_BUFFER_REG 0x0
#define TRANSMITTER_HOLDING_REG 0x0
#define INTERRUPT_ENABLE_REG 0x1
#define INTERRUPT_IDENTIFICATION_REG 0x2
#define FIFO_CTRL_REG 0x2
#define LINE_CTRL_REG 0x3
#define MODEM_CTRL_REG 0x4
#define LINE_STATUS_REG 0x5
#define MODEM_STATUS_REG 0x6
#define SCRATCHPAD_REG 0x7

#define DIVISOR_LATCH_LSB 0x0
#define DIVISOR_LATCH_MSB 0x1

/* Line Control Register */
#define LCR_LENGTH_5 0
#define LCR_LENGTH_6 BIT(0)
#define LCR_LENGTH_7 BIT(1)
#define LCR_LENGTH_8 BIT(0) | BIT(1)

#define LCR_1_STOP_BIT 0
#define LCR_2_STOP_BIT BIT(2)

#define LCR_NO_PARITY 0
#define LCR_ODD_PARITY BIT(3)
#define LCR_EVEN_PARITY BIT(4) | BIT(3)
#define LCR_1_PARITY BIT(5) | BIT(3)
#define LCR_0_PARITY BIT(5) | BIT(4) | BIT(3)

#define LCR_BREAK_ENABLE BIT(6)

#define LCR_SELECT_DL BIT(7)
#define LCR_SELECT_DATA 0

/* Line Status Register */
#define LSR_RECEIVER_READY BIT(0)
#define LSR_OVERRUN_ERR BIT(1)
#define LSR_PARITY_ERR BIT(2)
#define LSR_FRAMING_ERR BIT(3)
#define LSR_BREAK_INTERRUPT BIT(4)
#define LSR_TRANSMITTER_HOLDING_EMPTY BIT(5)
#define LSR_TRANSMITTER_EMPTY BIT(6)

/* Interrupt Enable Register */
#define IER_ENABLE_RECEIVED_DATA_INT BIT(0)
#define IER_ENABLE_TRANSMITTER_EMPTY_INT BIT(1)
#define IER_ENABLE_RECEIVER_LINE_STATUS_INT BIT(2)
#define IER_ENABLE_MODEM_STATUS_INT BIT(3)

/* Interrupt Identification Register */
#define IIR_PENDING_INT 0
#define IIR_NOT_PENDING_INT BIT(0)

#define IIR_MODEM_STATUS_INT 0
#define IIR_TRANSMITTER_EMPTY_INT BIT(1)
#define IIR_CHARACTER_TIMEOUT_INT BIT(3)
#define IIR_RECEIVED_DATA_AVAILABLE_INT BIT(2)
#define IIR_LINE_STATUS_INT BIT(2) | BIT(1)

#define IIR_64B_FIFO BIT(5)

#define IIR_NO_FIFO 0
#define IIR_ENABLED_FIFO BIT(7) | BIT(6)
#define IIR_UNUSABLE_FIFO BIT(7)

/* FIFO Control Register */
#define FCR_ENABLE_FIFO BIT(0)
#define FCR_CLEAR_RCV_FIFO BIT(1)
#define FCR_CLEAR_TRANS_FIFO BIT(2)
#define FCR_ENABLE_64_B_FIFO BIT(5)
#define FCR_INT_TRIGGER_LVL_1 0
#define FCR_INT_TRIGGER_LVL_4 BIT(6)
#define FCR_INT_TRIGGER_LVL_8 BIT(7)
#define FCR_INT_TRIGGER_LVL_14 BIT(7) | BIT(6)

int (ser_subscribe_int)(uint8_t *bit_no);
int (ser_unsubscribe_int)();


#endif
