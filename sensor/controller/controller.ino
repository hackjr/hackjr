/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  ReadAnalogVoltage
  Reads an analog input on pin 0, converts it to voltage, and prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

String die = String("Die");
String live = String("Live");
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

boolean alive = true;

// the setup routine runs once when you press reset:
void setup()
{              

  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  if (alive)
  {
    digitalWrite(led, HIGH);    
  }
  else
  {
    digitalWrite(led, LOW);
  }
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

// the loop routine runs over and over again forever:
void loop() 
{
  if(alive)
  {
    int x,y,z;
    float xV,yV,zV;
 
    x = analogRead(A0);
    y = analogRead(A1);
    z = analogRead(A2);
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 3.3V):
    xV = x * (3.3 /1023);
    yV = y * (3.3 /1023);
    zV = z * (3.3 /1023);
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.print(",");
    Serial.print(z);
    Serial.println("");
    delay(50);
  }
  // print the string when a newline arrives:
  if (stringComplete)
  {  
    if (inputString.equalsIgnoreCase(die))
    {
      alive = false;
      digitalWrite(led, LOW); 
    }
    if (inputString.equalsIgnoreCase(live))
    {
      alive = true;
      digitalWrite(led, HIGH); 
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
      stringComplete = true;
    }
    else
    {
      // add it to the inputString:
      inputString += inChar; 
    }
  }
}
