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

//#define DEBUG

STUSB4500 PD;

String ATc;
uint8_t commID = 0;
int8_t mode = 0;
#include "ATCommands.h"

uint8_t PDONum = 1;  //preset to "5V fixed"
uint8_t PDVGroup = 2;  //preset to "custom"
bool ATMode = false;

unsigned long pressedTime = 0;

void setup()//#######################################################################################################################
{
  #ifdef DEBUG
    Serial.begin(9600);
    Serial.println(F("Init..."));
  #endif
  #ifndef DEBUG
    //make sure the Serial port is passive
    Serial.end();
    //pinMode(SerialTX, INPUT);
    //pinMode(SerialRX, INPUT);
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
  //delay(50);
  Wire.begin(); //Join I2C bus
  
  if(!PD.begin())
  {
    #ifdef DEBUG
      Serial.println(F("STUSB4500 ERROR"));
    #endif
    while(1)//LED status error
    {
      blink(200, false);
    }
  }
  
  //SetPDVGroup(1);
  //PD.setPdoNumber(2);
  //PD.setUsbCommCapable(true);
  //PD.write();

  //read info from PD controller
  UpdatePDStatus();

  #ifdef DEBUG
    Serial.println(F("Ready"));
  #endif
  blink(200, true);
}

void loop()//#######################################################################################################################
{
//  AT command control  //-------------------------------------------------------------------------------------------------------------------------------
  /*while(ATMode == true)
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
            Serial.println(F("Try: AT+HELP"));
          break;
        }
        ATc = "";
      }
      else
      {
        ATc += temp;
      }
    }
  }*/

//  push button control  //-------------------------------------------------------------------------------------------------------------------------------
  //debouncing is handled by delays
  //polling is adequate, since no other time sensitive tasks need to be performed
  if(digitalRead(ModeSW) == LOW)
  {
    //get press time of mode button
    pressedTime = millis();
    delay(10);
    while(digitalRead(ModeSW) == LOW)
    {
      /*if(((millis() - pressedTime) % 500) == 0)
      {
        blink(100, true);
      }*/

      if(((millis() - pressedTime) > 200) && ((millis() - pressedTime) < 300))
      {
        blink(50, true);
      }
      if(((millis() - pressedTime) > 1000) && ((millis() - pressedTime) < 1100))
      {
        blink(50, true);
      }
      if(((millis() - pressedTime) > 2000) && ((millis() - pressedTime) < 2100))
      {
        blink(50, true);
      }
    }
    pressedTime = millis() - pressedTime;
    delay(1500);

    //shortpress
    if((pressedTime > 200) && (pressedTime < 1000))
    {
      PDONum++;
      if(PDONum > 3)
      {
        PDONum = 1;
      }

      PD.setPdoNumber(PDONum);
    }
    //longpress
    if((pressedTime > 1000) && (pressedTime < 2000))
    {
      //if voltage group is on custom, set to 0
      if(PDVGroup == 2)
      {
        PDVGroup = 0;
      }
      else
      {
        PDVGroup++;
        if(PDVGroup > 1)
        {
          PDVGroup = 0;
        }
      }
      SetPDVGroup(PDVGroup);
    }
    //long-longpress
    if(pressedTime > 2000)
    {
      ATMode = true;
      blink(50, false);
      blink(50, false);
      blink(50, true);
    }
    else
    {
      //display that changes are made
      StatLEDMode(PDONum,PDVGroup);
  
      //set changes
      PD.write();
      
      //reset PD controller
      //reset will trigger re-negotiation od PD contract
      digitalWrite(PDRST, HIGH);
      delay(10);
      digitalWrite(PDRST, LOW);
      delay(50);
  
      //read back parameters
      UpdatePDStatus();
    }
  }
}



void blink(uint16_t del, bool cutOff)
{
  digitalWrite(StatLED, HIGH);
  delay(del);
  digitalWrite(StatLED, LOW);
  if(cutOff != 1)
  {
    delay(del);
  }
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
    //group 0
    if(PDVGroup == 0)
    {
      digitalWrite(PDV0, HIGH);
      digitalWrite(PDV1, LOW);
    }
    //group 1
    else if(PDVGroup == 1)
    {
      digitalWrite(PDV0, LOW);
      digitalWrite(PDV1, HIGH);
    }
    //custom 
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
      //PD.setLowerVoltageLimit(2,20);
      //PD.setUpperVoltageLimit(2,20);

      PD.setVoltage(3,12.0);
      //PD.setLowerVoltageLimit(3,20);
      //PD.setUpperVoltageLimit(3,20);
    }
    else if(group == 1)
    {
      PD.setVoltage(2,15.0);
      //PD.setLowerVoltageLimit(2,20);
      //PD.setUpperVoltageLimit(2,20);

      PD.setVoltage(3,20.0);
      //PD.setLowerVoltageLimit(3,20);
      //PD.setUpperVoltageLimit(3,20);
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
    blink(100, false);
  }
  delay(500);
  for(uint8_t j = 0; j < (PDVG + 1); j++)
  {
    blink(100, false);
  }
}
