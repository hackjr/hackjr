#include <Servo.h>

#include "pitches.h"
#include "controller.h"

#define MAX(a,b) ((a>b)?a:b) 
#define MIN(a,b) ((a<b)?a:b)
 
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

#ifdef USE_BUTTON0
volatile boolean button0Value = false;
#endif
volatile boolean button1Value = false;
volatile boolean button2Value = false;
volatile boolean button3Value = false;

volatile int     light0Value  = 0;
volatile int     switch0Value = 0;
volatile int     switch1Value = 0;
volatile int     pot0Value    = 0;

volatile int     accel_x      = 0;
volatile int     accel_y      = 0;
volatile int     accel_z      = 0;

#ifdef USE_SERVO0
volatile int    servo0Value   = 0;
Servo servo0;
#endif

void printAccel(boolean force, char *tmp)
{
  int x,y,z;
 
  x = (analogRead(ACCEL0_X)/4)-128;
  y = (analogRead(ACCEL0_Y)/4)-128;
  z = (analogRead(ACCEL0_Z)/4)-128;
  if ((true == force)
      || ((MAX(accel_x,x) - MIN(accel_x,x)) > ACCEL_FILTER)
      || ((MAX(accel_y,y) - MIN(accel_y,y)) > ACCEL_FILTER)
      || ((MAX(accel_z,z) - MIN(accel_z,z)) > ACCEL_FILTER)
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
      || ((MAX(light0Value,v) - MIN(light0Value,v)) > LIGHT_FILTER)
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
      || ((MAX(pot0Value,v) - MIN(pot0Value,v)) > POT_FILTER)
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
      || ((MAX(temp0Value,v) - MIN(temp0Value,v)) > TEMP_FILTER)
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
//  pinMode(BUTTON0, INPUT_PULLUP);
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

#ifdef USE_SERVO0
  servo0.attach(SERVO0);
  servo0.write(0);
#endif
  
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
  
#ifdef USE_BUTTON0
  button0Value = digitalRead(BUTTON0);
#endif
  button1Value = digitalRead(BUTTON1);
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

#ifdef USE_SERVO0
  servo0.write(servo0Value);
#endif
  
  if(reportButton)
  {
     int tmp;
#ifdef USE_BUTTON0
     tmp = digitalRead(BUTTON0);
     if (tmp != button0Value)
     {
       Serial.println(inputString);
       Serial.print("/app/button/0/v ");
       Serial.println(tmp);
       button0Value = tmp;
     }
#endif
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
