// Name: main.c
// Project: Smart-Response-PE/examples/talk-back
// Author: Aaron Berger <code@bergera.com>
// Creation Date: 2019-04-01
// License: MIT

#include "hal.h"
#include "util.h"
#include "string_utils.h"
#include "clock.h"
#include "display.h"
#include "keypad.h"

int x_offset = 0;
int y_offset = 0;

inline void setup() {
  display_init();
  display_set_frame_rate(30);
}

inline void loop() {
  uint32_t millis;

  // Wait until its time to render our next frame
  if (!display_next_frame())
    return;
  
  // Poll the keypad (disable/enable UART to reduce shared pin conflicts)
  keypad_poll();

  // Display the name of any currently pressed buttons
  for (uint8_t col=0; col<4; col++)
  {
    for (uint8_t row=0; row<8; row++)
    {
      uint8_t mask = util_bit_to_mask[row];
      if (keypad_pressed(col, mask))
      {
        display_set_cursor(x_offset,y_offset);
        display_print(keypad_get_button_name(col, mask));
        x_offset++;
        if(x_offset > 10)
        {
          x_offset = 0;
          y_offset++;
        }
        if(y_offset > 10)
        {
          y_offset = 0;
          //clear screen
          display_init();
        }
      }
    }
  }

  // Paint the screen buffer to the LCD
  display_paint();
}

void main() {
  oscillator_32mhz();
  clock_init();
  ENABLE_INTERRUPTS;
  setup();

  while(1) {
    loop();
  }
}
