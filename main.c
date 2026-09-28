#include "main.h"
#include "tm1637.h"
#include "keyboard.h"

volatile uint32_t tickCount = 0;
uint16_t counter = 0;
int op1 = 0, op2 = 0, op = 0, state = 0;

int main(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
    tm1637_init();
    initKeyboard();

    while (1) {
        char key = readKey();
        if (key != '\0') {
            while (readKey() != '\0') { for(volatile int i=0; i<10000; i++); }

            if (key >= '0' && key <= '9') {
                if (state == 0 || state == 2) { 
                    op1 = key - '0'; 
                    counter = op1; 
                    state = 0; 
                } else { 
                    op2 = key - '0'; 
                    counter = op2; 
                }
                tm1637_display_number(counter);
            } 
            else if (key >= 'A' && key <= 'D') {
                state = 1;
                op = (key - 'A') + 1;
                tm1637_display_number(9 + op);
            } 
            else if (key == '#') {
                int res = 0;
                if (op == 1) res = op1 + op2;
                else if (op == 2) res = (op1 >= op2) ? (op1 - op2) : 0;
                else if (op == 3) res = op1 * op2;
                else if (op == 4) res = (op2 != 0) ? op1 / op2 : 0;
                
                counter = (uint16_t)res;
                tm1637_display_number(counter);
                state = 2;
                op = 0;
            }
        }
    }
}