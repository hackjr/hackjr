#include "controller.h"

#if 0
void button0_interrupt(void)
{
  int state;
  state = digitalRead(BUTTON0);
  Serial.print("/app/button/0/v ");
  if (LOW == state)
  {
    Serial.println(1);
  }
  else
  {
    Serial.println(0);    
  }
}

void button1_interrupt(void)
{
  int state;
  state = digitalRead(BUTTON1);
  Serial.print("/app/button/1/v ");
  if (LOW == state)
  {
    Serial.println(1);
  }
  else
  {
    Serial.println(0);    
  }
}
#endif

void handleAccel(int id)
{
  char tmp[32];
  if (0 != id)
  {
    Serial.println("Invalid Resource ID");
    return;
  }
  if (oper.equals("SET"))
  {
    sprintf(tmp,"/app/accel/%d/r",id);
    if (resource.equals(tmp))
    {
      reportAccel = parameters.toInt();
      Serial.println("OK");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("GET"))
  {
    sprintf(tmp,"/app/accel/%d/v",id);
    if (resource.equals(tmp))
    {
      printAccel(true, tmp);
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("EXECUTE"))
  {
    Serial.println("Invalid Operator");
    return;
  }
}

void handleButton(int id)
{
  char tmp[32];
  int button;
      
  switch(id)
  {
    case 0:
    {
      button=BUTTON0;
      break;
    } 
    case 1:
    {
      button=BUTTON1;
      break;
    } 
    case 2:
    {
      button=BUTTON2;
      break;
    } 
    case 3:
    {
      button=BUTTON3;
      break;
    }
    default:
    {
      Serial.println("Invalid Resource ID");
      return;
    }
  }
  if (oper.equals("SET"))
  {
    sprintf(tmp,"/app/button/%d/r",id);
    if (resource.equals(tmp))
    {
      reportButton = parameters.toInt();
      Serial.println("OK");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("GET"))
  {
    sprintf(tmp,"/app/button/%d/v",id);
    if (resource.equals(tmp))
    {
      int value;

      value=digitalRead(button);
      Serial.print(tmp);
      Serial.print(" ");
      Serial.print(value);
      Serial.println("");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("EXECUTE"))
  {
    Serial.println("Invalid Operator");
    return;
  }
}

void handleBuzzer(int id)
{
  char tmp[32];
  int buzzer;
  
  switch(id)
  {
    case 0:
    {
      buzzer=BUZZER0;
      break;
    } 
    default:
    {
      Serial.println("Invalid Resource ID");
      return;
    }
  } 
  if (oper.equals("SET"))
  {
    sprintf(tmp,"/app/buzzer/%d/v",id);
    if (resource.equals(tmp))
    {
      int durationIndex;
      int frequency, duration;
      
      durationIndex = parameters.indexOf(" ",1);
      frequency = parameters.toInt();
      duration  = parameters.substring(durationIndex).toInt();
      noTone(buzzer);
      
      if (frequency > 0)
      {
        if (duration > 0)
        {
          tone(buzzer, frequency, duration);
        }
        else
        {
          tone(buzzer, frequency);
        }
      }
      Serial.println("OK");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("GET"))
  {
#if 0
    sprintf(tmp,"/app/buzzer/%d/v",id);
    if (resource.equals(tmp))
    {
      int value;
      value=analogRead(buzzer);
      Serial.print(tmp);
      Serial.print(" ");
      Serial.print(value);
      Serial.println("");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
#else
    Serial.println("Invalid Operator");
    return;
#endif
  }
  else if (oper.equals("EXECUTE"))
  {
    Serial.println("Invalid Operator");
    return;
  }
}

void handleLed(int id)
{
  char tmp[32];
  int led;
  
  switch(id)
  {
    case 0:
    {
      led=LED0;
      break;
    } 
    case 1:
    {
      led=LED1;
      break;
    } 
    case 2:
    {
      led=LED2;
      break;
    } 
    case 3:
    {
      led=LED3;
      break;
    } 
    case 4:
    {
      led=LED4;
      break;
    }
    default:
    {
      Serial.println("Invalid Resource ID");
      return;
    }
  }  
  
  if (oper.equals("SET"))
  {
    sprintf(tmp,"/app/led/%d/v",id);
    if (resource.equals(tmp))
    {
      digitalWrite(led, parameters.toInt());
      Serial.println("OK");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("GET"))
  {
    sprintf(tmp,"/app/led/%d/v",id);
    if (resource.equals(tmp))
    {
      int value;
      value = digitalRead(led);
      Serial.print(tmp);
      Serial.print(" ");
      Serial.print(value);
      Serial.println("");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("EXECUTE"))
  {
    Serial.println("Invalid Operator");
    return;
  }
}

void handleLight(int id)
{
  char tmp[32];
  if (0 != id)
  {
    Serial.println("Invalid Resource ID");
    return;
  }
  if (oper.equals("SET"))
  {
    sprintf(tmp,"/app/light/%d/r",id);
    if (resource.equals(tmp))
    {
      reportLight = parameters.toInt();
      Serial.println("OK");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("GET"))
  {
    sprintf(tmp,"/app/light/%d/v",id);
    if (resource.equals(tmp))
    {
      printLight(true, tmp);
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("EXECUTE"))
  {
    Serial.println("Invalid Operator");
    return;
  }
}

void handlePot(int id)
{
  char tmp[32];
  if (0 != id)
  {
    Serial.println("Invalid Resource ID");
    return;
  }
  if (oper.equals("SET"))
  {
    sprintf(tmp,"/app/pot/%d/r",id);
    if (resource.equals(tmp))
    {
      reportPot = parameters.toInt();
      Serial.println("OK");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("GET"))
  {
    sprintf(tmp,"/app/pot/%d/v",id);
    if (resource.equals(tmp))
    {
      printPot(true, tmp);
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("EXECUTE"))
  {
    Serial.println("Invalid Operator");
    return;
  }
}

void handleRgb(int id)
{
  char tmp[32];
  if (0!=id)
  {
    Serial.println("Invalid Resource ID");
    return;
  }
  if (oper.equals("SET"))
  {
    sprintf(tmp,"/app/rgb/%d/v",id);
    if (resource.equals(tmp))
    {
      int greenIndex,blueIndex;
      greenIndex = parameters.indexOf(" ",1);
      blueIndex = parameters.indexOf(" ",1+greenIndex);
      analogWrite(RGB0_RED, 255 - parameters.toInt());
      analogWrite(RGB0_GREEN, 255 - parameters.substring(greenIndex).toInt());
      analogWrite(RGB0_BLUE, 255 - parameters.substring(blueIndex).toInt());
      Serial.println("OK");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("GET"))
  {
#if 0
    sprintf(tmp,"/app/rgb/%d/v",id);
    if (resource.equals(tmp))
    {
      int value;

      Serial.print(tmp);
      Serial.print(" ");
      value=analogRead(RGB0_RED);
      Serial.print(255-value);
      Serial.print(" ");
      value=analogRead(RGB0_GREEN);
      Serial.print(255-value);
      Serial.print(" ");
      value=analogRead(RGB0_BLUE);
      Serial.print(255-value);
      Serial.println("");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    }
#else
    Serial.println("Invalid Operator");
    return;
#endif
  }
  else if (oper.equals("EXECUTE"))
  {
    Serial.println("Invalid Operator");
    return;
  }
}

#if 0
void handleServo(int id)
{
  char tmp[32];
  if (0!=id)
  {
    Serial.println("Invalid Resource ID");
    return;
  }
  if (oper.equals("SET"))
  {
    sprintf(tmp,"/app/servo/%d/v",id);
    if (resource.equals(tmp))
    {
      tone(SERVO0, parameters.toInt());
      Serial.println("OK");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("GET"))
  {
#if 0
    sprintf(tmp,"/app/servo/%d/v",id);
    if (resource.equals(tmp))
    {
      int value;
      value=analogRead(SERVO0);
      Serial.print(tmp);
      Serial.print(" ");
      Serial.print(value);
      Serial.println("");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
#else
    Serial.println("Invalid Operator");
    return;
#endif
  }
  else if (oper.equals("EXECUTE"))
  {
    Serial.println("Invalid Operator");
    return;
  }
}
#endif

void handleSwitch(int id)
{
  char tmp[32];
  int s;
  
  switch(id)
  {
    case 0:
    {
      s=SWITCH0;
      break;
    } 
    case 1:
    {
      s=SWITCH1;
      break;
    }
    default:
    {
      Serial.println("Invalid Resource ID");
      return;
    }
  }
  if (oper.equals("SET"))
  {
    sprintf(tmp,"/app/switch/%d/r",id);
    if (resource.equals(tmp))
    {
      reportSwitch = parameters.toInt();
      Serial.println("OK");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("GET"))
  {
    sprintf(tmp,"/app/switch/%d/v",id);
    if (resource.equals(tmp))
    {
      int value;

      value=digitalRead(s);
      Serial.print(tmp);
      Serial.print(" ");
      Serial.print(value);
      Serial.println("");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("EXECUTE"))
  {
    Serial.println("Invalid Operator");
    return;
  }
}

#if 0
void handleTemp(int id)
{
  char tmp[32];
  if (0 != id)
  {
    Serial.println("Invalid Resource ID");
    return;
  }
  if (oper.equals("SET"))
  {
    sprintf(tmp,"/app/temp/%d/r",id);
    if (resource.equals(tmp))
    {
      reportTemp = parameters.toInt();
      Serial.println("OK");
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("GET"))
  {
    sprintf(tmp,"/app/temp/%d/v",id);
    if (resource.equals(tmp))
    {
      printTemp(true, tmp);
    }
    else
    {
      Serial.println("Invalid Resource");
      return;
    } 
  }
  else if (oper.equals("EXECUTE"))
  {
    Serial.println("Invalid Operator");
    return;
  }
}
#endif

void handleApp(void)
{
  int id,idIndex;
  
  idIndex = resource.indexOf("/",5)+1;
  id  = resource[idIndex] - '0';
  if ('/' != resource[idIndex+1])
  {
    Serial.println("Bad Resource");
    return;
  }
  else
  {

    if (oper.equals("EXECUTE"))
    {
      Serial.println("Invalid Operator for given resource");
      return; 
    }
      
    if (resource.startsWith("/app/accel/"))
    { 
      handleAccel(id);
    }
    else if (resource.startsWith("/app/button/"))
    {
      handleButton(id);
    }
    else if (resource.startsWith("/app/buzzer/"))
    { 
      handleBuzzer(id);
    }
    else if (resource.startsWith("/app/led/"))
    {
      handleLed(id);
    }
    else if (resource.startsWith("/app/light/"))
    {
      handleLight(id);
    }
    else if (resource.startsWith("/app/pot/"))
    {
      handlePot(id);
    }
    else if (resource.startsWith("/app/rgb/"))
    {
      handleRgb(id); 
    }
#if 0
    else if (resource.startsWith("/app/servo/"))
    {
      handleServo(id); 
    }
#endif
    else if (resource.startsWith("/app/switch/"))
    {
      handleSwitch(id);
    }
#if 0
    else if (resource.startsWith("/app/temp/"))
    { 
    }
#endif
    else
    {
      Serial.println("Bad Resource");
    }
  }
}

void handleArduino(void)
{
  int id,idIndex;
  
  idIndex = resource.indexOf("/",5)+1;
  id  = resource[idIndex] - '0';
  if ('/' != resource[idIndex+1])
  {
    Serial.println("Bad Resource");
    return;
  }
  else
  {
#if 0
    Serial.println("Arduino Resource Addressing NOT YET implemented");
    return;
#else
    if (oper.equals("EXECUTE"))
    {
      Serial.println("Invalid Operator for given resource");
      return; 
    }
      
    if (resource.startsWith("/arduino/a/"))
    { 
    }
    else if (resource.startsWith("/arduino/d/"))
    {
    }
    else if (resource.startsWith("/arduino/pwm/"))
    { 
    }
    else
    {
      Serial.println("Bad Resource");
    }
  }
#endif
}
