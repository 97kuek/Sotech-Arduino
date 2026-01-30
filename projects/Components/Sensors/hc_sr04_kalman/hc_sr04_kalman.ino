
// SimpleKalmanFilter - HTTP://github.com/denyssene/SimpleKalmanFilter
// 1ファイルで動作するように簡易実装したカルマンフィルタクラス

class SimpleKalmanFilter {
public:
  /*
   * コンストラクタ
   * mea_e: 測定の不確かさ（測定ノイズの大きさ）
   * est_e: 推定の不確かさ（初期値）
   * q: プロセスノイズ（値の変動しやすさ。小さいと滑らかに、大きいと急な変化に追従しやすい）
   */
  SimpleKalmanFilter(float mea_e, float est_e, float q);
  
  // 新しい値を入力してフィルタリングされた値を返す
  float updateEstimate(float mea);
  
  // パラメータ設定用関数
  void setMeasurementError(float mea_e);
  void setEstimateError(float est_e);
  void setProcessNoise(float q);
  float getKalmanGain();
  float getEstimateError();

private:
  float _err_measure;
  float _err_estimate;
  float _q;
  float _current_estimate = 0;
  float _last_estimate = 0;
  float _kalman_gain = 0;
};

SimpleKalmanFilter::SimpleKalmanFilter(float mea_e, float est_e, float q) {
  _err_measure = mea_e;
  _err_estimate = est_e;
  _q = q;
}

float SimpleKalmanFilter::updateEstimate(float mea) {
  // カルマンゲインの計算
  _kalman_gain = _err_estimate / (_err_estimate + _err_measure);
  
  // 現在の値を推定（前回の推定値 + ゲイン * (測定値 - 前回の推定値)）
  _current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
  
  // 推定誤差の更新
  _err_estimate =  (1.0 - _kalman_gain) * _err_estimate + abs(_last_estimate - _current_estimate) * _q;
  
  // 次回のために現在の推定値を保存
  _last_estimate = _current_estimate;

  return _current_estimate;
}

void SimpleKalmanFilter::setMeasurementError(float mea_e) {
  _err_measure = mea_e;
}

void SimpleKalmanFilter::setEstimateError(float est_e) {
  _err_estimate = est_e;
}

void SimpleKalmanFilter::setProcessNoise(float q) {
  _q = q;
}

float SimpleKalmanFilter::getKalmanGain() {
  return _kalman_gain;
}

float SimpleKalmanFilter::getEstimateError() {
  return _err_estimate;
}

// --- メインスケッチ ---

/*
 * HC-SR04 カルマンフィルタ搭載版
 * 出力: CSV形式 (時間, 生の距離, フィルタ後の距離)
 */

// カルマンフィルタのインスタンス作成
// 引数: (測定ノイズ, 推定ノイズ, プロセスノイズ)
// プロセスノイズ(0.01)を小さくするとより滑らかになりますが、反応が遅れます。
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);

const int TRIG_PIN = 10;
const int ECHO_PIN = 9;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // 直前にノイズなどでHIGHになっていた場合の修正
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // 超音波の発射（10µ秒のHIGH）
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // エコーが戻ってくるまでの時間を計測（最大30ms待機）
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  float distance = 0;
  float filteredDistance = 0;

  if (duration == 0) {
     // 計測失敗時は -1 を出力してグラフ上でわかるようにする
     Serial.print(millis());
     Serial.print(",");
     Serial.print("-1");
     Serial.print(",");
     Serial.println("-1");
  } else {
    // 距離の計算 (cm)
    distance = (duration * 0.034) / 2;
    
    // 異常値（500cm以上）の除外
    if (distance > 500) {
       Serial.print(millis());
       Serial.print(",");
       Serial.print("-1");
       Serial.print(",");
       Serial.println("-1");
    } else {
      // カルマンフィルタを通して値を滑らかにする
      filteredDistance = simpleKalmanFilter.updateEstimate(distance);

      // CSV出力: 時間(ms), 生の距離(cm), フィルタ後の距離(cm)
      Serial.print(millis());
      Serial.print(",");
      Serial.print(distance);
      Serial.print(",");
      Serial.println(filteredDistance);
    }
  }

  // 次の測定まで少し待つ
  delay(100);
}
