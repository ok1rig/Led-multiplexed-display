/*

7Segment display 
    ___A___
   |       | 
   |F     B|
   |___G___|
   |       |
   |E     C|
   |___D___| o DP
   

*/


//Segment Wiring
#define LED_A (1<<PB0)  // 1
#define LED_B (1<<PB1)  // 2
#define LED_C (1<<PB2)  // 4
#define LED_D (1<<PB3)  // 8
#define LED_E (1<<PB4)  // 16
#define LED_F (1<<PB5)  // 32
#define LED_G (1<<PB6)  // 64
#define LED_DP (1<<PB7) // 128

//Ports 
#define LED_PORT PORTB
#define LED_DDR DDRB
#define CATHODE_PORT PORTD
#define CATHODE_DDR DDRD

#define cathode_1 (1<<PD6)
#define cathode_2 (1<<PD5)
#define cathode_3 (1<<PD4)
#define cathode_4 (1<<PD3)

//Digits definition
#define digit_0 LED_A + LED_B + LED_C + LED_D + LED_E + LED_F
#define digit_1 LED_B + LED_C
#define digit_2 LED_A + LED_B + LED_G + LED_E + LED_D
#define digit_3 LED_A + LED_B + LED_G + LED_C + LED_D
#define digit_4 LED_F + LED_G + LED_B + LED_C
#define digit_5 LED_A + LED_F + LED_G + LED_C + LED_D
#define digit_6 LED_A + LED_F + LED_E + LED_D + LED_C + LED_G
#define digit_7 LED_A + LED_B + LED_C
#define digit_8 LED_A + LED_B + LED_C + LED_D + LED_E + LED_F + LED_G
#define digit_9 LED_G + LED_F + LED_A + LED_B + LED_C + LED_D
#define digit_a LED_E + LED_F + LED_A + LED_B + LED_C + LED_G
#define digit_b LED_F + LED_E + LED_D + LED_C + LED_G
#define digit_c LED_A + LED_F + LED_E + LED_D
#define digit_d LED_B + LED_C + LED_D + LED_E + LED_G
#define digit_e LED_A + LED_B + LED_G + LED_C + LED_D
#define digit_f LED_A + LED_B + LED_G + LED_C
