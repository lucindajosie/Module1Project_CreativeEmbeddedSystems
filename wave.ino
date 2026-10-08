#include <cmath>
#include <TFT_eSPI.h>
#include "image.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

float positionX = -20.0; //horizontal position of the wave
float positionY = -25.0; //vertical position of the wave

float xvel = 0.8; //horizontal velocity of the wave
float yvel = 0.48; //vertical velocity of the wave

const int TRAIL_LENGTH = 4; //number of previous positions to store for the trail

float trailX[TRAIL_LENGTH]; //array to store the previous horizontal positions of the wave
float trailY[TRAIL_LENGTH]; //array to store the previous vertical positions of the wave

void setup(){

  //initialize the TFT display
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  randomSeed(esp_random());

  //setup sprite to get rid of flickering
  sprite.setColorDepth(16);
  sprite.createSprite(240, 135);
  sprite.setSwapBytes(true);
  sprite.fillSprite(TFT_BLACK);

  //initialize the trail arrays with the starting position of the wave
  for (int i = 0; i < TRAIL_LENGTH; i++){
    trailX[i] = positionX;
    trailY[i] = positionY;
  }
}

//convert Desmos x coordinates to screen coordinates
int toScreenX(float x) {
  return (x + 5.1) / 7.1 * 239;
}

//convert Desmos y coordinates to screen coordinates
int toScreenY(float y) {
  float center = 2.0;
  float stretch = 1.3;

  y = center + (y - center) * stretch;

  return 134 - ((y + 1.0) / 6.0 * 134);
}

//draw the wave on the sprite
void drawWave(float posX, float posY, uint16_t color){

  //for-loops to draw the wave using the Desmos equations; loops through the x values
  for (float x = -5.02; x < -3.89; x += 0.02) {

    //use Desmos equations to calculate the y value for the current x value
    float y = 0.9 * (sin(x - 2.7) + 1.7);

    //convert Desmos coordinates to screen coordinates
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

  //update the position of the wave based on its velocity
  positionX +=xvel;
  positionY += yvel;

  //left and right boundaries for the wave's movement
  float minX = -3;
  float maxX = 51;

  //change the direction of the wave's movement if it hits the left or right boundaries
  if (positionX <= minX){
    positionX = minX;
    xvel = random(50, 101)/100.0; //creates a new random horizontal velocity
  }

  if (positionX >= maxX){
    positionX = maxX;
    xvel = -random(50, 101)/100.0;
  }

  //top and bottom boundaries for the wave's movement
  float minY = -41;
  float maxY = -10;

  //change the direction of the wave's movement if it hits the top or bottom boundaries
  if (positionY <= minY){
    positionY = minY;
    yvel = random(30, 71)/100.0;
  }

  if (positionY >= maxY){
    positionY = maxY;
    yvel = -random(30, 71)/100.0;
  }

  //creates a trail effect
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

  //draw the wave at its current position
  for (int i = TRAIL_LENGTH - 1; i >= 0; i--){
    drawWave(trailX[i], trailY[i], TFT_WHITE);
  }

  sprite.pushSprite(0, 0);

  delay(30);

}