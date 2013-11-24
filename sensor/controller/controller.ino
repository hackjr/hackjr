#include "pitches.h"

#define MAX(a,b) ((a>b)?a:b) 
#define MIN(a,b) ((a<b)?a:b)
 
// Digital Output
int RGB0_RED   = 13; //Red PWM
int RGB0_GREEN = 11; //Green PWM
int RGB0_BLUE  = 10; //Blue PWM

int LED0      = 9; //Green PWM
int LED1      = 12;
int LED2      = 1;
int LED3      = 6; // PWM
int LED4      = 5; // PWM

int BUZZER0 = 0; // PWM

// Digital Input
int BUTTON0   = 3; //Interrupt capable
int BUTTON1   = 2; //Interrupt capable
int BUTTON2   = A4; //18; //Analog A4 is being stolen for digital usage
int BUTTON3   = 4;

int SWITCH0   = 7;
int SWITCH1   = 8;

/* Analog In */
int PHOTO_RESISTOR0 = A5;
int POTENTIOMETER0 = A0;
int ACCEL0_X        = A1;
int ACCEL0_Y        = A2;
int ACCEL0_Z        = A3;

String oper            = "";
String resource        = "";
String parameters      = "";
String inputString     = "";         // a string to hold incoming data
volatile boolean stringComplete = false;  // whether the string is complete

volatile boolean reportAccel  = true;
volatile boolean reportButton = true;
volatile boolean reportLight  = true;
volatile boolean reportSwitch = true;
volatile boolean reportPot    = true;
volatile boolean reportTemp   = false;

volatile boolean button0Value = false;
volatile boolean button1Value = false;
volatile boolean button2Value = false;
volatile boolean button3Value = false;

volatile int     light0Value;
volatile int     switch0Value;
volatile int     switch1Value;
volatile int     pot0Value;

volatile int     accel_x;
volatile int     accel_y;
volatile int     accel_z;

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

void printAccel(boolean force, char *tmp)
{
  int x,y,z;
 
  x = (analogRead(ACCEL0_X)/4)-128;
  y = (analogRead(ACCEL0_Y)/4)-128;
  z = (analogRead(ACCEL0_Z)/4)-128;
  if ((true == force)
      || ((MAX(accel_x,x) - MIN(accel_x,x)) > 8)
      || ((MAX(accel_y,y) - MIN(accel_y,y)) > 8)
      || ((MAX(accel_z,z) - MIN(accel_z,z)) > 8)
     )
  {
    Serial.print(tmp);
    Serial.print(" ");
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.print(",");
    Serial.print(z);
    Serial.println("");
    accel_x = x;
    accel_y = y;
    accel_z = z;
  }
}

void printLight(boolean force, char *tmp)
{
  int v;
  v = (analogRead(PHOTO_RESISTOR0)/4);
  if ((true == force)
      || ((light0Value-v)<-4) 
      || ((light0Value-v)> 4)
     )
  {
     Serial.print("/app/light/0/v ");
     Serial.println(v);
     light0Value = v;
  }
}

void printPot(boolean force, char *tmp)
{
  int v;
  v = (analogRead(POTENTIOMETER0)/4)-128;
  if ((true == force)
      || ((pot0Value-v)< 1) 
      || ((pot0Value-v)> 1)
     )
  {
     Serial.print("/app/pot/0/v ");
     Serial.println(v);
     pot0Value = v;
  }
}

void printTemp(boolean force, char *tmp)
{
#if 0
  int v;
  v = (analogRead(TEMP0)/4)-128;
  if ((true == force)
      || ((light0Value-v)<-4) 
      || ((light0Value-v)> 4)
     )
  {
     Serial.print("/app/temp/0/v ");
     Serial.println(v);
     temp0Value = v;
  }
#endif
}

// the setup routine runs once when you press reset:
void setup()
{              
  // Initialize the digital output pins
  pinMode(RGB0_RED, OUTPUT);
  pinMode(RGB0_GREEN, OUTPUT);
  pinMode(RGB0_BLUE, OUTPUT);
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  // Initialize the digital input pins  
  pinMode(BUTTON0, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(SWITCH0, INPUT_PULLUP);
  pinMode(SWITCH1, INPUT_PULLUP);
  
  // Initialize the digital output pins as low
  analogWrite(RGB0_RED, 255);    
  analogWrite(RGB0_GREEN, 255);    
  analogWrite(RGB0_BLUE, 255);
  digitalWrite(LED0, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  resource.reserve(32);
  oper.reserve(16);
  parameters.reserve(128);
  oper            = "";
  resource        = "";
  parameters      = "";
  inputString     = ""; 
  
  button0Value = digitalRead(BUTTON2);
  button1Value = digitalRead(BUTTON3);
  button2Value = digitalRead(BUTTON2);
  button3Value = digitalRead(BUTTON3);
  
  switch0Value = digitalRead(SWITCH0);
  switch1Value = digitalRead(SWITCH1);

  light0Value  = (analogRead(PHOTO_RESISTOR0)/4);
  
  pot0Value    = (analogRead(POTENTIOMETER0)/4)-128;
  
  accel_x = (analogRead(ACCEL0_X)/4)-128;
  accel_y = (analogRead(ACCEL0_Y)/4)-128;
  accel_z = (analogRead(ACCEL0_Z)/4)-128;

//  attachInterrupt(0, button0_interrupt, CHANGE);
//  attachInterrupt(1, button1_interrupt, CHANGE);
}

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
}

// the loop routine runs over and over again forever:
void loop() 
{
#ifndef serialEvent_implemented
  if (Serial.available()) serialEvent();
#endif

  if(reportAccel)
  {
    printAccel(false, "/app/accel/0/v");
  }
  
  if(reportButton)
  {
     int tmp;
     tmp = digitalRead(BUTTON0);
     if (tmp != button0Value)
     {
       Serial.println(inputString);
       Serial.print("/app/button/0/v ");
       Serial.println(tmp);
       button0Value = tmp;
     }
     tmp = digitalRead(BUTTON1);
     if (tmp != button1Value)
     {
       Serial.println(inputString);
       Serial.print("/app/button/1/v ");
       Serial.println(tmp);
       button1Value = tmp;
     }
     tmp = digitalRead(BUTTON2);
     if (tmp != button2Value)
     {
       Serial.println(inputString);
       Serial.print("/app/button/2/v ");
       Serial.println(tmp);
       button2Value = tmp;
     }
     tmp = digitalRead(BUTTON3);
     if (tmp != button3Value)
     {
       Serial.print("/app/button/3/v ");
       Serial.println(tmp);
       button3Value = tmp;
     }
  }
  if (reportLight)
  {
    printLight(false, "/app/light/0/v");
  }
  if (reportPot)
  {
    printPot(false, "/app/pot/0/v");
  }
  if (reportSwitch)
  {
     int tmp;
     tmp = digitalRead(SWITCH0);
     if (tmp != switch0Value)
     {
       Serial.print("/app/switch/0/v ");
       Serial.println(tmp);
       switch0Value = tmp;
     }
     tmp = digitalRead(SWITCH1);
     if (tmp != switch1Value)
     {
       Serial.print("/app/switch/1/v ");
       Serial.println(tmp);
       switch1Value = tmp;
     }
  }
  if (reportTemp)
  {
    printTemp(false, "/app/temp/0/v");
  }
  // print the string when a newline arrives:
  if (stringComplete)
  { 
    if (resource.startsWith("/app/"))
    {
      handleApp(); 
    }
    else if (resource.startsWith("/arduino/"))
    {
      handleArduino();
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();

    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      int resourceIndex = 0;
      int parameterIndex;
      if (inputString.startsWith("SET "))
      {
        oper          = "SET";    
        resourceIndex = 4;
      }
      else if (inputString.startsWith("GET "))
      {
        oper          = "GET";
        resourceIndex = 4;
      }
      else if (inputString.startsWith("EXECUTE "))
      {
        resourceIndex = 8;
        oper          = "EXECUTE";
      }
      else
      {
        Serial.println("Bad Operator");
        oper        = "";
        inputString = "";
        return;
      }
      parameterIndex = inputString.indexOf(" ",resourceIndex);
      if (parameterIndex > 0)
      {
        resource   = inputString.substring(resourceIndex,parameterIndex);
        parameters = inputString.substring(parameterIndex);
      }
      else
      {
        resource   = inputString.substring(resourceIndex);
        parameters = "";
      }
      resource.toLowerCase();
      stringComplete = true;
    }
    else
    {
      // add it to the inputString:
      inputString += inChar; 
    }
  }
}
