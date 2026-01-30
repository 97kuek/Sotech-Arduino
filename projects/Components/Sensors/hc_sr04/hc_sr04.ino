const int TRIG_PIN = 10;
const int ECHO_PIN = 9;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // 直前にノイズなどでHIGHになっていた場合の修正（ピンをLOWにしてリセット）
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // 10µ秒以上のHIGH信号をTRIGピンに送ることで、超音波を発射させる
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // ECHOピンがHIGHになるまでの時間（往復時間）を計測する
  // 30000マイクロ秒（30ms）経っても戻ってこない場合はタイムアウト（0になる）
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    // 計測失敗（タイムアウト）の場合
    // グラフ描画時に異常値だとわかるように -1 を出力しておく（必要に応じて変更可）
    Serial.println("-1");
  } else {
    // 音速は 340m/s = 0.034cm/µs
    // 距離 = 時間 × 速さ ÷ 2 （往復なので2で割る）
    float distance = (duration * 0.034) / 2;

    // 距離が500cmを超えた場合は測定範囲外として扱う（HC-SR04の仕様は約400cmまで）
    if (distance > 500) {
      Serial.println("-1");
    } else {
      // CSV形式で出力：時間(ミリ秒), 距離(cm)
      Serial.print(millis());
      Serial.print(",");
      Serial.println(distance);
    }
  }

  // 測定間隔（100ms待機）
  delay(100);
}
