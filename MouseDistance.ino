
#include <math.h>
#include "MI0283QT2.h"
/*
 * an arduino sketch to interface with a ps/2 mouse.
 * Also uses serial protocol to talk back to the host
 * and report what it finds.
 */

/*
 * Pin 5 is the mouse data pin, pin 6 is the clock pin
 * Feel free to use whatever pins are convenient.
 */
#define MDATA 5
#define MCLK 6


/*
 * according to some code I saw, these functions will
 * correctly set the mouse clock and data pins for
 * various conditions.
 */

double xPos,yPos,xPrev,yPrev,dist = 0.0;

bool rightChk = false;

MI0283QT2 lcd ; 

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

void mouse_write(char data)
{
  char i;
  char parity = 1;

  //  Serial.print("Sending ");
  //  Serial.print(data, HEX);
  //  Serial.print(" to mouse\n");
  //  Serial.print("RTS");
  /* put pins in output mode */
  gohi(MDATA);
  gohi(MCLK);
  delayMicroseconds(300);
  golo(MCLK);
  delayMicroseconds(300);
  golo(MDATA);
  delayMicroseconds(10);
  /* start bit */
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
  /* parity */
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
  /* stop bit */
  gohi(MDATA);
  delayMicroseconds(50);
  while (digitalRead(MCLK) == HIGH)
    ;
  /* wait for mouse to switch modes */
  while ((digitalRead(MCLK) == LOW) || (digitalRead(MDATA) == LOW))
    ;
  /* put a hold on the incoming data. */
  golo(MCLK);
  //  Serial.print("done.\n");
}

/*
 * Get a byte of data from the mouse
 */
char mouse_read(void)
{
  char data = 0x00;
  int i;
  char bit = 0x01;

  //  Serial.print("reading byte from mouse\n");
  /* start the clock */
  gohi(MCLK);
  gohi(MDATA);
  delayMicroseconds(50);
  while (digitalRead(MCLK) == HIGH)
    ;
  delayMicroseconds(5);  /* not sure why */
  while (digitalRead(MCLK) == LOW) /* eat start bit */
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
  /* eat parity bit, which we ignore */
  while (digitalRead(MCLK) == HIGH)
    ;
  while (digitalRead(MCLK) == LOW)
    ;
  /* eat stop bit */
  while (digitalRead(MCLK) == HIGH)
    ;
  while (digitalRead(MCLK) == LOW)
    ;

  /* put a hold on the incoming data. */
  golo(MCLK);
  //  Serial.print("Recvd data ");
  //  Serial.print(data, HEX);
  //  Serial.print(" from mouse\n");
  return data;
}

void mouse_init()
{
  gohi(MCLK);
  gohi(MDATA);
  //  Serial.print("Sending reset to mouse\n");
  mouse_write(0xff);
  mouse_read();  /* ack byte */
  //  Serial.print("Read ack byte1\n");
  mouse_read();  /* blank */
  mouse_read();  /* blank */
  //  Serial.print("Sending remote mode code\n");
  mouse_write(0xf0);  /* remote mode */
  mouse_read();  /* ack */
  //  Serial.print("Read ack byte2\n");
  delayMicroseconds(100);
}

void setup()
{
  Serial.begin(9600);
  lcd.init (4 ) ; // spi-clk = Fcpu / 4
  lcd.clear ( RGB ( 255 , 255 , 255 ) ) ; // clear screen in white 
  lcd.setOrientation ( 270 ) ; // configure display orientation in degrees: 0,90,180,270 
  lcd.clear ( RGB ( 255 , 255 , 255 ) ) ; // clear screen in white
  lcd.led ( 50 ) ; // fixed brightness 
  lcd.drawText(20,150,"Distance: ",1,RGB(0,0,0),RGB(255,255,255));
  mouse_init();
}

/*
 * get a reading from the mouse and report it back to the
 * host via the serial line.
 */
void loop()
{
  char mstat;
  char mx;
  char my;


  // Block runs only when system is not paused
  // takes reading from mouse
  // gives X,Y values
  if(!rightChk){
    
    mouse_write(0xeb);  /* give me data! */
    mouse_read();      /* ignore ack */
    mstat = mouse_read();
    mx = mouse_read();
    my = mouse_read();

    // ---->>> For Calculating Distance    
    
    /* Convert X, Y values to int as below in code,
     *  Use Math.sqrt and Math.pow for calculating distance between 2 points
     *  Store previous values of X,Y to continue when paused and started 
     *  Use formula for measuring distance between two points
     */
  
    Serial.print("\tX=");
    Serial.print(mx, DEC);
    Serial.print("\tY=");
    Serial.print(my, DEC);
    Serial.println();

    xPos = double(mx);
    yPos = double(my);
    computeDistance(xPos,yPos,xPrev,yPrev);
 
  }
  
  mouse_write(0xeb);  
  mouse_read();
  mstat = mouse_read();


  // converting clicks variable to int
  int btnChk = int (mstat);
  // converting x,y positions to float for distance calculation
  
  delay(30);  

  // for int value of 10, right button is clicked
  if( btnChk == 10){
    Serial.print("Right Button Clicked, System Paused!,Left Click to Continue");
    rightChk = true;
    delay(40);  
  }

  // for int value of 9, left button is clicked
  if(btnChk == 9){
    Serial.print("System Started!");
    rightChk = false;
    delay(40);  
  }

  // for int value of 12, Mouse Wheel is clicked
  if(btnChk == 12){
    Serial.print("System Stopped!");
    xPrev,yPrev,dist = 0.0;
    lcd.clear ( RGB ( 255 , 255 , 255 ) ) ; 
    lcd.drawText(20,150,"Distance: ",1,RGB(0,0,0),RGB(255,255,255));
    lcd.drawFloat(95,150,dist,1,1,RGB ( 0, 0 , 0),RGB ( 255 , 255 , 255 ));  
    delay(30);
  }

  xPrev = xPos;
  yPrev = yPos;
 
  }

   void computeDistance(double xNew,double yNew,double xPrev,double yPrev){
    double xPos = xNew;
    double yPos = yNew;

    double sqr_diff_x = (xPos - xPrev) * (xPos - xPrev);
    double sqr_diff_y = (yPos - yPrev) * (yPos - yPrev);
    double sum = sqr_diff_x + sqr_diff_y;
    dist += sqrt(sum);

    Serial.print("Dist: ");
    char s[50];
    sprintf(s,"%.2f", dist);
    Serial.println(s);
    lcd.drawFloat(95,150,dist/12,2,1,RGB ( 0, 0 , 0),RGB ( 255 , 255 , 255 ));   
    lcd.drawText(155,150,"cm",1,RGB(0,0,0),RGB(255,255,255));
}

