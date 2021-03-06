#ifndef PinDefs_h
  #define PinDefs_h
  //see T3001 project files for details

  /*/UART0
  #define SerialTX PA1
  #define SerialRX PA2
  #define SerialDTR PB3   //RST
  */

  /*/I2C
  #define I2CSCL PA4
  #define I2CSDA PA6
  */

  //PD controller
  #define PDRST 7//PA7
  #define PDIRQ 9//PB1
  //#define PDO2
  //#define PDO3
  #define PDV0 10//PB0    //9V, 12V
  #define PDV1 8//PB2    //15V, 20V

  #define StatLED 0//PA0
  #define ModeSW 3//PA3
#endif
