#include <SPI.h>
#include "mcp_can.h"
#include <Wire.h>
#include <Adafruit_SH1106.h>


// Init OLED Display
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);


unsigned char Flag_Recv = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];


MCP_CAN CAN(10);                                            // Set CS to pin 10

void setup()
{
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Sperrgrad");
  display.display();


START_INIT:

  if (CAN_OK == CAN.begin(CAN_500KBPS))                  // init can bus : baudrate = 500k
  {
  }
  else
  {
    delay(100);
    goto START_INIT;
  }
  delay(500);

}


float Sperrgrad_Soll;
float m;



void loop()
{
  // 20 ms CAN Interval
  delay(20);

  // Lese Poti am analogen Eingang
  Sperrgrad_Soll = analogRead(A1);
  // Die ersten 20% werden ignoriert, mappe daher auf 20 - 100%
  Sperrgrad_Soll = map(Sperrgrad_Soll, 0, 1023, 200, 1023);
  // m ist nur für die Ausgabe auf dem OLED
  m = Sperrgrad_Soll;

  // Sperrgrad_Soll wird als Faktor benutzt, daher / 1023
  Sperrgrad_Soll = Sperrgrad_Soll / 1023;
  // Wenn das Haldex STG immer die selben Werte bekommt, geht es von einem Fehler aus, und öffnet das Ventil wieder
  // Daher kommt hier ein wenig Zufall dazu
  Sperrgrad_Soll = Sperrgrad_Soll + random(-0.02, 0.02);

  // Die CAN Telegramme
  // 0x280: Motor 1
  // 0x380: Motor 3
  // 0x488: Motor 6
  // 0x1A0: ABS/ESP 1
  // 0x4A0: ABS/ESP 2
  // Übersicht über die Telegramme: https://www.vwbuswiki.de/index.php?title=CAN_IDs
  // Verändert werden (u.a.): Motormoment und Raddrehzahlen


  // So "stumpf" diese Methode auch ist: zwischen Ist und Soll liegen maximal 3%, während die Endpunkte exakt sind. Es ist also beinahe linear und reicht zum test allemal
  
  unsigned char CAN_0_280[8] = {0, 240 * Sperrgrad_Soll, 32, 78 * Sperrgrad_Soll, 240 * Sperrgrad_Soll, 240 * Sperrgrad_Soll, 32, 240 * Sperrgrad_Soll};
  unsigned char CAN_0_380[8] = {0, 0, 250 * Sperrgrad_Soll, 0, 0, 0, 0, 254 * Sperrgrad_Soll};
  unsigned char CAN_0_488[8] = {0, 254 * Sperrgrad_Soll, 254 * Sperrgrad_Soll, 0, 0, 0, 0, 0};
  unsigned char CAN_0_1A0[8] = {0, 0, 0, 10, 0, 0, 0, 0};
  unsigned char CAN_0_4A0[8] = {254 * Sperrgrad_Soll, 10, 254 * Sperrgrad_Soll, 10, 0, 10, 0, 10};


  // Sende Telegramme
  CAN.sendMsgBuf(0x280, 0, 8, CAN_0_280);
  CAN.sendMsgBuf(0x380, 0, 8, CAN_0_380);
  CAN.sendMsgBuf(0x488, 0, 8, CAN_0_488);
  CAN.sendMsgBuf(0x1A0, 0, 8, CAN_0_1A0);
  CAN.sendMsgBuf(0x4A0, 0, 8, CAN_0_4A0);


  // Prüfe, ob Telegram erhalten
  if (CAN_MSGAVAIL == CAN.checkReceive())
  {

    // Empfange Telegramme vom Haldex Steuergerät
    CAN.readMsgBuf(&len, buf);

    // Der aktuelle Sperrgrad liegt im Byte 1 der Antwort
    int Sperrgrad = 100 * (buf[1] & B01111111) / 72;
    
    // Ausgabe auf OLED Display
    display.fillRect(0, 17, 150, 17, BLACK);
    display.setCursor(0, 17);
    display.print("Ist: ");
    display.print(Sperrgrad);
    display.print("%");

    display.fillRect(0, 34, 150, 34, BLACK);
    display.setCursor(0, 34);
    display.print("Soll: ");
    display.print(map(m, 200, 1023, 0, 100));
    display.print("%");
    display.display();
  }
}
