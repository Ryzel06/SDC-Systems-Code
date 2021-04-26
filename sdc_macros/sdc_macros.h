#include <Arduino.h>

#ifndef SDC_MACROS_H
#define SDC_MACROS_H

#define ABSL_FALLTHROUGH_INTENDED

/* A function to take in user input from the Serial Monitor.
 * @param prompt The message to tell the user indicating what is expected to be entered.
 * @param buffer_size The maximum length of the input string.
 */
String input(String prompt, int buffer_size = 64){
  char input_s[buffer_size] = {'\0'};
  int i = -1;
  while(Serial.available() == 0);
  do{
    if(Serial.available()){
      i++;
      input_s[i] = Serial.read();
      Serial.print(input_s[i]);
    }
  }while(i < buffer_size - 1 && input_s[i] != '\n');
  if(input_s[i] != '\n'){
    Serial.println();
  }
  return String(input_s);
}

#ifdef DEBUG

    #define dprintf(msg) Serial.print(msg)
    #define dblink(pin) blink_led(pin)

#endif

#endif