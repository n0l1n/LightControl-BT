// ***************************************************************************
// DEFINE: Animation driver
// ***************************************************************************     
      //#define USE_NEOANIMATIONFX  // Uses NeoAnimationFX, PIN is ignored & set to RX/GPIO3 or UART method: D4/GPIO2, see: https://github.com/debsahu/NeoAnimationFX
      #define USE_WS2812FX          // Uses WS2812FX, see: https://github.com/kitesurfer1404/WS2812FX

// ***************************************************************************
// DEFINE: Pins
// ***************************************************************************  
      #define PIN 15             // PIN (15 / D8) where neopixel / WS2811 strip is attached 
      #define NUMLEDS 5         // Number of leds in the strip 
      #define BUILTIN_LED 2      // ESP-12F has the built in LED on GPIO2, see https://github.com/esp8266/Arduino/issues/2192

// ***************************************************************************
// DEFINE: LED Type
// ***************************************************************************  
      #define RGBW               // If defined, use RGBW Strips
      
// ***************************************************************************
// DEFINE: other Options
// ***************************************************************************  
      #define ENABLE_LEGACY_ANIMATIONS
      #define ENABLE_STATE_SAVE_SPIFFS        // If defined, saves state on SPIFFS


// ***************************************************************************
// DEFINE: Alias
// ***************************************************************************   
      #define DBG_OUTPUT_PORT Serial  // Set debug output port


// ***************************************************************************
// DEFINE-ERROR Handling
// ***************************************************************************  
      #if defined(USE_NEOANIMATIONFX) and defined(USE_WS2812FX)
            #error "Cant have both NeoAnimationFX and WS2812FX enabled. Choose either one."
      #endif
      
      #if !defined(USE_NEOANIMATIONFX) and !defined(USE_WS2812FX)
            #error "Need to either use NeoAnimationFX and WS2812FX mode."
      #endif


// ***************************************************************************
// DEFINE: Autocycling Pattern --- if in Mode AUTO
// ***************************************************************************

// parameters for automatically cycling favorite patterns
uint32_t autoParams[][4] = {   // color, speed, mode, duration (seconds)
  {0x00ff0000, 120,  1,  5.0}, // blink red for 5 seconds
  {0x0000ff00, 120,  3, 10.0}, // wipe green for 10 seconds
  {0x000000ff, 196, 14,  5.0}, // dual scan blue for 5 seconds
  {0x000000ff, 196, 46, 15.0}  // fireworks for 15 seconds
};





// ***************************************************************************
// Global variables / definitions
// ***************************************************************************

      // List of all color modes
      #ifdef ENABLE_LEGACY_ANIMATIONS
            enum MODE { SET_MODE, HOLD, AUTO, OFF, TV, CUSTOM, SETCOLOR, SETSPEED, BRIGHTNESS, WIPE, RAINBOW, RAINBOWCYCLE, THEATERCHASE, TWINKLERANDOM, THEATERCHASERAINBOW};
            MODE mode = RAINBOW;         // Standard mode that is active when software starts
            bool exit_func = false;      // Global helper variable to get out of the color modes when mode changes
      #else
            enum MODE { SET_MODE, HOLD, AUTO, OFF, TV, CUSTOM, SETCOLOR, SETSPEED, BRIGHTNESS};
            MODE mode = SET_MODE;        // Standard mode that is active when software starts
      #endif


      MODE prevmode = mode;
        
        // ***************************************************************************
        // DEFINE: Default properties
        // ***************************************************************************
            int ws2812fx_speed = 196;   // Global variable for storing the delay between color changes --> smaller == faster
            int brightness = 20;       // Global variable for storing the brightness (255 == 100%)
            int ws2812fx_mode = 0;      // Helper variable to set WS2812FX modes
          
     

        // ***************************************************************************
        // DEFINE: Datatype LED STATE
        // ***************************************************************************
            struct ledstate             // Data structure to store a state of a single led
            {
              uint8_t red;
              uint8_t green;
              uint8_t blue;
              uint8_t white;
            };
          typedef struct ledstate LEDState;     // Define the datatype LEDState

         
        // ***************************************************************************
        // DEFINE: LED STATE for entire LED-Strip
        // *************************************************************************** 
            LEDState ledstates[NUMLEDS];          // Get an array of led states to store the state of the whole strip
            LEDState main_color = { 0, 255, 0, 0};  // Store the "main color" of the strip used in single color modes
            

      
        // ***************************************************************************
        // DEFINE: State saving
        // ***************************************************************************
            #ifdef ENABLE_STATE_SAVE_EEPROM
                  char current_state[36];               // Keeps the current state representation
                  char last_state[36];                  // Save the last state as string representation
                  unsigned long time_statechange = 0;   // Time when the state last changed
                  int timeout_statechange_save = 5000;  // Timeout in ms to wait before state is saved
                  bool state_save_requested = false;    // State has to be saved after timeout
            #endif
            #ifdef ENABLE_STATE_SAVE_SPIFFS
                  bool updateStateFS = false;
            #endif
