// FastLEDライブラリをインクルード
#include <FastLED.h>

// --- 基本設定 ---
#define DATA_PIN 6      // ピン番号
#define NUM_LEDS 300    // LEDの数
#define BRIGHTNESS 64   // 明るさ (0-255)

// --- 流れる光の設定（クリスマスカラー：赤、緑、金、雪） ---
const CRGB COMMON_COL[] = {
    CRGB::Red,      // 赤
    CRGB::Green,    // 緑
    CRGB::Gold,     // 金
    CRGB::Snow,     // 雪（白）
    CRGB::Red,      // 赤（繰り返し）
    CRGB::Green     // 緑（繰り返し）
};
const int COMMON_COL_SIZE = sizeof(COMMON_COL) / sizeof(COMMON_COL[0]);

// 各光の設定
#define CHANGETIME1 1000
#define FLOW_SPEED1 20
#define CHANGETIME2 1000
#define FLOW_SPEED2 60
#define CHANGETIME3 1000
#define FLOW_SPEED3 100
#define CHANGETIME4 1000
#define FLOW_SPEED4 150

// --- 背景の設定（ツリーや飾りを意識した色） ---
#define BG_CHANGETIME 10000
const CRGB BG_COL[] = {
    CRGB::DarkGreen,      // 濃い緑（モミの木イメージ）
    CRGB::Maroon,         // 茶色っぽい赤
    CRGB::Gold,           // 金色
    CRGB::DarkOliveGreen, // 深いオリーブ色
    CRGB::Red,            // 赤
    CRGB::Silver          // 銀色
};
const int BG_COL_SIZE = sizeof(BG_COL) / sizeof(BG_COL[0]);

// --- グローバル変数 ---
CRGB leds[NUM_LEDS];

// 流れる光用の変数
unsigned long lastColorChangeTime1 = 0, lastFlowTime1 = 0;
int currentColorIndex1 = 0, flowPosition1 = 0;
unsigned long lastColorChangeTime2 = 0, lastFlowTime2 = 0;
int currentColorIndex2 = 0, flowPosition2 = 0;
unsigned long lastColorChangeTime3 = 0, lastFlowTime3 = 0;
int currentColorIndex3 = 0, flowPosition3 = 0;
unsigned long lastColorChangeTime4 = 0, lastFlowTime4 = 0;
int currentColorIndex4 = 0, flowPosition4 = 0;

// 背景アニメーション用の変数
char bg_br[NUM_LEDS];
char bg_br_delta[NUM_LEDS];
unsigned long bg_last_change_time = 0;
int bg_current_index = 0;

// --- 初期設定 ---
void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  // 背景アニメーションの初期化
  for (int i = 0; i < NUM_LEDS; i++) {
    bg_br[i] = random(BRIGHTNESS);
    bg_br_delta[i] = 1;
  }
}

// --- メインループ ---
void loop() {
  // === 状態更新 ===
  // 流れる光の状態更新
  if (millis() - lastColorChangeTime1 > CHANGETIME1) { lastColorChangeTime1 = millis(); currentColorIndex1 = (currentColorIndex1 + 1) % COMMON_COL_SIZE; }
  if (millis() - lastFlowTime1 > FLOW_SPEED1) { lastFlowTime1 = millis(); flowPosition1 = (flowPosition1 + 1) % NUM_LEDS; }

  if (millis() - lastColorChangeTime2 > CHANGETIME2) { lastColorChangeTime2 = millis(); currentColorIndex2 = (currentColorIndex2 + 1) % COMMON_COL_SIZE; }
  if (millis() - lastFlowTime2 > FLOW_SPEED2) { lastFlowTime2 = millis(); flowPosition2 = (flowPosition2 + 1) % NUM_LEDS; }

  if (millis() - lastColorChangeTime3 > CHANGETIME3) { lastColorChangeTime3 = millis(); currentColorIndex3 = (currentColorIndex3 + 1) % COMMON_COL_SIZE; }
  if (millis() - lastFlowTime3 > FLOW_SPEED3) { lastFlowTime3 = millis(); flowPosition3 = (flowPosition3 + 1) % NUM_LEDS; }

  if (millis() - lastColorChangeTime4 > CHANGETIME4) { lastColorChangeTime4 = millis(); currentColorIndex4 = (currentColorIndex4 + 1) % COMMON_COL_SIZE; }
  if (millis() - lastFlowTime4 > FLOW_SPEED4) { lastFlowTime4 = millis(); flowPosition4 = (flowPosition4 + 1) % NUM_LEDS; }

  // 背景色の状態更新
  if (millis() - bg_last_change_time > BG_CHANGETIME) {
    bg_last_change_time = millis();
    bg_current_index = (bg_current_index + 1) % BG_COL_SIZE;
  }
  // 背景色の取得（HSV変換して輝度調整しやすくする）
  CHSV hsvBgColor = rgb2hsv_approximate(BG_COL[bg_current_index]);

  // --- LEDの描画処理 ---
  for (int i = 0; i < NUM_LEDS; i++) {
    // 1. 背景のキラキラを描画
    bg_br[i] += bg_br_delta[i];
    if (bg_br_delta[i] > 0 && bg_br[i] >= BRIGHTNESS - 1) {
      bg_br_delta[i] = -1;
    } else if (bg_br_delta[i] < 0 && bg_br[i] <= 20) { // 下限を少し下げて明暗をはっきりさせる
      bg_br_delta[i] = 1;
    }
    CRGB bgColor = CHSV(hsvBgColor.hue, hsvBgColor.sat, bg_br[i]);

    // 2. 流れる光の色を計算
    CRGB cometColor = CRGB::Black;
    
    // 光1
    int dist1 = (i - flowPosition1 + NUM_LEDS) % NUM_LEDS;
    if (dist1 < 20) {
      int bright = map(dist1, 0, 4, 255, 50);
      CRGB c = COMMON_COL[currentColorIndex1];
      c.nscale8(bright);
      cometColor += c;
    }
    // 光2
    int dist2 = (i - flowPosition2 + NUM_LEDS) % NUM_LEDS;
    if (dist2 < 20) {
      int bright = map(dist2, 0, 4, 255, 50);
      CRGB c = COMMON_COL[currentColorIndex2];
      c.nscale8(bright);
      cometColor += c;
    }
    // 光3
    int dist3 = (i - flowPosition3 + NUM_LEDS) % NUM_LEDS;
    if (dist3 < 20) {
      int bright = map(dist3, 0, 4, 255, 50);
      CRGB c = COMMON_COL[currentColorIndex3];
      c.nscale8(bright);
      cometColor += c;
    }
    // 光4
    int dist4 = (i - flowPosition4 + NUM_LEDS) % NUM_LEDS;
    if (dist4 < 20) {
      int bright = map(dist4, 0, 4, 255, 50);
      CRGB c = COMMON_COL[currentColorIndex4];
      c.nscale8(bright);
      cometColor += c;
    }

    // 3. 背景色と光の色を合成
    leds[i] = bgColor + cometColor;
  }

  FastLED.show();
  delay(10);
}