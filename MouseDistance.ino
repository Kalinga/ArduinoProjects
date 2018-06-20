/*
 * An Arduino program to interface with a ps/2 mouse.
 */

/*
 * Pin 5 is the mouse data pin, pin 6 is the clock pin
 */

#define MDATA 5
#define MCLK 6


bool rightChk = false;
void gohi(int pin)
{
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

void golo(int pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

/*
* serial protocol to talk back to the host
*/
void mouseWrite(char data)
{
  char i;
  char parity = 1;

  /* put pins in output mode */
  gohi(MDATA);
  gohi(MCLK);
  delayMicroseconds(300);
  golo(MCLK);
  delayMicroseconds(300);
  golo(MDATA);
  delayMicroseconds(10);

  gohi(MCLK);
  /* wait for mouse to take control of clock); */
  while (digitalRead(MCLK) == HIGH)
    ;
  /* clock is low, and we are clear to send data */
  for (i=0; i < 8; i++) {
    if (data & 0x01) {
      gohi(MDATA);
    } 
    else {
      golo(MDATA);
    }
    /* wait for clock cycle */
    while (digitalRead(MCLK) == LOW)
      ;
    while (digitalRead(MCLK) == HIGH)
      ;
    parity = parity ^ (data & 0x01);
    data = data >> 1;
  }  
  if (parity) {
    gohi(MDATA);
  } 
  else {
    golo(MDATA);
  }
  while (digitalRead(MCLK) == LOW)
    ;
  while (digitalRead(MCLK) == HIGH)
    ;
  gohi(MDATA);
  delayMicroseconds(50);
  while (digitalRead(MCLK) == HIGH)
    ;
  /* wait for mouse to switch modes */
  while ((digitalRead(MCLK) == LOW) || (digitalRead(MDATA) == LOW))
    ;
  golo(MCLK);
}

/*
 * Get a byte of data from the mouse
 */
char mouseRead(void)
{
  char data = 0x00;
  int i;
  char bit = 0x01;

  //  Serial.print("reading from mouse\n");
  /* start the clock */
  gohi(MCLK);
  gohi(MDATA);
  delayMicroseconds(50);
  while (digitalRead(MCLK) == HIGH)
    ;
  delayMicroseconds(5);  
  while (digitalRead(MCLK) == LOW) 
    ;
  for (i=0; i < 8; i++) {
    while (digitalRead(MCLK) == HIGH)
      ;
    if (digitalRead(MDATA) == HIGH) {
      data = data | bit;
    }
    while (digitalRead(MCLK) == LOW)
      ;
    bit = bit << 1;
  }

  while (digitalRead(MCLK) == HIGH)
    ;
  while (digitalRead(MCLK) == LOW)
    ;
  while (digitalRead(MCLK) == HIGH)
    ;
  while (digitalRead(MCLK) == LOW)
    ;

  golo(MCLK);

  return data;
}

void mouseInit()
{
  gohi(MCLK);
  gohi(MDATA);

  mouseWrite(0xff);
  mouseRead(); 

  mouseRead(); 
  mouseRead(); 

  mouseWrite(0xf0); 
  mouseRead();  

  delayMicroseconds(100);
}

void setup()
{
  Serial.begin(9600);
  mouseInit();
}

/*
 * reading from the mouse and report it back to the, host via the serial line.
 */
void loop()
{
  char mstat;
  char mx;
  char my;

  // for distance measurement
  int distance,prevX,prevY = 0;
  
  
  // reading from mouse, gives X,Y values
  if(!rightChk){
    
    mouseWrite(0xeb);  
    mouseRead();      
    mstat = mouseRead();
    mx = mouseRead();
    my = mouseRead();

    // Distance  Measurement    
    
  
    Serial.print("\tX=");
    Serial.print(mx, DEC);
    Serial.print("\tY=");
    Serial.print(my, DEC);
    Serial.println();
 
  }
  
  mouseWrite(0xeb);  
  mouseRead();
  mstat = mouseRead();


  // converting clicks variable to int
  int btnChk = int (mstat);

  delay(30);  

  // right button is clicked: int value of 10, 
  if( btnChk == 10){
    Serial.print("Right Clicked, System Paused!,Left to Continue");
    rightChk = true;
    prevX = 0;
    prevY = 0;
    delay(40);  
  }

  // left button is clicked: int value of 9, 
  if(btnChk == 9){
    Serial.print("System Started!");
    rightChk = false;
    delay(40);  
  }

  // Mouse Wheel is clicked: int value of 12, 
  if(btnChk == 12){
    Serial.print("System Stopped!");
    delay(30);
    exit(0);
      
  }
  /* send the data back up */
  
}
