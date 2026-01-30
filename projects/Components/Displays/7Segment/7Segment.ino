// セグメントピンの定義（a, b, c, d, e, f, g の順）
const int segmentPins[] = {2, 3, 4, 5, 6, 7, 8};
const int buttonPin = 9;

// 各数字の点灯パターン
// ビット配置: a, b, c, d, e, f, g の順 (左から)
const byte digits[10] = {
  0b1111110, // 0
  0b0110000, // 1
  0b1101101, // 2
  0b1111001, // 3
  0b0110011, // 4
  0b1011011, // 5
  0b1011111, // 6
  0b1110000, // 7
  0b1111111, // 8
  0b1111011  // 9
};

void setup() {
  // 出力ピンの設定
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  
  // 入力ピンの設定（内部プルアップ）
  pinMode(buttonPin, INPUT_PULLUP);

  // 乱数シードの初期化
  randomSeed(analogRead(0));
}

// 数字を1つ表示する関数
void displayDigit(int num) {
  for (int i = 0; i < 7; i++) {
    // 6-iビット目を読み取り、各ピンに出力
    bool bit = bitRead(digits[num], 6 - i);
    digitalWrite(segmentPins[i], bit);
  }
}

void loop() {
  // ボタンが押された（LOW）か判定
  if (digitalRead(buttonPin) == LOW) {
    
    // 高速でシャッフル（徐々にスピードダウン）
    for (int i = 0; i < 15; i++) {
      displayDigit(random(0, 10));
      delay(30 + i * 10); 
    }

    // 乱数を表示
    int result = random(0, 10);
    displayDigit(result);

    // チャタリング防止
    while (digitalRead(buttonPin) == LOW);
    delay(50); // デバウンス処理
  }
}