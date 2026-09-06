#include <Arduino.h>

// включаем функцию для логарифмического формата
#define LOG_OUT           1
// задаем количество выходных отсчетов
#define FFT_N             128
// подключаем библиотеку FFT — быстрое преобразование Фурье
#include <FFT.h>
 
// номер пина датчика микрофона
#define MIC_PIN           A5
// номера цифровых пинов светодиодов
#define RED_LED           3
#define BLUE_LED          4
#define GREEN_LED         9
#define YELLOW_LED        10
 
// распределяем номера отчётов по частотам
// весь диипазон (FFT_N / 2)
#define FREQ_LOW_FFT      2
#define FREQ_MIDDLE_FFT   30
#define FREQ_HIGH_FFT     60
 
#define FREQ_LOW_LEVEL    38
#define FREQ_MIDDLE_LEVEL 18
#define FREQ_HIGH_LEVEL   15 
 
void setup()
{
  // открываем последовательный порт
  Serial.begin(115200);
  // назначаем пины светодиодов в режим выхода
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED , OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
}
 
void loop()
{
  // считываем заданное количество отсчётов
  for (int i = 0 ; i < FFT_N; i++) {
    // считываем показания микрофона и вычитаем отрицательную полу-волну
    int sample = analogRead(MIC_PIN) - 511;
    // игнорируем помехи АЦП
    if(sample < 5 && sample > -5) {
      sample = 0;
    }
    // сохраняем действительные значения в четные отсчеты
    fft_input[i++] = sample;
    // задаем нечетным отсчетам значение «0»
    fft_input[i] = 0;
  }
  // функция-окно, повышающая частотное разрешение
  fft_window();
  // реорганизовываем данные перед запуском FFT
  fft_reorder();
  // обрабатываем данные в FFT
  fft_run();
  // извлекаем данные, обработанные FFT
  fft_mag_log();
  // выводим значения преобразования Фурье
  Serial.print("<");
  Serial.print(FFT_N / 2);
  Serial.print(":");
  for (int curBin = 0; curBin < FFT_N / 2; ++curBin) {
    if (curBin > 0) {
      Serial.print(",");
    }
    Serial.print(fft_log_out[curBin]);
  }
  // если значение низких частот превысило предел
  if (fft_log_out[FREQ_LOW_FFT] > FREQ_LOW_LEVEL) {
    // зажигаем красный светодиод
    digitalWrite(RED_LED, HIGH);
  } else {
    // гасим красный светодиод
    digitalWrite(RED_LED, LOW);
  }
  // если значение средних частот превысило предел
  if (fft_log_out[FREQ_MIDDLE_FFT] > FREQ_MIDDLE_LEVEL) {
    // зажигаем зелёный светодиод
    digitalWrite(GREEN_LED, HIGH);
    // гасим синий светодиод (фон средних частот)
    digitalWrite(BLUE_LED, LOW);
  } else {
    // гасим зелёный светодиод
    digitalWrite(GREEN_LED, LOW);
    // зажигаем синий светодиод (фон средних частот)
    digitalWrite(BLUE_LED, HIGH);
  }
  // если значение высоких частот превысило предел
  if (fft_log_out[FREQ_HIGH_FFT] > FREQ_HIGH_LEVEL) {
    // зажигаем жёлтый светодиод
    digitalWrite(YELLOW_LED, HIGH);
  } else {
    // зажигаем жёлтый светодиод
    digitalWrite(YELLOW_LED, LOW);
  }
  Serial.println(">");
}