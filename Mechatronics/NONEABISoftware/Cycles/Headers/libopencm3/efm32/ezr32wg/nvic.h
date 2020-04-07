/* This file is part of the libopencm3 project.
 *
 * It was generated by the irq2nvic_h script from ./include/libopencm3/efm32/ezr32wg/irq.json
 */

#ifndef LIBOPENCM3_EZR32WG_NVIC_H
#define LIBOPENCM3_EZR32WG_NVIC_H

#include <libopencm3/cm3/nvic.h>

/** @defgroup CM3_nvic_defines_EZR32WG User interrupts for EZR32 Wonder Gecko series
    @ingroup CM3_nvic_defines

    @{*/

#define NVIC_DMA_IRQ 0
#define NVIC_GPIO_EVEN_IRQ 1
#define NVIC_TIMER0_IRQ 2
#define NVIC_USART0_RX_IRQ 3
#define NVIC_USART0_TX_IRQ 4
#define NVIC_USB_IRQ 5
#define NVIC_ACMP01_IRQ 6
#define NVIC_ADC0_IRQ 7
#define NVIC_DAC0_IRQ 8
#define NVIC_I2C0_IRQ 9
#define NVIC_I2C1_IRQ 10
#define NVIC_GPIO_ODD_IRQ 11
#define NVIC_TIMER1_IRQ 12
#define NVIC_TIMER2_IRQ 13
#define NVIC_TIMER3_IRQ 14
#define NVIC_USART1_RX_IRQ 15
#define NVIC_USART1_TX_IRQ 16
#define NVIC_LESENSE_IRQ 17
#define NVIC_USART2_RX_IRQ 18
#define NVIC_USART2_TX_IRQ 19
#define NVIC_UART0_RX_IRQ 20
#define NVIC_UART0_TX_IRQ 21
#define NVIC_UART1_RX_IRQ 22
#define NVIC_UART1_TX_IRQ 23
#define NVIC_LEUART0_IRQ 24
#define NVIC_LEUART1_IRQ 25
#define NVIC_LETIMER0_IRQ 26
#define NVIC_PCNT0_IRQ 27
#define NVIC_PCNT1_IRQ 28
#define NVIC_PCNT2_IRQ 29
#define NVIC_RTC_IRQ 30
#define NVIC_BURTC_IRQ 31
#define NVIC_CMU_IRQ 32
#define NVIC_VCMP_IRQ 33
#define NVIC_LCD_IRQ 34
#define NVIC_MSC_IRQ 35
#define NVIC_AES_IRQ 36
#define NVIC_EBI_IRQ 37
#define NVIC_EMU_IRQ 38

#define NVIC_IRQ_COUNT 39

/**@}*/

/** @defgroup CM3_nvic_isrprototypes_EZR32WG User interrupt service routines (ISR) prototypes for EZR32 Wonder Gecko series
    @ingroup CM3_nvic_isrprototypes

    @{*/

BEGIN_DECLS

void dma_isr(void);
void gpio_even_isr(void);
void timer0_isr(void);
void usart0_rx_isr(void);
void usart0_tx_isr(void);
void usb_isr(void);
void acmp01_isr(void);
void adc0_isr(void);
void dac0_isr(void);
void i2c0_isr(void);
void i2c1_isr(void);
void gpio_odd_isr(void);
void timer1_isr(void);
void timer2_isr(void);
void timer3_isr(void);
void usart1_rx_isr(void);
void usart1_tx_isr(void);
void lesense_isr(void);
void usart2_rx_isr(void);
void usart2_tx_isr(void);
void uart0_rx_isr(void);
void uart0_tx_isr(void);
void uart1_rx_isr(void);
void uart1_tx_isr(void);
void leuart0_isr(void);
void leuart1_isr(void);
void letimer0_isr(void);
void pcnt0_isr(void);
void pcnt1_isr(void);
void pcnt2_isr(void);
void rtc_isr(void);
void burtc_isr(void);
void cmu_isr(void);
void vcmp_isr(void);
void lcd_isr(void);
void msc_isr(void);
void aes_isr(void);
void ebi_isr(void);
void emu_isr(void);

END_DECLS

/**@}*/

#endif /* LIBOPENCM3_EZR32WG_NVIC_H */
