
#define DEBUG

String ATc;
uint8_t commID = 0;
int8_t mode = 0;
#include "ATCommands.h"

void setup()//#######################################################################################################################
{
  Serial.begin(9600);

  Serial.println(F("Ready"));
}

void loop()//#######################################################################################################################
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
          Serial.println(F("Saved!"));
        break;

        case 2:
          Serial.println(F("Exiting AT-Mode"));
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

        case 5:
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

        case 6:
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

        case 7:
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
}
