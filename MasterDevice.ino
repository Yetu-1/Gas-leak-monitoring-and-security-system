#include <SPI.h>
#include <Wire.h>
#include <Sim800l.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdio.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#include <SoftwareSerial.h>
// creating Sim800l object
#include <avr/interrupt.h>
Sim800l Sim800l;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define PIR_MOTION_SENSOR 9//Use pin 2 to receive the signal from the module 
#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

SoftwareSerial bluetoothModule(3, 2); // RX, TX
/* -----this is a pointer to a character array that define the number we will be sending the messages to-----*/
char* number;

bool error; //to catch the response of sendSms;
double alert = 0;
/* if alert = 1{ a gas leak has been detected by the slave device}
 * if alert = 2{ motion has been detected by slave device}
 */

void setup() {
  // set up uart for the bluetooth module
  bluetoothModule.begin(9600);
  number= "08102420672";
  // this function is for setting  up the display
  setUpDisplay();
  // set up the pin of the motion sensor as an input.. so we can receive data from it
  pinMode(9,INPUT);
  printtoDisplay("SEARCHING FOR SIGNAL",5,10);
  delay(10000);
}

void loop() { 
  //if the motion sensor detects the moving people?
  if(isPeopleDetected()){
   //  printtoDisplay("Person detected",15,10);
   alert = 3;
  }
  // checks if the slave device has sent anything
  if (bluetoothModule.available()>0) {
    alert = bluetoothModule.read(); // possible values are 1 and 2
  }
  
  if(alert == 1){
    display.clearDisplay();
    printtoDisplay("FROM SLAVE",2,1);
    printtoDisplay("GAS LEAK DETECTED",15,17);
    delay(1000);
    // send message to the number that a gas leak has been detected
    display.clearDisplay();
    printtoDisplay("  SENDING...",15,10);
    Sim800l.begin();
    error= Sim800l.sendSms(number, "ALERT!!. GAS LEAK DETECTED");
    delay(2000);
    display.clearDisplay();
    printtoDisplay("MESSAGE SENT",17,10);
    bluetoothModule.begin(9600);
    delay(1000);
  }
  else if(alert == 2   ){
    display.clearDisplay();
    printtoDisplay("FROM SLAVE",2,1);
    printtoDisplay("INTRUDER DETECTED",15,17);
    delay(1000);
    // send message to the number that a gas leak has been detected
    display.clearDisplay();
    printtoDisplay("  SENDING...",15,10);
    Sim800l.begin();
    error= Sim800l.sendSms(number, "ALERT!!. INTRUDER DETECTED BY SLAVE");
    delay(2000);
    printtoDisplay("MESSAGE SENT",17,10);
    bluetoothModule.begin(9600);
    delay(1000);
  }
  else if(alert == 3){
    display.clearDisplay();
    printtoDisplay("FROM MASTER",2,1);
    printtoDisplay("INTRUDER DETECTED",15,17);
    delay(1000);
    // send message to the number that a gas leak has been detected
    display.clearDisplay();
    printtoDisplay("  SENDING...",15,10);
    Sim800l.begin();
    error= Sim800l.sendSms(number, "ALERT!!. INTRUDER DETECTED BY MASTER");
    delay(2000);
    display.clearDisplay();
    printtoDisplay("MESSAGE SENT",17,10);
    bluetoothModule.begin(9600);
    delay(1000);
  }
  else{
    display.clearDisplay();
    printtoDisplay("  SYSTEM CLEAR",15,12);
  }
  alert = 0;
}
void setUpDisplay(){
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);
  // Clear the buffer
  display.clearDisplay();
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
}

void printtoDisplay(String input, int x, int y){
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x, y);
  display.println(input);
  display.display();
}
boolean isPeopleDetected()
{
  int sensorValue = digitalRead(PIR_MOTION_SENSOR);
  if(sensorValue == HIGH)//if the sensor value is HIGH?
  {
    return true;//yes,return ture
  }
  else
  {
    return false;//no,return false
  }
}
void printsensorValueToDisplay(double sensorValue){
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30, 10);
  display.println(String(sensorValue,2));
  display.display();
}
