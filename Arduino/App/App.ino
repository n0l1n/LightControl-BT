#include "definitions.h"

#include <FS.h>
#include <EEPROM.h>

        // ***************************************************************************
        // Include: Bluetooth 
        // ***************************************************************************

          #include "BluetoothSerial.h"
          
          #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
                #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
          #endif

          BluetoothSerial SerialBT;
          char incoming;
          String message;


          
        // ***************************************************************************
        // Include: ANIMATION DRIVER  -- IF: NEOANIMATIONFX
        // ***************************************************************************
          
          #ifdef USE_NEOANIMATIONFX
                #include "NeoAnimationFX.h"
                #define NEOMETHOD NeoPBBGRB800
                
                NEOMETHOD neoStrip(NUMLEDS);
                NeoAnimationFX<NEOMETHOD> strip(neoStrip);
                
                // Uses Pin RX / GPIO3 (Only pin that is supported, due to hardware limitations)
                // NEOMETHOD NeoPBBGRB800 uses GRB config 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
                // NEOMETHOD NeoPBBGRB400 uses GRB config 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
                // NEOMETHOD NeoPBBRGB800 uses RGB config 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
                // NEOMETHOD NeoPBBRGB400 uses RGB config 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
                
                // Uses Pin D4 / GPIO2 (Only pin that is supported, due to hardware limitations)
                // NEOMETHOD NeoPBBGRBU800 uses GRB config 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
                // NEOMETHOD NeoPBBGRBU400 uses GRB config 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
                // NEOMETHOD NeoPBBRGBU800 uses RGB config 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
                // NEOMETHOD NeoPBBRGBU400 uses RGB config 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
          #endif

          // ***************************************************************************
          // Include: ANIMATION DRIVER  -- IF: WS2812FX --> Default
          // ***************************************************************************
          
          #ifdef USE_WS2812FX
                  #include "WS2812FX.h"
          
                  #ifdef RGBW
                          WS2812FX strip = WS2812FX(NUMLEDS, PIN, NEO_GRBW + NEO_KHZ800);
                  #else
                          WS2812FX strip = WS2812FX(NUMLEDS, PIN, NEO_GRB + NEO_KHZ800);
                  #endif
          
                  // Parameter 1 = number of pixels in strip
                  // Parameter 2 = Arduino pin number (most are valid)
                  // Parameter 3 = pixel type flags, add together as needed:
                  //   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
                  //   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
                  //   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
                  //   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
                  
                  // IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
                  // pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
                  // and minimize distance between Arduino and first pixel.  Avoid connecting
                  // on a live circuit...if you must, connect GND first.
          #endif

        // ***************************************************************************
        // Include: Request handlers
        // ***************************************************************************
        
        #include "request_handlers.h"

        // ***************************************************************************
        // Include: Color modes
        // ***************************************************************************
        
        #ifdef ENABLE_LEGACY_ANIMATIONS
                #include "colormodes.h"
        #endif

        
        // ***************************************************************************
        // EEPROM helper
        // ***************************************************************************
        #ifdef ENABLE_STATE_SAVE_EEPROM
                  String readEEPROM(int offset, int len) {
                        String res = "";
                        for (int i = 0; i < len; ++i){
                              res += char(EEPROM.read(i + offset));
                              //DBG_OUTPUT_PORT.println(char(EEPROM.read(i + offset)));
                        }
                        DBG_OUTPUT_PORT.printf("readEEPROM(): %s\n", res.c_str());
                        return res;
                  }
                  
                  void writeEEPROM(int offset, int len, String value) {
                        DBG_OUTPUT_PORT.printf("writeEEPROM(): %s\n", value.c_str());
                        for (int i = 0; i < len; ++i){
                              if (i < value.length()) {
                                  EEPROM.write(i + offset, value[i]);
                              } else {
                                  EEPROM.write(i + offset, NULL);
                              }
                        }
                  }
        #endif
      
      // ***************************************************************************
      // Saved state handling
      // ***************************************************************************
      // https://stackoverflow.com/questions/9072320/split-string-into-string-array
          String getValue(String data, char separator, int index){
                int found = 0;
                int strIndex[] = {0, -1};
                int maxIndex = data.length()-1;
              
                for(int i=0; i<=maxIndex && found<=index; i++){
                      if(data.charAt(i)==separator || i==maxIndex){
                              found++;
                              strIndex[0] = strIndex[1]+1;
                              strIndex[1] = (i == maxIndex) ? i+1 : i;
                      }
                }
              
                return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
          }





// ***************************************************************************
//            MAIN -- SETUP
// ***************************************************************************
void setup() {
  
          // ***************************************************************************
          // Setup: SERIAL
          // ***************************************************************************
              DBG_OUTPUT_PORT.begin(115200);
              DBG_OUTPUT_PORT.println("");
              DBG_OUTPUT_PORT.println("Starting....");

          // ***************************************************************************
          // Setup: BLUETOOTH SERIAL
          // ***************************************************************************
              SerialBT.begin("Anna_Lampe"); 
              DBG_OUTPUT_PORT.println("Starting Bluetooth....");

          // ***************************************************************************
          // Setup: Neopixel
          // ***************************************************************************
              DBG_OUTPUT_PORT.println("Starting NeoPixel INIT....");
              strip.init();
              strip.setBrightness(brightness);
              strip.setSpeed(convertSpeed(ws2812fx_speed));
              strip.setColor(main_color.white, main_color.red, main_color.green, main_color.blue);
              strip.start();
              DBG_OUTPUT_PORT.println("NeoPixel INIT....");
              DBG_OUTPUT_PORT.println(".... DONE!");

}







// ***************************************************************************
//            MAIN -- LOOP
// ***************************************************************************
void loop() {

          // ***************************************************************************
          // Get Bluetooth message
          // ***************************************************************************

             String message="";
                if (SerialBT.available()){ 
                      while(SerialBT.available()){
                          message += char(SerialBT.read()); 
                      }
                      DBG_OUTPUT_PORT.println(message);   
                      DBG_OUTPUT_PORT.println("Process new message ...");
                      DBG_OUTPUT_PORT.println(" ...    ...    ...    ...");
                      getArgsof(message);
                }

  

          // ***************************************************************************
          // Simple Statemashine Handler
          // ***************************************************************************
                  if (mode == SET_MODE) {
                      DBG_OUTPUT_PORT.printf("SET_MODE: %d %d\n", ws2812fx_mode, mode);
                      strip.setMode(ws2812fx_mode);
                      strip.trigger();
                      prevmode = SET_MODE;
                      mode = SETCOLOR;
                    }
                    if (mode == OFF) {
                      if(strip.isRunning()) strip.stop(); //should clear memory
                      // mode = HOLD;
                    }
                    if (mode == SETCOLOR) {
                      strip.setColor(main_color.white, main_color.red, main_color.green, main_color.blue);
                      strip.trigger();
                      mode = (prevmode == SET_MODE) ? SETSPEED : HOLD;
                    }
                    if (mode == SETSPEED) {
                      strip.setSpeed(convertSpeed(ws2812fx_speed));
                      strip.trigger();
                      mode = (prevmode == SET_MODE) ? BRIGHTNESS : HOLD;
                    }
                    if (mode == BRIGHTNESS) {
                      strip.setBrightness(brightness);
                      strip.trigger();
                      if (prevmode == SET_MODE) prevmode = HOLD;
                      mode = HOLD;
                    }
                    #ifdef ENABLE_LEGACY_ANIMATIONS
                          if (mode == WIPE) {
                            strip.setColor(main_color.white, main_color.red, main_color.green, main_color.blue);
                            strip.setMode(FX_MODE_COLOR_WIPE);
                            strip.trigger();
                            mode = HOLD;
                          }
                          if (mode == RAINBOW) {
                            strip.setMode(FX_MODE_RAINBOW);
                            strip.trigger();
                            mode = HOLD;
                          }
                          if (mode == RAINBOWCYCLE) {
                            strip.setMode(FX_MODE_RAINBOW_CYCLE);
                            strip.trigger();
                            mode = HOLD;
                          }
                          if (mode == THEATERCHASE) {
                            strip.setColor(main_color.white, main_color.red, main_color.green, main_color.blue);
                            strip.setMode(FX_MODE_THEATER_CHASE);
                            strip.trigger();
                            mode = HOLD;
                          }
                          if (mode == TWINKLERANDOM) {
                            strip.setColor(main_color.white, main_color.red, main_color.green, main_color.blue);
                            strip.setMode(FX_MODE_TWINKLE_RANDOM);
                            strip.trigger();
                            mode = HOLD;
                          }
                          if (mode == THEATERCHASERAINBOW) {
                            strip.setMode(FX_MODE_THEATER_CHASE_RAINBOW);
                            strip.trigger();
                            mode = HOLD;
                          }
                    #endif
                    if (mode == HOLD || mode == CUSTOM) {
                        if(!strip.isRunning()) strip.start();
                        #ifdef ENABLE_LEGACY_ANIMATIONS
                              if (exit_func) {
                                exit_func = false;
                              }
                        #endif
                      }
                    
                  
                    // Only for modes with WS2812FX functionality
                    #ifdef ENABLE_LEGACY_ANIMATIONS//--------
                          if (mode != TV && mode != CUSTOM) {
                    #else//----------------------------------
                          if (mode != CUSTOM) {//------------
                    #endif//---------------------------------
                    //---------------------------------------
                      strip.service();
                    }

          // ***************************************************************************
          // STATE SAVE
          // ***************************************************************************

            #ifdef ENABLE_STATE_SAVE_EEPROM
                    // Check for state changes
                    sprintf(current_state, "STA|%2d|%3d|%3d|%3d|%3d|%3d|%3d|%3d", mode, strip.getMode(), ws2812fx_speed, brightness, main_color.white, main_color.red, main_color.green, main_color.blue);
                
                    if (strcmp(current_state, last_state) != 0) {
                            // DBG_OUTPUT_PORT.printf("STATE CHANGED: %s / %s\n", last_state, current_state);
                            strcpy(last_state, current_state);
                            time_statechange = millis();
                            state_save_requested = true;
                    }
                    if (state_save_requested && time_statechange + timeout_statechange_save <= millis()) {
                            time_statechange = 0;
                            state_save_requested = false;
                            writeEEPROM(256, 36, last_state); // 256 --> last_state (reserved 36 bytes)
                            EEPROM.commit();
                    }
            #endif



} // End of Loop
