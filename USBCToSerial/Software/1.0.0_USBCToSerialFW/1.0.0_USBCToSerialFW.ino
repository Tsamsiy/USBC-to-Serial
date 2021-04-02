/*
  ATTinyCore[Spence Konde] Settings:

  Board:        ATTiny441/841 (Optiboot)
  Chip:         841
  Clock         8MHz internal (Vcc < 4,5V)
  BOD Level:    2,7V
  BOD active:   disabled
  BOD sleep:    disabled
  Pin Mapping:  clockwise
  LTO:          enabled
  Bootloader:   UART0
  Wire Modes:   Master only
  tinyNeoPixel: Port A
  millis():     enabled
*/

#include <Wire.h>
#include <SparkFun_STUSB4500.h>
#include "PinDefs.h"

#define DEBUG

STUSB4500 PD;

String ATc;
uint8_t commID = 0;
int8_t mode = 0;
#include "ATCommands.h"

uint8_t PDONum = 1;  //preset to "5V fixed"
uint8_t PDVGroup = 2;  //preset to "custom"
bool ATMode = true;

void setup()//#######################################################################################################################
{
  #ifdef DEBUG
    Serial.begin(9600);
    Serial.println(F("Init..."));
  #endif

  //init OI pins  //-------------------------------------------------------------------------------------------------
  pinMode(StatLED, OUTPUT);
  digitalWrite(StatLED, LOW);
  
  pinMode(PDRST, OUTPUT);
  digitalWrite(PDRST, LOW);
  
  pinMode(PDV0, OUTPUT);
  digitalWrite(PDV0, LOW);

  pinMode(PDV1, OUTPUT);
  digitalWrite(PDV1, LOW);
  
  pinMode(PDIRQ, INPUT_PULLUP);
  
  pinMode(ModeSW, INPUT_PULLUP);

  //init connection to PD controller  //-------------------------------------------------------------------------------------------------
  Wire.begin(); //Join I2C bus
  
  if(!PD.begin())
  {
    #ifdef DEBUG
      Serial.println(F("STUSB4500 ERROR"));
    #endif
    while(1)//LED status error
    {
      digitalWrite(StatLED, HIGH);
      delay(200);
      digitalWrite(StatLED, LOW);
      delay(200);
      digitalWrite(StatLED, HIGH);
      delay(200);
      digitalWrite(StatLED, LOW);
      delay(200);
    }
  }
  //read info from PD controller
  UpdatePDStatus();
  PD.setUsbCommCapable(true);

  #ifdef DEBUG
    Serial.println(F("Ready"));
  #endif
}

void loop()//#######################################################################################################################
{
//  AT command control  //-------------------------------------------------------------------------------------------------------------------------------
  while(ATMode == true)
  { 
    if(Serial.available() != 0)
    {
      char temp = Serial.read();
      if((temp == '\n') || (temp == '\r'))
      {
        //empty rest of buffer
        delay(10);
        while(Serial.available() != 0)
        {
          Serial.read();
        }
        
        commID = ParseATCommand(ATc);
        switch(commID)
        {
          case 0://HELP
            //print all command words
            for(uint8_t i = 0; i < ATCommandsNum; i++)
            {
              //read string back from PROGMEM
              for(uint8_t j = 0; j < ATCommandsDim; j++)
              {
                char temp = (char)pgm_read_byte_near(ATCommands[i] + j);
                if(temp == ATCommandEndChar)
                {
                  break;
                }
                Serial.print(temp);
              }
              Serial.println();
            }
          break;
  
          case 1:
            //PD.write();
            Serial.println(F("Saved!"));
          break;
  
          case 2:
            Serial.println(F("Exiting AT-Mode"));
            Serial.end();
            ATMode = false;
          break;
  
          case 3:
            Serial.print(F("Command: "));
            Serial.println(commID);
            Serial.print(F("Mode: "));
            Serial.println(ParseCommandMode(ATc));
            for(uint8_t i = 0; i < (uint8_t)pgm_read_byte_near(ATCommandsNumParams + commID); i++)
            {
              Serial.print(F("Parameter "));
              Serial.print(i);
              Serial.print(F(": "));
              Serial.println(ParseCommandParams(ATc, commID, i));
            }
          break;
  
          case 4:
            
          break;
  
          case 5:
            
          break;
  
          case 6:
            
          break;
  
          case 7:
            
          break;
  
          default:
            //Serial.println(F("Try: AT+HELP"));
          break;
        }
        ATc = "";
      }
      else
      {
        ATc += temp;
      }
    }
  }

//  push button control  //-------------------------------------------------------------------------------------------------------------------------------
  
}



void UpdatePDStatus()//#######################################################################################################################
{
  PD.read();
  PDONum = PD.getPdoNumber();
  if((PD.getVoltage(2) == 9.0) && (PD.getVoltage(3) == 12.0))
  {
    PDVGroup = 0;
  }
  else if((PD.getVoltage(2) == 15.0) && (PD.getVoltage(3) == 20.0))
  {
    PDVGroup = 1;
  }
  else
  {
    PDVGroup = 2;
  }
  if(PDONum == 1)
  {
    digitalWrite(PDV0, LOW);
    digitalWrite(PDV1, LOW);
  }
  else
  {
    if(PDVGroup == 0)
    {
      digitalWrite(PDV0, HIGH);
      digitalWrite(PDV1, LOW);
    }
    else if(PDVGroup == 1)
    {
      digitalWrite(PDV0, LOW);
      digitalWrite(PDV1, HIGH);
    }
    else
    {
      digitalWrite(PDV0, LOW);
      digitalWrite(PDV1, LOW);
    }
  }
}

void SetPDVGroup(uint8_t group)//#######################################################################################################################
{
  PDVGroup = group;
  if(group == 0)
    {
      PD.setVoltage(2,9.0);
      PD.setLowerVoltageLimit(2,20);
      PD.setUpperVoltageLimit(2,20);

      PD.setVoltage(3,12.0);
      PD.setLowerVoltageLimit(3,20);
      PD.setUpperVoltageLimit(3,20);
    }
    else if(group == 1)
    {
      PD.setVoltage(2,15.0);
      PD.setLowerVoltageLimit(2,20);
      PD.setUpperVoltageLimit(2,20);

      PD.setVoltage(3,20.0);
      PD.setLowerVoltageLimit(3,20);
      PD.setUpperVoltageLimit(3,20);
    }
}

void StatLEDMode(uint8_t PDO, uint8_t PDVG)//#######################################################################################################################
{
  /*/repeated parallel display
  for(uint8_t i = 0; i < PDO; i++)
  {
    for(uint8_t j = 0; j < (PDVG + 1); j++)
    {
      digitalWrite(StatLED, HIGH);
      delay(100);
      digitalWrite(StatLED, LOW);
      delay(100);
    }
    delay(500);
  }*/

  //series display
  for(uint8_t i = 0; i < PDO; i++)
  {
    digitalWrite(StatLED, HIGH);
    delay(100);
    digitalWrite(StatLED, LOW);
    delay(100);
  }
  delay(500);
  for(uint8_t j = 0; j < (PDVG + 1); j++)
  {
    digitalWrite(StatLED, HIGH);
    delay(100);
    digitalWrite(StatLED, LOW);
    delay(100);
  }
}
