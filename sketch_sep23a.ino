#define PIN_LED 7

void setup() {
  // GPIO 7번 핀을 출력 모드로 설정
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  // 처음 1초 동안 LED 켜기
  digitalWrite(PIN_LED, 0);  // LED 켜기
  delay(1000);                  // 1초 대기

  // 다음 1초 동안 LED 5회 깜빡이기
  for (int i = 0; i < 6; i++) {
    digitalWrite(PIN_LED, LOW);   // LED 끄기
    delay(100);                   // 0.1초 대기
    digitalWrite(PIN_LED, HIGH);  // LED 켜기
    delay(100);                   // 0.1초 대기
  }

  // LED 끄고 무한 루프에 진입
  digitalWrite(PIN_LED, HIGH);   // LED 끄기
  while (1) {
    // 무한 루프
  }
}
