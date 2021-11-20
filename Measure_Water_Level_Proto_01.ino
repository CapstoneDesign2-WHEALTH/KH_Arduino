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
//수위측정용 
int trigPin = 6;
int echoPin = 7;
long dis[5]={0,0,0,0,0};
long last=0;
int cnt=0;
//114 empty 66 max 
float water=3.5;
void LCD_PRINT(String v1, String v2="NULL");
float Calculate_Water(int dis);
void setup(){                                                                                                    
Serial.begin(9600);            // 통신속도 9600으로 설정
Serial.println("Hello!");      // 시리얼모니터에 Hello 출력
BTSerial.begin(9600);
LCD_PRINT("Welcome!");
pinMode(echoPin, INPUT);   // echoPin 입력    
pinMode(trigPin, OUTPUT);  // trigPin 출력  
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
float Calculate_Water(int dis)
{
  return (114-dis) * 3.5;
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
  
  long duration, distance;
  long dis1,dis2,dis3;
  long sip;
  if(cnt==0 || cnt == 4){
  digitalWrite(trigPin, HIGH);  // trigPin에서 초음파 발생(echoPin도 HIGH)        
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);    // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  dis[cnt] = ((float)(340 * duration) / 1000) / 2; 
  if(abs(dis[4]-dis[0])<=1){
    Serial.print("lid is closed\n");
    Serial.print(dis[4]);
    //if(Serial.available())
    BTSerial.write("11"); 
  }
  else
    Serial.print("lid is opened\n");
  //distance=dis[cnt];
  }
  /*
  if(distance <= 117 && distance >=57)
  {
    long fluc =(dis[0]-dis[4]);
    float sip=0.0;
    //Serial.print(fluc);
    if(fluc >= -4 && fluc <=4){
      Serial.print("lid is closed\n");
      //Serial.print(dis[4]);
      if(last==0)
        last = dis[4];
      else 
        last = dis[4];
      sip=(last-dis[4])*water;
      
      Serial.print(sip);
    }
    else
     Serial.print("lid is opened\n");
  }
  else
    Serial.print("lid is opened\n");*/
  delay(1000); // 1000ms
  cnt=(cnt+1)%5;
  /*
  while (Serial.available()){ // pc -> bluetooth
    
    byte data = Serial.read();
    // Print a message to the LCD.
    BTSerial.write(data); 
  }*/
   
}
