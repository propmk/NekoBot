// NekoBot
// Your cute desk buddy

#include <Arduino_GFX_Library.h>
#include "images.h"

Arduino_ST7789 *gfx = new Arduino_ST7789(
  new Arduino_ESP32SPI(-1, 5, 18, 23),
  4, 0, false, 240, 320
);

enum FrameIndex {
  EYE_CLOSE_MOUTH_CLOSE,
  EYE_OPEN_MOUTH_OPEN,
  EYE_CLOSE_MOUTH_OPEN,
  EYE_OPEN_MOUTH_CLOSE
};

const uint16_t* frames[] = {
  Eye_Close_Mouth_Close_565,
  Eye_Open_Mouth_Open_565,
  Eye_Close_Mouth_Open_565,
  Eye_Open_Mouth_Close_565
};

void drawFrame(int index) {
  gfx->draw16bitRGBBitmap(0, 0, frames[index], 320, 240);
}

int state = 0;
unsigned long lastChange = 0;

// States:
// 0 = idle
// 1 = blink
// 2 = mouth move

void idleAnimation() {
  unsigned long now = millis();

  switch (state) {

    case 0:
      drawFrame(EYE_OPEN_MOUTH_CLOSE);

      if (now - lastChange > random(2000, 5000)) {
        int r = random(0, 3);

        if (r == 0) {
          state = 1;
        } else {
          state = 2;
        }

        lastChange = now;
      }
      break;

    case 1:
      drawFrame(EYE_CLOSE_MOUTH_CLOSE);
      delay(120); 

      drawFrame(EYE_OPEN_MOUTH_CLOSE);

      state = 0;
      lastChange = millis();
      break;

    case 2:
      drawFrame(EYE_OPEN_MOUTH_OPEN);
      delay(200);

      drawFrame(EYE_OPEN_MOUTH_CLOSE);

      state = 0;
      lastChange = millis();
      break;
  }
}

void setup() {
  gfx->begin(40000000);

  gfx->setRotation(1);

  randomSeed(esp_random());

  drawFrame(EYE_OPEN_MOUTH_CLOSE);
}

void loop() {
  idleAnimation();
}