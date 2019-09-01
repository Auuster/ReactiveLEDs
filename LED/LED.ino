/*LED Simple Animation
 * ============================================================================
 * Aaron Au 2019
 */

//LIBRARIES
//===========================================================================
#include <NeoPixelBus.h>

//CONSTANTS
//===========================================================================
const uint16_t PixelCount = 26; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266
const uint8_t colorSat = 20;
const uint16_t space = 50;
const int spd = 5;

//VARIABLES
//===========================================================================
bool posDir = false;
int colorVals[26];

//OBJECTS
//===========================================================================
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

uint8_t color[20][3] = {{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
                      ,{random(0,colorSat),random(0,colorSat),random(0,colorSat)}};
RgbColor col;

void setup()
{
  Serial.begin(115200);
  Serial.println("Ready");

  // this resets all the neopixels to an off state
  strip.Begin();
  strip.Show();

  for (int i = 0; i < 26; i++) {
    colorVals[i] = i*space;
  }
}

void loop()
{
  /*if (random(0,1)){
    posDir = !posDir;
  }*/
  for (int i = 0; i < PixelCount; i ++) {
    //MOVE PER PIXEL
    if (posDir) {
      colorVals[i] = colorVals[i] + spd + random(-10, 11);
    } else {
      colorVals[i] = colorVals[i] - spd + random(-10, 11);
    }
    if (colorVals[i] < 0) {
      colorVals[i] = 2000 + colorVals[i];
    } else if (colorVals[i] > 2000) {
      colorVals[i] = colorVals[i] - 1900;
    }
    uint16_t r = ((colorVals[i])/100)%20;
    int r2 = (r + 1)%20;
    col = RgbColor::LinearBlend(RgbColor(color[r][0],color[r][1],color[r][2]), RgbColor(color[r2][0],color[r2][1],color[r2][2]), (colorVals[i]%100)/100.0);
    strip.SetPixelColor(i, col);
  }    
  strip.Show();
  delay(50);  
}
