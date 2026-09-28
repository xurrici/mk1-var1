#include "keyboard.h"

void initKeyboard() {
  GPIOB->MODER &= ~(3U << 14 | 3U << 12 | 3U << 6 | 3U << 20 | 3U << 8 | 3U << 10);
  GPIOA->MODER &= ~(3U << 20 | 3U << 18);
  GPIOB->MODER |= (1U << 14 | 1U << 12 | 1U << 6);
  GPIOA->MODER |= (1U << 20);
  GPIOB->OTYPER |= (1 << 7) | (1 << 6) | (1 << 3);
  GPIOA->OTYPER |= (1 << 10);
  GPIOB->PUPDR |= (1U << 20) | (1U << 8) | (1U << 10);
  GPIOA->PUPDR |= (1U << 18);
}

char readKey() {
  const uint8_t rows[] = {7, 6, 10, 3}; 
  const uint8_t cols[] = {10, 4, 5, 9};
  const char keymap[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
  };

  for (uint8_t i = 0; i < 4; i++) {
    if (rows[i] == 10) GPIOA->BRR = (1 << 10); else GPIOB->BRR = (1 << rows[i]);
    for (uint8_t j = 0; j < 4; j++) {
      int pressed = (cols[j] == 9) ? !(GPIOA->IDR & (1 << 9)) : !(GPIOB->IDR & (1 << cols[j]));
      if (pressed) {
        if (rows[i] == 10) GPIOA->BSRR = (1 << 10); else GPIOB->BSRR = (1 << rows[i]);
        return keymap[i][j];
      }
    }
    if (rows[i] == 10) GPIOA->BSRR = (1 << 10); else GPIOB->BSRR = (1 << rows[i]);
  }
  return '\0';
}