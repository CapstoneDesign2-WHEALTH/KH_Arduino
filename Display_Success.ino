#include <SoftwareSerial.h> 
#include <Wire.h>                        // i2C 통신을 위한 라이브러리
#include <LiquidCrystal_I2C.h>   
LiquidCrystal_I2C lcd(0x27,16,2);     // 접근주소: 0x3F or 0x27
SoftwareSerial BTSerial(2, 3); // 소프트웨어 시리얼 (TX,RX) 
String lcdString="";
String WaterString1="It's time to";
String WaterString2="drink Water :-)";
String PillString1="It's time to";
String PillString2="take Pills :-)";
#define CONNECT 1
#define DISCONNECT 0
boolean isconnect=true;
boolean isdata=false;
void LCD_PRINT(String v1, String v2="NULL");
void setup(){                                                                                                    
Serial.begin(9600);            // 통신속도 9600으로 설정
Serial.println("Hello!");      // 시리얼모니터에 Hello 출력
BTSerial.begin(9600);
LCD_PRINT("Welcome!");
//lcd.backlight();                // 백라이트 켜기
//lcd.init();                      // LCD 초기화

}
void LCD_PRINT(String v1, String v2)
{
  lcd.init();
  lcd.setCursor(0,0);
  lcd.backlight();
  lcd.print(v1);
  if(v2!="NULL")
  {
    lcd.setCursor(0,1);
    lcd.print(v2);
  }
  delay(2000);
  lcd.clear();
  lcd.noBacklight();
  //delay(2000);
   
}
void loop(){
  //lcd.setCursor(0,0);   
  if(BTSerial.available())
  {
    delay(200);
      while (BTSerial.available()){ //bluetooth -> pc
        char data = (char)BTSerial.read();
        if(data!='\n'&&data!='\r')
          lcdString+=data;
        }
        isdata=true;
  }
  if(lcdString=="OK+CONN")
  {
    isconnect=1;
    LCD_PRINT("Connected-Hi!");
  }
  else if(lcdString=="OK+LOST")
  {
    isconnect=0;
    LCD_PRINT("Disconnected-Bye!");
  }
  else
  {
    if(isconnect && isdata){
            if(lcdString=="0")
              LCD_PRINT(WaterString1,WaterString2);
            else if(lcdString=="1")
              LCD_PRINT(PillString1,PillString2);
    }
    //lcdString='0';
  }
  //Serial.println(lcdString);           //시리얼 모니터에 입력한 값 출력
  lcdString="";
  isdata=false;

  while (Serial.available()){ // pc -> bluetooth
    
    byte data = Serial.read();
    // Print a message to the LCD.
    BTSerial.write(data); 
  }
   
}
