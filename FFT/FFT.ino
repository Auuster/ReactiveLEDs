/*Test Fourier transform of Microphone
 *===========================================================================
 * Aaron Au 2019
 * Takes samples and breaks down into frequency space via arduinoFFT package  
*/

//LIBRARIES
//===========================================================================
#include "arduinoFFT.h"
#include "math.h"

//CONSTANTS
//===========================================================================
#define audioPin A5
const uint16_t samples = pow(2,7); // Number of samples, MUST BE POWER OF 2
const String prt = ""; //'spectrum', 'peak', 'beat', 'tone'
//Spectrum >>> print FFT spectrum
//Peak >>> print Peak and Magnitude of peak
//Beat >>> print whenever f1 > pastF1 and f2 > past f2
//Tone >>> print Tone matching use > https://www.szynalski.com/tone-generator/ 
const uint8_t f1 = 6;
const uint8_t f2 = 11;

//VARIABLES
//===========================================================================
double val;
double vReal[samples];
double vImag[samples];
double peak;
double mag;
double pastF1;
double pastF2;

//OBJECTS
//===========================================================================
arduinoFFT FFT = arduinoFFT(); // Create FFT object

void setup()
{
  Serial.begin(115200);
  Serial.println("ArduinoFFT - Aaron Au 2019");
}

void loop()
{
 for (uint16_t i = 0; i < samples; i++) {
  val = analogRead(audioPin);
  vReal[i] = val;
  vImag[i] = 0.0;
 }
 
 /* Forward FFT Compute */
 FFT.Windowing(vReal, samples, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
 FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
 FFT.ComplexToMagnitude(vReal, vImag, samples);
 FFT.MajorPeak(vReal, samples, 8500, &peak, &mag); //Finds peak + mag

 if (prt.equals("spectrum")){
  for (int i = 2; i < samples-2; i++) {
    Serial.println(vReal[i]);
  }
  for (int i = 0; i< 10; i++) {
    Serial.println(0);
  }
 }

 if (prt.equals("peak")){
  Serial.print(peak);
  Serial.print(',');
  Serial.println(mag);
 }

 if (prt.equals("beat")) {
  if (pastF1 < abs(vReal[f1-1] - (2.0 * vReal[f1]) + vReal[f1+1])){
    Serial.print(1);
  } else {
   Serial.print(0);
  }
   Serial.print(',');
  if(pastF1 < abs(vReal[f1-1] - (2.0 * vReal[f1]) + vReal[f1+1]) && pastF2 < abs(vReal[f2-1] - (2.0 * vReal[f2]) + vReal[f2+1])) {
   Serial.println(1);
  } else {
   Serial.println(0);
  }
 }

 pastF1 = abs(vReal[f1-1] - (2.0 * vReal[f1]) + vReal[f1+1]);
 pastF2 = abs(vReal[9] - (2.0 * vReal[10]) + vReal[f2+1]);
 
 if (prt.equals("tone")){
  for (int i = 0; i < samples-1; i++){
   if (abs(vReal[i - 1] - (2.0 * vReal[i]) + vReal[i + 1]) == mag) {
    Serial.println(i);
   }
  }
 } 
}
