// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
////Technical support:goodtft@163.com

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>// Hardware-specific library
#include <SD.h>
#include <SPI.h>
#include <DallasTemperature.h>
#include <OneWire.h>



// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
#define DATA_TRANS 26
#define DATA_TRANS_2 28

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).


#define SD_CS 10
int t_DS18B20pin=40;
// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#include <Wire.h>
float c2_arr[55]={25.63,24.34,26.32,27.11,24.12,27.92,25.91,27.23,26.15,28.12,24.43,21.11,24.34,25.22,27.12,31.23,31.11,32.12,29.81,27.12,29.15,27.64,23.43,27.23,31.12,27.82,30.33,32.22,33.12,30.05,28.96,29.88,27.82,29.34,27.43,28.12,24.43,21.11,24.34,25.22,27.12,31.23,28.12,24.43,21.11,24.34,25.22,27.12,31.23,31.11,32.12,29.81,27.12,29.15,27.64};
float a2_arr[55]={32.21,34.34,32.31,34.42,34.11,35.62,37.11,36.12,31.12,34.12,32.12,35.43,30.12,29.81,34.12,35.32,35.62,37.11,36.12,31.12,34.12,32.12,35.21,34.53,32.14,34.11,34.12,32.12,35.43,30.12,29.81,34.12,35.32,29.32,35.12,34.12,33.56,31.24,37.12,34.53,32.14,34.11,34.12,32.12,35.43,30.12,29.81,27.12,31.23,31.11,32.12,29.81,27.12,29.15,27.64};
float b2_arr[55]={36.87,36.48,32.47,33.45,34.58,30.26,35.36,31.21,26.32,29.87,32.49,35.39,37.21,36.47,34.69,37.84,32.65,32.36,36.45,37.12,30.21,31.56,32.45,33.14,36.14,36.87,36.48,32.47,33.45,34.58,32.49,35.39,37.21,36.47,34.69,32.65,32.36,36.45,37.12,30.21,36.87,36.48,32.47,33.45,34.58,31.21,26.32,29.87,32.49,35.39,30.25,30.47,36.48,32.47,36.7};
int d2_arr[55] = {23.6,25.69,25.39,24.49,26.7,26.74,26.48,29.34,29.46,29.00,31.25,30.65,32.78,30.78,30.62,30.45,28.48,29.69,23.47,36.15,26.48,24.83,23.16,29.34,26.47,31.25,30.65,32.78,30.78,30.62,23.6,25.69,25.39,24.49,26.7,30.45,28.48,29.69,23.47,36.15,30.45,28.48,29.69,23.47,36.15,23.6,25.69,25.39,24.49,26.7,26.74,26.48,29.34,29.46,29.00};
int e2_arr[55] = {34.98,29.36,31.49,30.28,28.47,26.97,24.32,27.39,23.61,27.63,26.14,29.36,29.14,27.98,25.88,21.96,23.69,27.36,26.47,24.69,29.99,24.68,29.47,21.37,25.62,26.14,29.36,29.14,27.98,25.88,26.97,24.32,27.39,23.61,27.63,34.98,29.36,31.49,30.28,28.47,29.99,24.68,29.47,21.37,25.62,26.14,29.36,29.14,27.98,25.88,26.97,24.32,27.39,23.61};
OneWire t_oneWirePin(t_DS18B20pin);
DallasTemperature t_sensor1(&t_oneWirePin);
String SA;
String NVP;
String a1;
String a2;
String b1;
String b2;
String c1;
String c2;
String d1;
String d2;
String e1;
String e2;
String f1;
String f2;
String g1;
String g2;




String frame_rxv;

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;

Adafruit_GFX_Button buttons[15];




/*__________________________________________________________________________________________________________*/
void setup(void) {
 
  Wire.begin(2); // address
  delay(100);
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));
  pinMode(DATA_TRANS,INPUT);
  pinMode(DATA_TRANS_2,OUTPUT);
  digitalWrite(DATA_TRANS_2,HIGH);






#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  Serial.println(F("Using Adafruit 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Adafruit 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); 
  Serial.print(tft.width()); 
  Serial.print("x"); 
  Serial.println(tft.height());

  tft.reset();

  uint16_t identifier = tft.readID();
 // if(identifier==0x0101)
     // identifier=0x9341;
  tft.begin(0x9341);
  delay(1000);
  Serial.print(F("Initializing SD card..."));
  if (!SD.begin(SD_CS)) {
    Serial.println(F("failed!"));
    return;
  }
  Serial.println(F("OK!"));
  tft.setRotation(2);
  bmpDraw("logonew.bmp", 0, 0);
  delay(3000);
  tft.setRotation(4);
 /* if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.4\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;*/
 
tft.fillScreen(BLACK);
 
 
  tft.drawRoundRect(10, 10, 108, 140, 10, WHITE);
  tft.fillRoundRect(20, 20, 88, 120, 4, BLUE);
  
  tft.drawRoundRect(10, 170, 108, 140, 10, WHITE);
  tft.fillRoundRect(20, 180, 88, 120, 4, BLUE);
  
  tft.drawRoundRect(10, 330, 108, 140, 10, WHITE);
  tft.fillRoundRect(20, 340, 88, 120, 4, BLUE);
  

  tft.drawRoundRect(170, 10, 108, 140, 10, WHITE);
  tft.fillRoundRect(180, 20, 88, 120, 4, BLUE);
  tft.drawRoundRect(170, 170, 108, 140, 10, WHITE);
  tft.fillRoundRect(180, 180, 88, 120, 4, BLUE);
  tft.drawRoundRect(170, 330, 108, 140, 10, WHITE);
  tft.fillRoundRect(180, 340, 88, 120, 4, BLUE);
  /*tft.setRotation(3);
  PrintText("T1",35,285,RED,YELLOW,3);
  PrintText("T2",200,285,RED,YELLOW,3);
  PrintText("T3",365,285,RED,YELLOW,3);

  PrintText("T4",35,125,RED,YELLOW,3);
  PrintText("T5",200,125,RED,YELLOW,3);
  PrintText("T6",365,125,RED,YELLOW,3);
  delay(7000);
/* tft.setRotation(2);
  tft.fillRect(0,0,160,480, YELLOW);
 for(int iii=0,kkk=1;iii<5;iii++,kkk++)
 tft.drawRect(31-kkk, 41-kkk, 100+(2*iii), 400+(2*iii), RED);
 tft.setRotation(3);
 tft.setFont(&FreeSerif9pt7b);
 PrintText("Hello",55,265,RED,YELLOW,4);
 tft.setRotation(8);
 
 tft.fillRect(0,0,160,480, BLACK);
 tft.setRotation(3);
 PrintText("!!WARNING!!",20,80,RED,YELLOW,4);
 */
  }
  

/*------------------------------------------------------------------------------------------------------------------------------------------*/

String t_temp_ds1(){
  t_sensor1.requestTemperatures();
  float t_dstemp1=t_sensor1.getTempCByIndex(0);
  String t_dstemp1_1=(String)t_dstemp1;
  return t_dstemp1_1;
}

  
void PrintText(String str,int16_t x, int16_t y,uint16_t c,uint16_t cbg,uint8_t s)
 {tft.setCursor(x,y);
    tft.setTextColor(c,cbg);
   tft.setTextSize(s);
   tft.println(str);
   
 }
  

  void getData(String &SA,String &NVP,String &a,String &b, String &c, String &d, String &e, String &f,String &g,String &h, String &i, String &z,String &z1,String &z2,String &z3,String &z4,String s)
{
  //the data frame, as received
  String set[16];//string array,in which the 6 values will be stored at 6 indices
  int j,k=0;
  for(int i=0;i<16;i++)
  {
  for(j=1;;j++)
     {
     k++;
     if(s.charAt(j)=='%'||s.charAt(j)=='@')
     break;
     }
  set[i]=s.substring(1,k);//extraction of required substring
  s.remove(0,j);
  k=0;
  
  
  }
    //Value of 6 parameters, stored in pre defined addresses---extension can be possibly made, but only after expanding the address variable list
  
   SA=set[0];
  NVP=set[1];
    a=set[2];
    b=set[3];
    c=set[4];
    d=set[5];
    e=set[6];
    f=set[7];
    g=set[8];
    h=set[9];
    i=set[10];
    z=set[11];
    z1=set[12];
    z2=set[13];
    z3=set[14];
    z4=set[15];
 }

/*----------------------------------------------*/
void estab_wire()
{
while(digitalRead(DATA_TRANS)!=LOW)
Serial.println("Data_Trans detected low!");
digitalWrite(DATA_TRANS_2,LOW);
delay(200);
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
String invitation = "start";
int i_s  = 0;
Wire.beginTransmission(1); // transmit to device #8
while(invitation[i_s] != '\0')
    {   Wire.write(invitation[i_s] );// sends five bytes
        Serial.print(invitation[i_s] );
        i_s++;
        
    }
Wire.endTransmission();    // stop transmitting
Serial.print("invitation send");

//delay(2000);(old delay)
//delay(1500);

  
//frame_rxv = "";
int loop = 0;
int count = 0;
for(loop  = 0 ; loop < 5 ; loop++)
{//Wire.flush();
Wire.requestFrom(1,24);    // request 6 bytes from slave device #1

  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    //delay(500);(old delay)
    //delay(100);
    frame_rxv = frame_rxv + String(c);
    if(c == '@')
    {count++;
        if(count == 2) // 2 value ka reason to be find out....
        {count=0;
          break;}
    }
    
    Serial.print(c);// print the character
  }
}



   // Serial.println();// print the character
    Serial.println("-----------------------------------");// print the character
  Serial.println(frame_rxv);
  Serial.println("-----------------------------------");// print the character

  getData(SA,NVP,a1,a2,b1,b2,c1,c2,d1,d2,e1,e2,f1,f2,g1,g2,frame_rxv);
  Serial.println("-------------------------------");
  Serial.println("SA:");
  Serial.println(SA);
  Serial.println("NVP:");
  Serial.println(NVP);
  Serial.println("Seta1:");
  Serial.println(a1);
  Serial.println("Seta2");
  Serial.println(a2);
  Serial.println("Setb1");
  Serial.println(b1);
  Serial.println("Setb2");
  Serial.println(b2);
  Serial.println("Setc1");
  Serial.println(c1);
  Serial.println("Setc2");
  Serial.println(c2);
  Serial.println("Setd1");
  Serial.println(d1);
  Serial.println("Setd2");
  Serial.println(d2);
  Serial.println("Sete1");
  Serial.println(e1);
  Serial.println("Sete2");
  Serial.println(e2);
  digitalWrite(DATA_TRANS_2,HIGH);
  //delay(1000);
  
  frame_rxv = "";
  delay(500);
  
}

 

/*________________________________________________________________________________________________________________________________*/
void loop(void) {
delay(200);

estab_wire();
 //delay(1000);(old delay)
 delay(1000);
if(f2=="Y")
{
tft.width();
tft.height();
//
tft.reset();
  tft.setRotation(2);
  uint16_t identifier = tft.readID();
 // if(identifier==0x0101)
     // identifier=0x9341;
  tft.begin(0x9341);
  delay(1000);
  tft.setRotation(2);
  //bmpDraw("logonew.bmp", 0, 0);
  tft.setRotation(4);
  
  tft.fillScreen(BLACK);
 
 
  tft.drawRoundRect(10, 10, 108, 140, 10, WHITE);
  tft.fillRoundRect(20, 20, 88, 120, 4, BLUE);
  
  tft.drawRoundRect(10, 170, 108, 140, 10, WHITE);
  tft.fillRoundRect(20, 180, 88, 120, 4, BLUE);
  
  tft.drawRoundRect(10, 330, 108, 140, 10, WHITE);
  tft.fillRoundRect(20, 340, 88, 120, 4, BLUE);
  

  tft.drawRoundRect(170, 10, 108, 140, 10, WHITE);
  tft.fillRoundRect(180, 20, 88, 120, 4, RED);
  tft.drawRoundRect(170, 170, 108, 140, 10, WHITE);
  tft.fillRoundRect(180, 180, 88, 120, 4, BLUE);
  tft.drawRoundRect(170, 330, 108, 140, 10, WHITE);
  tft.fillRoundRect(180, 340, 88, 120, 4, BLUE);
  tft.setRotation(3);

  
  PrintText("Motor",35,285,RED,YELLOW,3);
  PrintText("Battery",185,285,RED,YELLOW,3);
  PrintText("Smoke",361,285,RED,YELLOW,3);

  PrintText("SF",41,125,RED,YELLOW,3);
  PrintText("SR",225,125,RED,YELLOW,3);
  PrintText("MCU",369,125,RED,YELLOW,3);
  

//  tft.setRotation(3);
  Serial.println("values to be printed");
  Serial.println(a2);
  Serial.println(b2);
  Serial.println(c2);
  Serial.println(d2);
  Serial.println(e2);
  Serial.println(f2);

  PrintText(t_temp_ds1(),50,50,CYAN,BLACK,2);
  PrintText(a2,200,50,CYAN,BLACK,2);
   PrintText(c2,350,50,CYAN,BLACK,2);
  
PrintText(d2,50,220,CYAN,BLACK,2);
  PrintText(e2,200,220,CYAN,BLACK,2);
   PrintText(f2,380,220,CYAN,BLACK,2);
   
// PrintText(g2,200,150,CYAN,BLACK,2);

SA="";
  NVP="";
    a1="";
    a2="";
    b1="";
    b2="";
    c1="";
    c2="";
    d1="";
    d2="";
    e1="";
    e2="";
    f1="";
    f2="";
    g1="";
    g2="";



  
 //delay(3000);(old delay)
 delay(3000);
 
     
}
else
{
  tft.width();
tft.height();
//
tft.reset();
  tft.setRotation(2);
  uint16_t identifier = tft.readID();
 // if(identifier==0x0101)
     // identifier=0x9341;
  tft.begin(0x9341);
  delay(1000);
  tft.setRotation(2);
  //bmpDraw("logonew.bmp", 0, 0);
  tft.setRotation(4);
  
  tft.fillScreen(BLACK);
 
 
  tft.drawRoundRect(10, 10, 108, 140, 10, WHITE);
  tft.fillRoundRect(20, 20, 88, 120, 4, BLUE);
  
  tft.drawRoundRect(10, 170, 108, 140, 10, WHITE);
  tft.fillRoundRect(20, 180, 88, 120, 4, BLUE);
  
  tft.drawRoundRect(10, 330, 108, 140, 10, WHITE);
  tft.fillRoundRect(20, 340, 88, 120, 4, BLUE);
  

  tft.drawRoundRect(170, 10, 108, 140, 10, WHITE);
  tft.fillRoundRect(180, 20, 88, 120, 4, BLUE);
  tft.drawRoundRect(170, 170, 108, 140, 10, WHITE);
  tft.fillRoundRect(180, 180, 88, 120, 4, BLUE);
  tft.drawRoundRect(170, 330, 108, 140, 10, WHITE);
  tft.fillRoundRect(180, 340, 88, 120, 4, BLUE);
  tft.setRotation(3);

  
  PrintText("Motor",35,285,RED,YELLOW,3);
  PrintText("Battery",185,285,RED,YELLOW,3);
  PrintText("Smoke",361,285,RED,YELLOW,3);

  PrintText("SF",41,125,RED,YELLOW,3);
  PrintText("SR",225,125,RED,YELLOW,3);
  PrintText("MCU",369,125,RED,YELLOW,3);
  

//  tft.setRotation(3);
  Serial.println("values to be printed");
  Serial.println(a2);
  Serial.println(b2);
  Serial.println(c2);
  Serial.println(d2);
  Serial.println(e2);
  Serial.println(f2);

  PrintText(t_temp_ds1(),50,50,CYAN,BLACK,2);
  PrintText(a2,200,50,CYAN,BLACK,2);
   PrintText(c2,350,50,CYAN,BLACK,2);
  
PrintText(d2,50,220,CYAN,BLACK,2);
  PrintText(e2,200,220,CYAN,BLACK,2);
   PrintText(f2,380,220,CYAN,BLACK,2);
   
// PrintText(g2,200,150,CYAN,BLACK,2);

SA="";
  NVP="";
    a1="";
    a2="";
    b1="";
    b2="";
    c1="";
    c2="";
    d1="";
    d2="";
    e1="";
    e2="";
    f1="";
    f2="";
    g1="";
    g2="";



  
 //delay(3000);(old delay)
 delay(3000);
 
  
}
/*tft.width();
tft.height();
//
tft.reset();
  tft.setRotation(2);
  uint16_t identifier = tft.readID();
 // if(identifier==0x0101)
     // identifier=0x9341;
  tft.begin(0x9341);
  delay(1000);
  tft.setRotation(2);
  //bmpDraw("logonew.bmp", 0, 0);
  tft.setRotation(4);
  
  tft.fillScreen(BLACK);
 
 
  tft.drawRoundRect(10, 10, 108, 140, 10, WHITE);
  tft.fillRoundRect(20, 20, 88, 120, 4, BLUE);
  
  tft.drawRoundRect(10, 170, 108, 140, 10, WHITE);
  tft.fillRoundRect(20, 180, 88, 120, 4, BLUE);
  
  tft.drawRoundRect(10, 330, 108, 140, 10, WHITE);
  tft.fillRoundRect(20, 340, 88, 120, 4, BLUE);
  

  tft.drawRoundRect(170, 10, 108, 140, 10, WHITE);
  tft.fillRoundRect(180, 20, 88, 120, 4, BLUE);
  tft.drawRoundRect(170, 170, 108, 140, 10, WHITE);
  tft.fillRoundRect(180, 180, 88, 120, 4, BLUE);
  tft.drawRoundRect(170, 330, 108, 140, 10, WHITE);
  tft.fillRoundRect(180, 340, 88, 120, 4, BLUE);
  tft.setRotation(3);

  
  PrintText("Motor",35,285,RED,YELLOW,3);
  PrintText("Battery",185,285,RED,YELLOW,3);
  PrintText("Smoke",361,285,RED,YELLOW,3);

  PrintText("SF",41,125,RED,YELLOW,3);
  PrintText("SR",225,125,RED,YELLOW,3);
  PrintText("MCU",369,125,RED,YELLOW,3);
  

//  tft.setRotation(3);
  Serial.println("values to be printed");
  Serial.println(a2);
  Serial.println(b2);
  Serial.println(c2);
  Serial.println(d2);
  Serial.println(e2);
  Serial.println(f2);

  PrintText(a2,50,50,CYAN,BLACK,2);
  PrintText(b2,200,50,CYAN,BLACK,2);
   PrintText(c2,350,50,CYAN,BLACK,2);
  
PrintText(d2,50,220,CYAN,BLACK,2);
  PrintText(e2,200,220,CYAN,BLACK,2);
   PrintText(f2,380,220,CYAN,BLACK,2);
   
// PrintText(g2,200,150,CYAN,BLACK,2);

SA="";
  NVP="";
    a1="";
    a2="";
    b1="";
    b2="";
    c1="";
    c2="";
    d1="";
    d2="";
    e1="";
    e2="";
    f1="";
    f2="";
    g1="";
    g2="";



  
 //delay(3000);(old delay)
 delay(3000);
 
  */
}






















#define BUFFPIXEL 20

void bmpDraw(char *filename, int x, int y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel in buffer (R+G+B per pixel)
  uint16_t lcdbuffer[BUFFPIXEL];  // pixel out buffer (16-bit per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
  uint8_t  lcdidx = 0;
  boolean  first = true;

  if((x >= 320) || (y >= 480)) return;

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');
  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.println(F("File not found"));
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.println(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= 320)  w = 320  - x;
        if((y+h-1) >= 480) h = 480 - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...
          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each column...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              // Push LCD buffer to the display first
              if(lcdidx > 0) {
                tft.pushColors(lcdbuffer, lcdidx, first);
                lcdidx = 0;
                first  = false;
              }
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            lcdbuffer[lcdidx++] = tft.color565(r,g,b);
          } // end pixel
        } // end scanline
        // Write any remaining data to LCD
        if(lcdidx > 0) {
          tft.pushColors(lcdbuffer, lcdidx, first);
        } 
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
