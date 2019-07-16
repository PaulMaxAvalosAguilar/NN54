#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include "FreeRTOS.h"
#include "printf.h"
#include "task.h"
#include "lcd.h"

static volatile uint32_t timesCalled = 0;
static volatile uint32_t error = 0;
static volatile uint32_t lastTimesCalled = 0;
static volatile uint32_t lastTimeValue = 0;
static volatile uint32_t currTimeValue = 0;

static volatile uint32_t overflowCounter=0 ;

void configurePeriphereals(void);

void configurePeriphereals(void){
  rcc_periph_clock_enable(RCC_AFIO);    // I2C
  rcc_periph_clock_enable(RCC_GPIOA);   // TIM1 TIM2
  rcc_periph_clock_enable(RCC_GPIOB);	// I2C TIM4 TIM2
  rcc_periph_clock_enable(RCC_I2C1);	// I2C
  rcc_periph_clock_enable(RCC_TIM1);    // TIM1
  rcc_periph_clock_enable(RCC_TIM2);    // TIM2
  rcc_periph_clock_enable(RCC_TIM4);    // TIM4

  //32bit Timer
  
  //TIM1
  TIM1_CCMR1 |= (TIM_CCMR1_CC1S_IN_TI1);//TI1 for CaptureChannel 1
  TIM1_CCER &= ~(TIM_CCER_CC1P);//Rising Edge
  TIM1_CCER |= (TIM_CCER_CC1E);//Enable capture on Channel1
  
  TIM1_CCMR1 |= (TIM_CCMR1_CC2S_IN_TI1);//TI1 for CaptureChannel 2
  TIM1_CCER |= (TIM_CCER_CC2P);//Falling Edge
  TIM1_CCER |= (TIM_CCER_CC2E);//Enable capture on Channel2

  timer_enable_irq(TIM1, TIM_DIER_CC1IE);//Interrupts
  timer_enable_irq(TIM1, TIM_DIER_CC2IE);//Interrupts
  timer_enable_irq(TIM1, TIM_DIER_UIE);//Interrupts
  
  TIM1_PSC = 72;
  TIM1_CR1 |= TIM_CR1_CEN;  
  //TIM1

  //32bit Timer

  //I2C
  gpio_set_mode(GPIOB,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		GPIO8|GPIO9);			
  gpio_set(GPIOB,GPIO8|GPIO9);
  //I2C

  //TIM4
  gpio_set_mode(GPIOB,
		GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_FLOAT,
		GPIO6|GPIO7);
  TIM4_ARR = 0xFFFF;
  TIM4_CCMR1 |= (TIM_CCMR1_CC2S_IN_TI2 | TIM_CCMR1_CC1S_IN_TI1);
  TIM4_CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
  TIM4_SMCR |= TIM_SMCR_SMS_EM3;
  TIM4_CNT = 32767;
  TIM4_CR1 = TIM_CR1_CEN;
  //TIM4  

  gpio_primary_remap(0,AFIO_MAPR_I2C1_REMAP);  //I2C PB8 + PB9
  nvic_enable_irq(NVIC_TIM1_CC_IRQ);
  nvic_enable_irq(NVIC_TIM1_UP_IRQ);
  nvic_set_priority(NVIC_TIM1_CC_IRQ,0);
  nvic_set_priority(NVIC_TIM1_UP_IRQ,0);
}

static void
task1(void *args __attribute__((unused))) {

  lcd_init(LCD_DISP_ON);
  lcd_puts("Hello");


  char buffer[20];
  
  for (;;) {

    if(error){    
      sprintf(buffer,"%u", lastTimeValue);
      lcd_gotoxy(0,2);
      lcd_puts(buffer);

      sprintf(buffer,"%u",currTimeValue);
      lcd_gotoxy(0,3);
      lcd_puts(buffer);
    }

    
    sprintf(buffer,"%u", timesCalled);
    lcd_gotoxy(0,4);
    lcd_puts(buffer); 
  }
}

void tim1_cc_isr(){

  if(TIM1_SR & TIM_SR_CC1IF){
    timesCalled = (overflowCounter << 16) | TIM1_CCR1;
  }else if(TIM1_SR & TIM_SR_CC2IF){
    timesCalled = (overflowCounter << 16) | TIM1_CCR2;
  }
  
  if(timesCalled < lastTimesCalled){
    error = 1;
    lastTimeValue = lastTimesCalled;
    currTimeValue = timesCalled;    
  }
  
  lastTimesCalled = timesCalled;
}

void tim1_up_isr(){
  TIM1_SR &= ~TIM_SR_UIF;
  overflowCounter++;  
}

int main(void)
{
  rcc_clock_setup_in_hse_8mhz_out_72mhz();// For "blue pill"
  configurePeriphereals();
  xTaskCreate(task1,"task1",800,NULL,1,NULL);
  vTaskStartScheduler();
  for(;;);
  return 0;
}
