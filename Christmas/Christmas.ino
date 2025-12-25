// FastLEDライブラリをインクルード
#include <FastLED.h>

// --- 基本設定 ---
#define DATA_PIN 6      // ピン番号
#define NUM_LEDS 300    // LEDの数
#define BRIGHTNESS 64  // 最大の明るさ

// --- 色の設定（クリスマスカラー4色） ---
// ちょうど4つの光があるので、4色を循環させます
const CRGB COMMON_COL[] = {
    CRGB::Red,      // 0: 赤
    CRGB::Green,    // 1: 緑
    CRGB::Gold,     // 2: 金
    CRGB::Snow      // 3: 白
};
const int COMMON_COL_SIZE = sizeof(COMMON_COL) / sizeof(COMMON_COL[0]);

// 色が変わる時間（2秒ごとに色が隣へ移動）
#define CHANGETIME 2000

// --- グローバル変数 ---
CRGB leds[NUM_LEDS];

// 光の中心位置（300個を4等分）
int pos1 = 38;
int pos2 = 113;
int pos3 = 188;
int pos4 = 263;

// 光の広がる範囲
#define LIGHT_RADIUS 65 

// 色管理用の変数（初期値をズラしています）
unsigned long lastColorChangeTime = 0;
// ここで +1, +2, +3 することで、最初から全員違う色になります
int colorIndexOffset1 = 0;
int colorIndexOffset2 = 1;
int colorIndexOffset3 = 2;
int colorIndexOffset4 = 3;

// --- 初期設定 ---
void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

// --- メインループ ---
void loop() {
  unsigned long currentMillis = millis();
  FastLED.clear(); // 毎回リセット

  // === 1. 時間で色を更新 ===
  // 一定時間ごとに、全体の色の基準（ベース）を1つ進めます
  // これにより、色が「赤→緑→金→白」と順番に回っていきます
  int baseIndex = (currentMillis / CHANGETIME) % COMMON_COL_SIZE;

  // === 2. 描画処理 ===

  // --- エリア1 ---
  // 色の決定: (基準の時間 + 個別のズレ) を色の数で割った余り
  int idx1 = (baseIndex + colorIndexOffset1) % COMMON_COL_SIZE;
  // 点滅: BPM 40（ゆっくりめ）
  int b1 = beatsin8(60, 40, 255); 
  
  for (int i = 0; i < NUM_LEDS; i++) {
    int dist = abs(i - pos1);
    if (dist < LIGHT_RADIUS) {
      CRGB c = COMMON_COL[idx1];
      int falloff = map(dist, 0, LIGHT_RADIUS, 255, 0); // 中心ほど明るく
      c.nscale8(falloff);
      c.nscale8(b1); // 点滅適用
      leds[i] += c;
    }
  }

  // --- エリア2 ---
  int idx2 = (baseIndex + colorIndexOffset2) % COMMON_COL_SIZE;
  // 点滅: BPM 55（少し速い）
  int b2 = beatsin8(60, 40, 255);

  for (int i = 0; i < NUM_LEDS; i++) {
    int dist = abs(i - pos2);
    if (dist < LIGHT_RADIUS) {
      CRGB c = COMMON_COL[idx2];
      int falloff = map(dist, 0, LIGHT_RADIUS, 255, 0);
      c.nscale8(falloff);
      c.nscale8(b2);
      leds[i] += c;
    }
  }

  // --- エリア3 ---
  int idx3 = (baseIndex + colorIndexOffset3) % COMMON_COL_SIZE;
  // 点滅: BPM 45（普通）
  int b3 = beatsin8(60, 40, 255);

  for (int i = 0; i < NUM_LEDS; i++) {
    int dist = abs(i - pos3);
    if (dist < LIGHT_RADIUS) {
      CRGB c = COMMON_COL[idx3];
      int falloff = map(dist, 0, LIGHT_RADIUS, 255, 0);
      c.nscale8(falloff);
      c.nscale8(b3);
      leds[i] += c;
    }
  }

  // --- エリア4 ---
  int idx4 = (baseIndex + colorIndexOffset4) % COMMON_COL_SIZE;
  // 点滅: BPM 60（速い）
  int b4 = beatsin8(60, 40, 255);

  for (int i = 0; i < NUM_LEDS; i++) {
    int dist = abs(i - pos4);
    if (dist < LIGHT_RADIUS) {
      CRGB c = COMMON_COL[idx4];
      int falloff = map(dist, 0, LIGHT_RADIUS, 255, 0);
      c.nscale8(falloff);
      c.nscale8(b4);
      leds[i] += c;
    }
  }

  FastLED.show();
  delay(10);
}