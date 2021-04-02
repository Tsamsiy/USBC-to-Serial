#ifndef ATCommands_h
  #define ATCommands_h
  #include <avr/pgmspace.h>
  #include <Arduino.h>  //needed to use Serial here

  //const unsigned char ATCommandVarChar = '$';
  const char ATCommandEndChar = '%';
  const char ATCommandSetChar = '=';
  const char ATCommandGetChar = '?';
  const char ATParamsDelimiterChar = ',';

  const uint8_t ATCommandsNum = 9;
  const uint8_t ATCommandsDim = 6;
  const char ATCommands[ATCommandsNum][ATCommandsDim] PROGMEM =
  {
    "HELP%%",  //prints all Commands
    "SAVE%%",  //writes changend settings to PD controller
    "EXIT%%",  //exits AT mode
    "PDO%%%",  //PDO priority
    "PDOV%%",  //voltage of a PDO
    "PDOCL%",  //current limit of a PDO
    "PDOVLU",  //upper voltage limit of a PDO
    "PDOVLL",  //lower voltage limit of a PDO
    "USBCOM"  //USB data communication capable
  };
  //max number of expected parameters for both modes
  const uint8_t ATCommandsNumParams[ATCommandsNum] PROGMEM =
  {
    0,  //prints all Commands
    0,  //writes changend settings to PD controller
    0,  //exits AT mode
    1,  //PDO priority
    2,  //voltage of a PDO
    2,  //current limit of a PDO
    2,  //upper voltage limit of a PDO
    2,  //lower voltage limit of a PDO
    1  //USB data communication capable
  };



  int8_t ParseATCommand(String str)
  {
    //Command structure:
    //AT
    //AT+COMMAND?PARAMETER,PARAMETER
    //AT+COMMAND=PARAMETER,PARAMETER

    String comm;
    int8_t mode = -1;
    uint8_t index = 0;
    
    Serial.println(str);
    //Serial.print(": ");
    //Serial.println(str.length());
    //Serial.println((char)str.charAt(2), HEX);

    if(str.length() < 2)
    {
      return -1;
    }

    //check vor command headder
    if(str.substring(0, 2) == "AT")
    {
      if(str.length() == 2)
      {
        Serial.println(F("OK"));
        return -1;
      }

      //command follows headder
      if((str.charAt(2) == '+') && (str.length() >= 5))
      {
        //Serial.println(str.indexOf(ATCommandGetChar, 4));
        //Serial.println(str.indexOf(ATCommandSetChar, 4));
        
        //get command
        if(str.indexOf(ATCommandGetChar, 4) != -1)
        {
          index = str.indexOf(ATCommandGetChar, 4);
          mode = 0;
        }
        //set command
        if(str.indexOf(ATCommandSetChar, 4) != -1)
        {
          index = str.indexOf(ATCommandSetChar, 4);
          mode = 1;
        }

        //Serial.print("mode: ");
        //Serial.println(mode);
        //command does not contain operator
        if(mode == -1)
        {
          //return -1;
          comm = str.substring(3, str.length());
        }
        else
        {
          comm = str.substring(3, index);
        }
        
        //parse command word
        for(uint8_t i = 0; i < ATCommandsNum; i++)
        {
          String compare = "";
          //read string back from PROGMEM
          for(uint8_t j = 0; j < ATCommandsDim; j++)
          {
            compare += ((char)pgm_read_byte_near(ATCommands[i] + j));
          }
          //Serial.println(compare);
          if(comm == compare.substring(0, compare.indexOf(ATCommandEndChar)))
          {
            return i;
          }
        }
        //Serial.print("commID: ");
        //Serial.println(commID);
      }
    }
    return -1;
  }

  String ParseCommandParams(String str, int8_t comm, uint8_t paramIndex)
  {
    String params;
    uint8_t delimIndex = 0;
    
    if(comm < 0)
    {
      return "";
    }

    //length of command word string
    for(uint8_t j = 0; j < ATCommandsDim; j++)
    {
      if((char)pgm_read_byte_near(ATCommands[comm] + j) == ATCommandEndChar)
      {
        //extract parameter string
        params = str.substring((3 + j + 1), str.length());
        break;
      }
    }
    //Serial.println(params);

    //check if more params were asked than exist
    if((params.indexOf(ATParamsDelimiterChar) == -1) && (paramIndex > 0))
    {
      return "";
    }
    else if((params.indexOf(ATParamsDelimiterChar) != -1) && (paramIndex == 0))
    {
      return params.substring(0, params.indexOf(ATParamsDelimiterChar));
    }
    else if((params.indexOf(ATParamsDelimiterChar) == -1) && (paramIndex == 0))
    {
      return params;
    }

    for(uint8_t i = 0; i < paramIndex; i++)
    {
      if(params.indexOf(ATParamsDelimiterChar, (delimIndex + 1)) != -1)
      {
        delimIndex = params.indexOf(ATParamsDelimiterChar, (delimIndex + 1));

        //Serial.print(F("delimIndex: "));
        //Serial.println(delimIndex);
      }
      else
      {
        return "";
      }
    }
    
    if(params.indexOf(ATParamsDelimiterChar, (delimIndex + 1)) != -1)
    {
      return params.substring((delimIndex + 1), params.indexOf(ATParamsDelimiterChar, (delimIndex + 1)));
    }
    else
    {
      return params.substring((delimIndex + 1), params.length());
    }
  }

  int8_t ParseCommandMode(String str)
  {
    if(str.indexOf(ATCommandGetChar, 4) != -1)
    {
      return 0;
    }
    //set command
    if(str.indexOf(ATCommandSetChar, 4) != -1)
    {
      return 1;
    }
    //no mode detected
    return -1;
  }


#endif
