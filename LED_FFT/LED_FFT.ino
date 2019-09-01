/*Test Fourier transform of Microphone
 *===========================================================================
 * Aaron Au 2019
 * LED reacts to FFT from audio
 * 
 * TODO
 * 1. no music mode
 * 2. dynamic decay
*/

//LIBRARIES
//===========================================================================
#include <NeoPixelBus.h>
#include "arduinoFFT.h"
#include "math.h"

//CONSTANTS
//===========================================================================
#define audioPin A5
#define PixelPin 2

const uint8_t PixelCount = 26;
const double colorSat = 40;
const double maxBright = 80;
const uint16_t refresh = 5;
const int expand = 10;
const int space = 40;

const uint16_t samples = 128; // Must be power of 2
const double  samplingFreq = 8500;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin); /* Create LED object */
arduinoFFT FFT = arduinoFFT(); /* Create FFT object */

uint16_t    spectrum[samples/2];

double  vReal[samples];
double vImag[samples];
double prevF1 = 0;
double prevF2 = 0;

double currBright = 0.0;
double decay = 0.25 ;
uint8_t f1 = 6; //~400Hz
uint8_t f2 = 11; //~800Hz

int colors = 20;

double color[20][3] = {{random(0,colorSat),random(0,colorSat),random(0,colorSat)}
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
int colorVals[PixelCount];

int start = 0;
uint8_t delta = 30;
uint8_t spd = 5; 
bool posDir = false;

void setup()
{
  Serial.begin(115200);
  Serial.println("Ready");

  randomSeed(analogRead(0));

  //Initialize colorVals
  for (int i = 0; i < 26; i++) {
    colorVals[i] = i*space;
  }
}

void loop()
{
 /* Gather data for sample length */
 for (uint16_t i = 0; i < samples; i++) {
  vReal[i] = analogRead(audioPin);
  vImag[i] = 0.0;
 }
 
 /* Forward FFT Compute */ 
 FFT.Windowing(vReal, samples, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
 FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
 FFT.ComplexToMagnitude(vReal, vImag, samples);

 /*Brightness based if f1 and f2 increases*/
 if (vReal[f1] > prevF1 && vReal[f2] > prevF2){
    currBright = maxBright/colorSat;
    posDir = !posDir;
 } else {
    currBright = currBright - decay;
 }
 if (currBright < 1){
  currBright = 1;
 }

 /*Assign Colour and Brightness*/
 for (int i = 0; i < PixelCount; i++) {
  //MOVE PER PIXEL
  if (posDir) {
  colorVals[i] = colorVals[i] + spd + random(-1*expand, expand+1);
  } else {
    colorVals[i] = colorVals[i] - spd + random(-1*expand, expand+1);
  }
  if (colorVals[i] < 0) {
    colorVals[i] = 2000 + colorVals[i];
  } else if (colorVals[i] > 2000) {
    colorVals[i] = colorVals[i] - 1900;
  }
  uint16_t r = (colorVals[i]/100)%colors;
  int r2 = (r + 1)%colors;
  col = RgbColor::LinearBlend(RgbColor(currBright*color[r][0],currBright*color[r][1],currBright*color[r][2]), RgbColor(currBright*color[r2][0],currBright*color[r2][1],currBright*color[r2][2]), (colorVals[i])%100/100.0);
  strip.SetPixelColor(i, col);
 }
 strip.Show();

 prevF1 = vReal[f1];
 prevF2 = vReal[f2];
}
