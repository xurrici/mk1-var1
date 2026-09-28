#include "tm1637.h"

void delay_us(uint32_t us) {
  volatile uint32_t cycles = us * 24;
  while (cycles-- > 0) { asm("nop"); }
}

void tm1637_init(void) {
  GPIOA->MODER = (GPIOA->MODER & ~(0xF << (TM1637_CLK_PIN * 2))) | (0x5 << (TM1637_CLK_PIN * 2));
  GPIOA->OTYPER |= (1 << TM1637_CLK_PIN) | (1 << TM1637_DIO_PIN);
  GPIOA->BSRR = (1 << TM1637_CLK_PIN) | (1 << TM1637_DIO_PIN);
  tm1637_start(); tm1637_write_byte(TM1637_CMD1); tm1637_stop();
  tm1637_start(); tm1637_write_byte(TM1637_CMD3); tm1637_stop();
}

void tm1637_start(void) {
  GPIOA->BSRR = (1 << TM1637_DIO_PIN) | (1 << TM1637_CLK_PIN);
  delay_us(5);
  GPIOA->BRR = (1 << TM1637_DIO_PIN);
  delay_us(5);
}

void tm1637_stop(void) {
  GPIOA->BRR = (1 << TM1637_CLK_PIN);
  GPIOA->BRR = (1 << TM1637_DIO_PIN);
  delay_us(5);
  GPIOA->BSRR = (1 << TM1637_CLK_PIN);
  delay_us(5);
  GPIOA->BSRR = (1 << TM1637_DIO_PIN);
}

void tm1637_write_byte(uint8_t byte) {
  for (uint8_t i = 0; i < 8; i++) {
    GPIOA->BRR = (1 << TM1637_CLK_PIN);
    if (byte & 0x01) GPIOA->BSRR = (1 << TM1637_DIO_PIN);
    else GPIOA->BRR = (1 << TM1637_DIO_PIN);
    delay_us(5);
    GPIOA->BSRR = (1 << TM1637_CLK_PIN);
    byte >>= 1;
    delay_us(5);
  }
  GPIOA->BRR = (1 << TM1637_CLK_PIN);
  GPIOA->BSRR = (1 << TM1637_DIO_PIN);
  delay_us(5);
  GPIOA->BSRR = (1 << TM1637_CLK_PIN);
  delay_us(5);
}

void tm1637_display_number(int number) {
  const uint8_t codes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x70, 0x40, 0x63, 0x52};
  uint8_t d[4] = {0, 0, 0, 0};
  
  if (number >= 10 && number <= 13) {
      d[3] = codes[number]; 
  } else {
      d[3] = codes[number % 10];
      d[2] = (number >= 10) ? codes[(number / 10) % 10] : 0;
      d[1] = (number >= 100) ? codes[(number / 100) % 10] : 0;
      d[0] = (number >= 1000) ? codes[(number / 1000) % 10] : 0;
  }

  tm1637_start(); tm1637_write_byte(0x40); tm1637_stop();
  tm1637_start(); tm1637_write_byte(0xC0);
  for (int i = 0; i < 4; i++) tm1637_write_byte(d[i]);
  tm1637_stop();
  tm1637_start(); tm1637_write_byte(0x8F); tm1637_stop();
}

void tm1637_clear(void) { tm1637_display_number(0); }
void tm1637_update(void) { }