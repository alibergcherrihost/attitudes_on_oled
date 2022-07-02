/********************************************************************/
/* MPU6050_OLED_I2C.ino
 * Author: Gabriel Christopher
 * Date: 2021/06/02
 * Function: Displays all the attitude angles from mpu6050 onto the display using arduino.
 * This code is for a 128x64 pixel Monochrome OLED display 
 * based on SSD1306/SH1106 drivers using I2C to communicate.
 * 3 pins are required to interface (two I2C and one reset).
 * Version: 1.1 */
/********************************************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_SH110X.h>   // uncomment if you are using an SH1106 based display
//#include <Adafruit_SSD1306.h>  // uncomment if you are using an SSD1306 based display
#include <MPU6050_light.h>
#include <Fonts/Waukegan_LDO5pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

// positional parameters for icon placement
int xx=40;
int yy=10;
int tt=30;

unsigned long previousMillis = 0;

//------------------------------------------------------------------------------------------------------
// hex array needed for the gyro icon at the startup
const unsigned char PROGMEM gyro [] = {    // storing the array in progmem to save dynamic ram          
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x70, 0x00, 0x03,
0xE0, 0x07, 0xC0, 0xF0, 0x00, 0x0F, 0x00, 0x00, 0xF0, 0xF0, 0x00, 0x1C, 0x00, 0x00, 0x39, 0xE0,
0x00, 0x38, 0x00, 0x00, 0x1F, 0x80, 0x00, 0x60, 0x00, 0x0F, 0x07, 0x00, 0x00, 0xC0, 0x00, 0xFF,
0xC3, 0x00, 0x01, 0x8F, 0x83, 0xE0, 0xF1, 0x80, 0x03, 0x9F, 0xEF, 0x00, 0x39, 0xC0, 0x03, 0x18,
0x7C, 0x00, 0x18, 0xC0, 0x06, 0x18, 0x7C, 0x00, 0x0C, 0x60, 0x06, 0x18, 0xEF, 0x00, 0x0C, 0x60,
0x0C, 0x19, 0xC3, 0x80, 0x04, 0x30, 0x0C, 0x0D, 0x81, 0xC0, 0x06, 0x30, 0x0C, 0x0C, 0x00, 0xE0,
0x06, 0x30, 0x18, 0x06, 0x1C, 0x70, 0x06, 0x18, 0x18, 0x07, 0x1F, 0x18, 0x04, 0x18, 0x18, 0x03,
0x1F, 0x8C, 0x0C, 0x18, 0x18, 0x19, 0x8F, 0xCE, 0x0C, 0x18, 0x18, 0x19, 0xC6, 0xE7, 0x0C, 0x18,
0x18, 0x30, 0xE7, 0x63, 0x98, 0x18, 0x18, 0x30, 0x63, 0xF1, 0x98, 0x18, 0x18, 0x30, 0x31, 0xF8,
0xF0, 0x18, 0x18, 0x20, 0x18, 0xF8, 0xF0, 0x18, 0x18, 0x60, 0x0C, 0x38, 0x60, 0x18, 0x0C, 0x60,
0x07, 0x00, 0xF0, 0x30, 0x0C, 0x60, 0x03, 0x81, 0xF0, 0x30, 0x0C, 0x60, 0x01, 0xC3, 0x98, 0x30,
0x06, 0x30, 0x00, 0xF3, 0x18, 0x60, 0x06, 0x30, 0x00, 0x38, 0x18, 0x60, 0x03, 0x18, 0x00, 0x1E,
0x18, 0xC0, 0x03, 0x9C, 0x00, 0xC7, 0xF9, 0xC0, 0x01, 0x8F, 0x07, 0xC1, 0xF1, 0x80, 0x00, 0xC3,
0xFF, 0x00, 0x03, 0x00, 0x00, 0xE0, 0xF0, 0x00, 0x06, 0x00, 0x01, 0xF8, 0x00, 0x00, 0x1C, 0x00,
0x07, 0x9C, 0x00, 0x00, 0x38, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0xF0, 0x00, 0x0F, 0x03, 0xE0, 0x07,
0xC0, 0x00, 0x0E, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
//-------------------------------------------------------------------------------------------------------------------------------------
// Let's create the OLED display
// uncomment the below lines if you are using an SH1106 based display
/*
#define OLED_RESET -1           // Reset pin # (or -1 if sharing Arduino reset pin)
#define i2c_Address 0x3D        // initialize with the I2C addr 0x3D Typically Adafruit OLED's
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, OLED_RESET);
*/
// uncomment the below lines if you are using an SSD1306 based display
/*
#define OLED_RESET  4           // Reset pin # (or -1 if sharing Arduino reset pin)
#define i2c_Address 0x3D        // See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);
*/
//--------------------------------------------------------------------------------------------

MPU6050 mpu(Wire);                                    // create gyro object

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);  
  //display.begin(SSD1306_SWITCHCAPVCC, i2c_Address); // uncomment if you are using an SSD1306 based display
  //display.begin(i2c_Address, true);                 // uncomment if you are using an SH1106 based display                          
  display.setTextSize(1);             
  display.setTextColor(SH110X_WHITE);                 // SSD1306_WHITE for SSD1306 & SH110X_WHITE for SH1106 
  display.setFont(&FreeSans9pt7b);                    
  display.clearDisplay();                             // clear the buffer
  display.drawBitmap(xx, yy,gyro,48,48, 1);           // display the gyro icon at the startup
  display.display();
  delay(1000);   
  display.clearDisplay();                                            
  Wire.begin();
  mpu.begin();
  display.setCursor(0,20); 
  display.println(F("Calibrating..."));               // storing in progmem to save dynamic ram
  display.display(); 
  display.setFont(&Waukegan_LDO5pt7b);
  display.println(F("NO NOT MOVE, STAY STILL"));  
  display.display();
  mpu.calcGyroOffsets();                              // calibration
  display.clearDisplay();
  display.setFont(&FreeSans9pt7b);
  display.setCursor(20,27); 
  display.println(F("Calibration\n    Complete!")); 
  display.display();  
  delay(1000);
  display.setTextSize(1);          
}

void loop() {
  mpu.update();                                      // get the accelerometer & gyro data 
  if((millis()-previousMillis)>10)                   // print data every 10ms
  {                                           
    display.clearDisplay();                          // clear the buffer
    display.setCursor(0,16);                         
    display.print("Pitch :  ");
    display.println(mpu.getAngleX());                // display the pitch angle
    display.print("Roll :    ");
    display.println(mpu.getAngleY());                // display the roll angle
    display.print("Yaw :   ");
    display.print(mpu.getAngleZ());                  // display the yaw angle
    display.display();                               // show the display buffer on the screen
    previousMillis = millis();  
  }
}
