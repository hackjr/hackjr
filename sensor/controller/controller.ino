/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  ReadAnalogVoltage
  Reads an analog input on pin 0, converts it to voltage, and prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.
 
  This example code is in the public domain.
*/
 
// Digital Output
int RGB_RED   = 13; //Red PWM
int RGB_GREEN = 11; //Green PWM
int RGB_BLUE  = 10; //Blue PWM

int LED0      = 9; //Green PWM
int LED1      = 12;
int LED2      = 1;
int LED3      = 6; // PWM
int LED4      = 5; // PWM

int BUZZER = 0; // PWM

// Digital Input
int BUTTON0   = 3; //Interrupt capable
int BUTTON1   = 2; //Interrupt capable
int BUTTON2   = A4; //18; //Analog A4 is being stolen for digital usage
int BUTTON3   = 4;

int SWITCH0   = 7;
int SWITCH1   = 8;

/* Analog In */
int PHOTO_RESISTOR = A5;
int PONTENTIOMETER = A0;
int ACCEL_X        = A1;
int ACCEL_Y        = A2;
int ACCEL_Z        = A3;

String oper            = "";
String resource        = "";
String parameters      = "";
String inputString     = "";         // a string to hold incoming data
volatile boolean stringComplete = false;  // whether the string is complete

boolean reportAccel  = false;
boolean reportButton = true;
boolean reportLight  = true;
boolean reportSwitch = true;
boolean reportTemp   = false;

boolean button2Value = false;
boolean button3Value = false;

int     light0Value;
int     switch0Value;
int     switch1Value;

int     accel_x;
int     accel_y;
int     accel_z;

void button0_interrupt(void)
{
  int state;
  state = digitalRead(BUTTON0);
  Serial.print("/app/button/0 ");
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
  Serial.print("/app/button/1 ");
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
 
  x = (analogRead(ACCEL_X)/4)-128;
  y = (analogRead(ACCEL_Y)/4)-128;
  z = (analogRead(ACCEL_Z)/4)-128;
  if ((true == force)
      || (((accel_x - x) < -8) || ((accel_x -x) > 8))
      || (((accel_y - y) < -8) || ((accel_y -y) > 8))
      || (((accel_z - z) < -8) || ((accel_z -z) > 8))
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
    accel_x = z;
  }
}

void printLight(boolean force, char *tmp)
{
  int v;
  v = (analogRead(PHOTO_RESISTOR)/4)-128;
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


// the setup routine runs once when you press reset:
void setup()
{              
  // Initialize the digital output pins
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
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
  digitalWrite(RGB_RED, LOW);    
  digitalWrite(RGB_GREEN, LOW);    
  digitalWrite(RGB_BLUE, LOW);
  digitalWrite(LED0, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  resource.reserve(32);
  oper.reserve(16);
  parameters.reserve(128);
  oper            = "";
  resource        = "";
  parameters      = "";
  inputString     = ""; 
  
  attachInterrupt(0, button0_interrupt, CHANGE);
  attachInterrupt(1, button1_interrupt, CHANGE);
  button2Value = digitalRead(BUTTON2);
  button3Value = digitalRead(BUTTON3);
  switch0Value = digitalRead(SWITCH0);
  switch1Value = digitalRead(SWITCH1);
  light0Value  = (analogRead(PHOTO_RESISTOR)/4)-128;
  
  accel_x = (analogRead(ACCEL_X)/4)-128;
  accel_y = (analogRead(ACCEL_Y)/4)-128;
  accel_z = (analogRead(ACCEL_Z)/4)-128;
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
      Serial.println("Invalid Operator");
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
  if ((4<=id) || (0>id))
  {
    Serial.println("Invalid Resource ID");
    return;
  }
  if (oper.equals("SET"))
  {
    sprintf(tmp,"/app/button/%d/r",id);
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
    sprintf(tmp,"/app/button/%d/v",id);
    if (resource.equals(tmp))
    {
      int value;
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
      }
      value=digitalRead(button);
      Serial.print(tmp);
      Serial.print(" ");
      Serial.print(value);
      Serial.println("");
    }
    else
    {
      Serial.println("Invalid Operator");
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
  if (0!=id)
  {
    Serial.println("Invalid Resource ID");
    return;
  }
  if (oper.equals("SET"))
  {
    sprintf(tmp,"/app/buzzer/%d/v",id);
    if (resource.equals(tmp))
    {
      analogWrite(BUZZER0, parameters.toInt());
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
    sprintf(tmp,"/app/buzzer/%d/v",id);
    if (resource.equals(tmp))
    {
      int value
      value=analogRead(BUZZER0);
      Serial.print(tmp);
      Serial.print(" ");
      Serial.print(value);
      Serial.println("");
    }
    else
    {
      Serial.println("Invalid Operator");
      return;
    } 
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
  if (0!=id)
  {
    Serial.println("Invalid Resource ID");
    return;
  }
  if (oper.equals("SET"))
  {
    sprintf(tmp,"/app/led/%d/v",id);
    if (resource.equals(tmp))
    {
      analogWrite(BUZZER0, parameters.toInt());
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
      int value
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
      }
      value=digitalWrite(led,paramaters.toInt());
      Serial.println("OK");
    }
    else
    {
      Serial.println("Invalid Operator");
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
      Serial.println("Invalid Operator");
      return;
    } 
  }
  else if (oper.equals("EXECUTE"))
  {
    Serial.println("Invalid Operator");
    return;
  }
}


void handleApp(void)
{
  int id;
  id  = resource[5] - '0';
  if ('/' != resource[6])
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
    else if (resource.startsWith("/app/rgb/"))
    { 
    }
    else if (resource.startsWith("/app/servo/"))
    { 
    }
    else if (resource.startsWith("/app/switch/"))
    { 
    }
    else if (resource.startsWith("/app/temp/"))
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
  if(reportAccel)
  {
    printAccel(false, "/app/accel/0/v");
  }
  
  if(reportButton)
  {
     int tmp;
     tmp = digitalRead(BUTTON2);
     if (tmp != button2Value)
     {
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
      if (resource.startsWith("a/"))
      { 
      }
      else if (resource.startsWith("d/"))
      { 
      }
      else if (resource.startsWith("pwm/"))
      { 
      }
      else
      {
      }
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
