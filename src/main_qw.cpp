/*
 * ============================================
 *   АРДУИНО ЦВЕТОМУЗЫКА
 *   Анализ частот и управление LED лентами
 * ============================================
 * 
 * Описание:
 *   Скетч анализирует входящий аудиосигнал
 *   с помощью FFT и разделяет спектр на
 *   6 частотных диапазонов. Каждый диапазон
 *   управляет своей светодиодной лентой.
 * 
 * Компоненты:
 *   - Arduino Uno / Nano
 *   - Микрофонный модуль (MAX9812 / MAX4466)
 *   - 6x MOSFET транзисторов (IRLZ44N / IRF520)
 *   - 6x LED лент (12V)
 *   - Блок питания 12V
 * 
 * Подключение:
 *   A0 - аналоговый вход с микрофона
 *   D3  - Bass (20-60 Hz)     - Красная лента
 *   D5  - Low-Mid (60-250 Hz) - Оранжевая лента
 *   D6  - Mid (250-500 Hz)    - Жёлтая лента
 *   D9  - Upper-Mid (500-2kHz)- Зелёная лента
 *   D10 - High (2k-4kHz)      - Синяя лента
 *   D11 - Ultra-High (4k-8kHz)- Фиолетовая лента
 * 
 * Библиотека:
 *   ArduinoFFT - https://github.com/kosme/arduinoFFT
 */
#include <arduinoFFT.h>
// ========== НАСТРОЙКИ ==========
#define SAMPLES         128        // Количество сэмплов FFT (степень 2)
#define SAMPLING_FREQ   8000       // Частота дискретизации (Гц)
#define ANALOG_PIN      A0         // Пин микрофона
#define NOISE_GATE      50         // Порог шумоподавления
// Частотные диапазоны (индексы бинов FFT)
// Бин = частота / (SAMPLING_FREQ / SAMPLES)
// Разрешение по частоте = 8000/128 = 62.5 Гц/бин
#define BASS_MIN        0          // 20-60 Hz
#define BASS_MAX        1
#define LOWMID_MIN      1          // 60-250 Hz
#define LOWMID_MAX      4
#define MID_MIN         4          // 250-500 Hz
#define MID_MAX         8
#define UPMID_MIN       8          // 500-2000 Hz
#define UPMID_MAX       32
#define HIGH_MIN        32         // 2000-4000 Hz
#define HIGH_MAX        64
#define ULTRA_MIN       64         // 4000-8000 Hz
#define ULTRA_MAX       127
// PWM выходы для LED лент
#define LED_BASS        3          // Bass - Красный
#define LED_LOWMID      5          // Low-Mid - Оранжевый
#define LED_MID         6          // Mid - Жёлтый
#define LED_UPMID       9          // Upper-Mid - Зелёный
#define LED_HIGH        10         // High - Синий
#define LED_ULTRA       11         // Ultra-High - Фиолетовый
// Параметры сглаживания
#define SMOOTHING       0.7        // Коэффициент сглаживания (0-1)
#define DECAY_RATE      3          // Скорость затухания
// ========== ПЕРЕМЕННЫЕ ==========
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long samplingPeriod;
unsigned long startTime;
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQ);
// Текущие и целевые значения яркости
uint8_t currentBrightness[6] = {0};
uint8_t targetBrightness[6] = {0};
// Пины LED
const uint8_t ledPins[6] = {
  LED_BASS, LED_LOWMID, LED_MID,
  LED_UPMID, LED_HIGH, LED_ULTRA
};
// Границы диапазонов
const uint8_t bandMin[6] = {
  BASS_MIN, LOWMID_MIN, MID_MIN,
  UPMID_MIN, HIGH_MIN, ULTRA_MIN
};
const uint8_t bandMax[6] = {
  BASS_MAX, LOWMID_MAX, MID_MAX,
  UPMID_MAX, HIGH_MAX, ULTRA_MAX
};
// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  
  // Настройка PWM выходов
  for (int i = 0; i < 6; i++) {
    pinMode(ledPins[i], OUTPUT);
    analogWrite(ledPins[i], 0);
  }
  
  // Настройка аналогового входа
  pinMode(ANALOG_PIN, INPUT);
  
  // Расчёт периода дискретизации
  samplingPeriod = round(1000000.0 / SAMPLING_FREQ);
  
  Serial.println(F("=== Цветомузыка запущена ==="));
  Serial.print(F("Разрешение FFT: "));
  Serial.print((float)SAMPLING_FREQ / SAMPLES);
  Serial.println(F(" Hz/bin"));
}
// ========== ОСНОВНОЙ ЦИКЛ ==========
void loop() {
  // 1. Считываем сэмплы
  sampleAudio();
  
  // 2. Выполняем FFT
  performFFT();
  
  // 3. Вычисляем энергию каждого диапазона
  calculateBands();
  
  // 4. Сглаживаем и обновляем яркость LED
  updateLEDs();
  
  // 5. Вывод отладки (раскомментировать при необходимости)
  // printDebug();
}
// ========== ФУНКЦИИ ==========
// Считывание аудиосэмплов
void sampleAudio() {
  for (int i = 0; i < SAMPLES; i++) {
    startTime = micros();
    
    vReal[i] = analogRead(ANALOG_PIN);
    vImag[i] = 0.0;
    
    // Точная задержка для равномерной дискретизации
    while (micros() - startTime < samplingPeriod) {
      // ждём
    }
  }
}
// Выполнение FFT
void performFFT() {
  // Оконная функция (Hann) для уменьшения утечек
  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  
  // Вычисление FFT
  FFT.compute(FFT_FORWARD);
  
  // Вычисление магнитуд
  FFT.complexToMagnitude();
}
// Вычисление энергии каждого частотного диапазона
void calculateBands() {
  for (int band = 0; band < 6; band++) {
    double sum = 0;
    int count = 0;
    
    // Суммируем энергию бинов в диапазоне
    for (int bin = bandMin[band]; bin <= bandMax[band]; bin++) {
      sum += vReal[bin];
      count++;
    }
    
    // Средняя энергия в диапазоне
    double avg = (count > 0) ? sum / count : 0;
    
    // Шумоподавление
    if (avg < NOISE_GATE) {
      targetBrightness[band] = 0;
    } else {
      // Масштабирование в диапазон 0-255
      double scaled = avg / 4.0;  // Коэффициент усиления
      if (scaled > 255) scaled = 255;
      targetBrightness[band] = (uint8_t)scaled;
    }
  }
}
// Обновление яркости LED с плавным переходом
void updateLEDs() {
  for (int i = 0; i < 6; i++) {
    // Плавное приближение к целевому значению
    if (currentBrightness[i] < targetBrightness[i]) {
      // Нарастание (быстрее)
      currentBrightness[i] = currentBrightness[i] + 
        (targetBrightness[i] - currentBrightness[i]) * (1.0 - SMOOTHING * 0.5);
    } else if (currentBrightness[i] > targetBrightness[i]) {
      // Затухание (медленнее)
      if (currentBrightness[i] > DECAY_RATE) {
        currentBrightness[i] -= DECAY_RATE;
      } else {
        currentBrightness[i] = 0;
      }
    }
    
    // Обновляем PWM
    analogWrite(ledPins[i], currentBrightness[i]);
  }
}
// Отладочный вывод
void printDebug() {
  Serial.print(F("Bass:"));
  Serial.print(currentBrightness[0]);
  Serial.print(F(" LowMid:"));
  Serial.print(currentBrightness[1]);
  Serial.print(F(" Mid:"));
  Serial.print(currentBrightness[2]);
  Serial.print(F(" UpMid:"));
  Serial.print(currentBrightness[3]);
  Serial.print(F(" High:"));
  Serial.print(currentBrightness[4]);
  Serial.print(F(" Ultra:"));
  Serial.println(currentBrightness[5]);
}
