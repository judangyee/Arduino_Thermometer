#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define RT0 10000   // 서미스터의 정격 저항 (25°C에서 10kΩ)
#define B 3977      // 서미스터의 B 계수 (K)
#define VCC 5.0     // 공급 전압 (V)
#define R 1000      // 시리즈 저항 (1kΩ)

#define SCREEN_WIDTH 128  // OLED 디스플레이 폭
#define SCREEN_HEIGHT 64  // OLED 디스플레이 높이
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

float RT, VR, ln, Temp, T0, Read;

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  T0 = 25 + 273.15;  // T0를 섭씨에서 켈빈으로 변환

  // OLED 초기화
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  Read = analogRead(A0);  // 아날로그 값을 읽음
  float Vout = (5.0 / 1023.0) * Read;  // 전압으로 변환
  VR = VCC - Vout;
  RT = (Vout * R) / VR;  // 서미스터의 저항값 계산

  ln = log(RT / RT0);
  Temp = (1 / ((ln / B) + (1 / T0))); // 서미스터의 온도 계산

  Temp = Temp - 273.15;  // 켈빈을 섭씨로 변환

  // 시리얼 모니터에 출력
  Serial.print("Temperature: ");
  Serial.print(Temp);
  Serial.println(" C");

  // OLED 디스플레이에 출력
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Temperature:");
  display.setCursor(0, 10);
  display.print(Temp);
  display.print(" C");
  display.display();

  delay(1000);
}
