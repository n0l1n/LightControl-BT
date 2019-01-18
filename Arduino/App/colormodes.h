// ***************************************************************************
// Color modes
// ***************************************************************************

int dipInterval = 10;
int darkTime = 250;
unsigned long currentDipTime;
unsigned long dipStartTime;
unsigned long currentMillis;
int ledState = LOW;
long previousMillis = 0; 
int led = 5;
int interval = 2000;
int twitch = 50;
int dipCount = 0;
int analogLevel = 100;
boolean timeToDip = false;
int ledStates[NUMLEDS];


void hsb2rgbAN1(uint16_t index, uint8_t sat, uint8_t bright, uint8_t myled) {
    // Source: https://blog.adafruit.com/2012/03/14/constant-brightness-hsb-to-rgb-algorithm/
    uint8_t temp[5], n = (index >> 8) % 3;
    temp[0] = temp[3] = (uint8_t)((                                         (sat ^ 255)  * bright) / 255);
    temp[1] = temp[4] = (uint8_t)((((( (index & 255)        * sat) / 255) + (sat ^ 255)) * bright) / 255);
    temp[2] =           (uint8_t)(((((((index & 255) ^ 255) * sat) / 255) + (sat ^ 255)) * bright) / 255);

    strip.setPixelColor(myled, temp[n + 2], temp[n + 1], temp[n], 0);
}


void updateLed (int led, int brightness) {
	ledStates[led] = brightness;
	
	for (int i=0; i<NUMLEDS; i++)
	{
		uint16_t index = (i%3 == 0) ? 400 : random(0,767);
		hsb2rgbAN1(index, 200, ledStates[i], i);
	}
	strip.show();
}
