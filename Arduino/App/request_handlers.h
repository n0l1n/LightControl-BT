// ***************************************************************************
// Request handlers
// ***************************************************************************

#ifdef ENABLE_STATE_SAVE_SPIFFS
void tickerSpiffsSaveState(){
  updateStateFS = true;
}
#endif



char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}


unsigned int hexToDec(String hexString) {
    unsigned int decValue = 0;
    int nextInt;
    for (int i = 0; i < hexString.length(); i++) {
        nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
        nextInt = constrain(nextInt, 0, 15);
        decValue = (decValue * 16) + nextInt;
      }
    return decValue;
}




void getArgsof(String message){

             // validate form
            if (!((message.startsWith("["))&&(message.endsWith("]")))){
                  DBG_OUTPUT_PORT.println("[ERROR]: Form Validation failed!");
                  return;
            }
            
            String msg = message;
            // remove Frame
            msg = msg.substring(1,message.indexOf("]"));

                  int  lmode  =  hexToDec(msg.substring(0,2));  ws2812fx_mode      = lmode;
                  String wrgb = msg.substring(3,11);
                
                  uint8_t w  = hexToDec(wrgb.substring(0,2));      main_color.white   = w;
                  uint8_t r  = hexToDec(wrgb.substring(2,4));      main_color.red     = r;
                  uint8_t g  = hexToDec(wrgb.substring(4,6));      main_color.green   = g;
                  uint8_t b  = hexToDec(wrgb.substring(7));        main_color.blue    = b;
                
                  int br  = hexToDec(msg.substring(12,14));    brightness         = br;
                  int lspeed  = hexToDec(msg.substring(15));    ws2812fx_speed     = lspeed;
                  

      
            
                  main_color.white = constrain(main_color.white, 0, 255);
                  main_color.red   = constrain(main_color.red,   0, 255);
                  main_color.green = constrain(main_color.green, 0, 255);
                  main_color.blue  = constrain(main_color.blue,  0, 255);
            
              DBG_OUTPUT_PORT.print("Mode: ");
              DBG_OUTPUT_PORT.print(ws2812fx_mode);
              DBG_OUTPUT_PORT.print(", Color: ");
              DBG_OUTPUT_PORT.print(main_color.white);
              DBG_OUTPUT_PORT.print(", ");
              DBG_OUTPUT_PORT.print(main_color.red);
              DBG_OUTPUT_PORT.print(", ");
              DBG_OUTPUT_PORT.print(main_color.green);
              DBG_OUTPUT_PORT.print(", ");
              DBG_OUTPUT_PORT.print(main_color.blue);
              DBG_OUTPUT_PORT.print(", Speed:");
              DBG_OUTPUT_PORT.print(ws2812fx_speed);
              DBG_OUTPUT_PORT.print(", Brightness:");
              DBG_OUTPUT_PORT.println(brightness);
              mode = SET_MODE;
            }


uint16_t convertSpeed(uint8_t mcl_speed) {
  //long ws2812_speed = mcl_speed * 256;
  uint16_t ws2812_speed = 61760 * (exp(0.0002336 * mcl_speed) - exp(-0.03181 * mcl_speed));
  ws2812_speed = SPEED_MAX - ws2812_speed;
  if (ws2812_speed < SPEED_MIN) {
    ws2812_speed = SPEED_MIN;
  }
  if (ws2812_speed > SPEED_MAX) {
    ws2812_speed = SPEED_MAX;
  }
  return ws2812_speed;
}









 
