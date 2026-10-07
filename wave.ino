#include <cmath>
#include <TFT_eSPI.h>
#include "image.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

float positionX = -20.0;
float positionY = -25.0;

float xvel = 0.8;
float yvel = 0.48;

const int TRAIL_LENGTH = 4;

float trailX[TRAIL_LENGTH];
float trailY[TRAIL_LENGTH];

void setup(){

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  randomSeed(esp_random());

  sprite.setColorDepth(16);
  sprite.createSprite(240, 135);
  sprite.setSwapBytes(true);
  sprite.fillSprite(TFT_BLACK);

  for (int i = 0; i < TRAIL_LENGTH; i++){
    trailX[i] = positionX;
    trailY[i] = positionY;
  }
}

int toScreenX(float x) {
  return (x + 5.1) / 7.1 * 239;
}

int toScreenY(float y) {
  float center = 2.0;
  float stretch = 1.3;

  y = center + (y - center) * stretch;

  return 134 - ((y + 1.0) / 6.0 * 134);
}

void drawWave(float posX, float posY, uint16_t color){

  for (float x = -5.02; x < -3.89; x += 0.02) {

    // Original Desmos equation
    float y = 0.9 * (sin(x - 2.7) + 1.7);

    // Convert Desmos coordinates to screen coordinates
    int wavex = toScreenX(x) + posX;
    int wavey = toScreenY(y) + posY;

    sprite.drawPixel(wavex, wavey, color);
  }

  for (float x = -3.9; x < -2.78; x += 0.02){
    float y = 1.1* cos(x+2.2) + 1.4;

    int wavex = toScreenX(x) + posX;
    int wavey = toScreenY(y) + posY;

    sprite.drawPixel(wavex, wavey, color);
  }

  for (float x = -3.041; x < -1; x +=0.02){
    float y = -0.9 * sin(0.8*(x - 0.7)) + 2;

    int wavex = toScreenX(x) + posX;
    int wavey = toScreenY(y) + posY;

    sprite.drawPixel(wavex, wavey, color);
  }

  for (float x = -1.02; x < 0.5; x +=0.02){
    float y = -0.9 * sin(x - 0.4) + 2;

    int wavex = toScreenX(x) + posX;
    int wavey = toScreenY(y) + posY;

    sprite.drawPixel(wavex, wavey, color);
  }

  for (float x = -0.18; x < 0.48; x +=0.002){
    float y = 1.34 + 0.8485*(x + 0.18) + 2.6*(x + 0.18)*(x-0.48);

    int wavex = toScreenX(x) + posX;
    int wavey = toScreenY(y) + posY;

    sprite.drawPixel(wavex, wavey, color);
  }

  for (float x = -0.641; x < -0.19; x +=0.02){
    float y = (2.2*x) + (1/(1.6*(x+1))) + 1;

    int wavex = toScreenX(x) + posX;
    int wavey = toScreenY(y) + posY;

    sprite.drawPixel(wavex, wavey, color);
  }

  for (float x = -0.947; x < -0.58; x +=0.02){
    float y = -(((x*x)-x-2.5)/(x+1.9));

    int wavex = toScreenX(x) + posX;
    int wavey = toScreenY(y) + posY;

    sprite.drawPixel(wavex, wavey, color);
  }

  for (float x = -0.977; x < -0.941; x +=0.005){
    float y = -pow(80.0, -9.0 * (x + 1.0)) + 0.8;

    int wavex = toScreenX(x) + posX;
    int wavey = toScreenY(y) + posY;

    sprite.drawPixel(wavex, wavey, color);
  }

  for (float x = -0.977; x < -0.923; x +=0.005){
    float y = pow(80.0, -6.8 * (x + 1.0)) - 0.1;

    int wavex = toScreenX(x) + posX;
    int wavey = toScreenY(y) + posY;

    sprite.drawPixel(wavex, wavey, color);
  }

  for (float x = -0.918; x < -0.59; x +=0.005){
    float y = ((2.7 * x * x) - 0.4) / (x + 3.0) - 0.9;

    int wavex = toScreenX(x) + posX;
    int wavey = toScreenY(y) + posY;

    sprite.drawPixel(wavex, wavey, color);
  }

}

int trailCounter = 0;

void loop() {

  sprite.pushImage(0, 0, 240, 135, IMG_2395);

  positionX +=xvel;
  positionY += yvel;

  float minX = -3;
  float maxX = 51;

  if (positionX <= minX){
    positionX = minX;
    xvel = random(50, 101)/100.0;
  }

  if (positionX >= maxX){
    positionX = maxX;
    xvel = -random(50, 101)/100.0;
  }

  float minY = -41;
  float maxY = -10;

  if (positionY <= minY){
    positionY = minY;
    yvel = random(30, 71)/100.0;
  }

  if (positionY >= maxY){
    positionY = maxY;
    yvel = -random(30, 71)/100.0;
  }

  trailCounter++;
  if(trailCounter >= 8){
    for (int i = TRAIL_LENGTH - 1; i > 0; i--){
      trailX[i] = trailX[i-1];
      trailY[i] = trailY[i -1];
    }

    trailX[0] = positionX;
    trailY[0] = positionY;

    trailCounter = 0;
  }

  for (int i = TRAIL_LENGTH - 1; i >= 0; i--){
    drawWave(trailX[i], trailY[i], TFT_WHITE);
  }

  sprite.pushSprite(0, 0);

  delay(30);

}