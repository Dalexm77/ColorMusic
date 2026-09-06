/****************************************************************
 Этот код написан и принадлежит компании duino.ru
 При использование кода указание авторства обязательно
****************************************************************/

//*********************радуга************************************
void rainbow() 
{
 while(true) 
 { 
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) 
  {
    for(uint8_t i=0; i<N_LEDS; i++) 
    { 
      int pixelHue = firstPixelHue + (i * 65536L / N_LEDS);
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); 
    button_pattern();
    if (pattern != PATTERN_RAINBOW) return; 
  }
 } 
}

//******************снег искрится***********************************
void sparkle()
{
  #define min_light 5
   strip.clear();
  while(true) 
   { 
      uint8_t j = random(N_LEDS); //рандомная позиция пикселя
      strip.setPixelColor(j, 255,255,255); //максимально возможная яркость случайного пикселя
      strip.show();
      delay(random(20));
      strip.setPixelColor(j, 0,0,0); //гасим пиксель на уровень яркости всей ленты
      strip.show();
     // delay(random(100, 400));
      
     button_pattern();
    if (pattern != PATTERN_SPARKLE) return; 
   }  
} 
 
//*******************рандомные цветные пиксели***************************************
void col_random() 
{
  while(true) 
  {
    byte r=random(2); if(r) r=255;
    byte g=random(2); if(g) g=255;
    byte b=random(2); if(b) b=255;
    if(r==0 && g==0 && b==0) b=128;
     strip.setPixelColor(random(N_LEDS),r,g,b);
     strip.setPixelColor(random(N_LEDS),0,0,0);
     strip.show(); 

     button_pattern();
    if (pattern != PATTERN_COL_RANDOM) return;  
  }
}

//********************шарик туда сюда*************************************
void CylonBounce()
{
  while(true)
 {
    byte red=random(2); if(red) red=255;
    byte green=random(2); if(green) green=255;
    byte blue=random(2); if(blue) blue=255;
    if(red==0 && green==0 && blue==0) {blue=255; red=255;}
    
  for(uint8_t i = 0; i < N_LEDS-1-2; i++) 
  {
    strip.clear();
    strip.setPixelColor(i, red/10, green/10, blue/10);//боковой пиксель
      strip.setPixelColor(i+1, red, green, blue); //основные пиксели
    strip.setPixelColor(i+2, red/10, green/10, blue/10); //боковой пиксель
    strip.show();
    delay(30);
    
    button_pattern();
  if (pattern != PATTERN_CYLON) return; 
  }

    red=random(2); if(red) red=255;
    green=random(2); if(green) green=255;
    blue=random(2); if(blue) blue=255;
    if(red==0 && green==0 && blue==0) {green=100; red=255;}
    
  delay(30);
  for(uint8_t i = N_LEDS-1-2; i > 0; i--) 
  {
    strip.clear(); 
    strip.setPixelColor(i, red/10, green/10, blue/10);
    strip.setPixelColor(i+1, red, green, blue);
    strip.setPixelColor(i+2, red/10, green/10, blue/10);
    strip.show();
    delay(30);
    button_pattern();
  if (pattern != PATTERN_CYLON) return; 
  }

    button_pattern();
  if (pattern != PATTERN_CYLON) return; 
 }
}

//*****************изменяем яркость лампы*******************
void LampDimmer()
{
 /* byte old_val=30;
     strip.clear();
       
 while(true)
 { 
  byte val = map(analogRead(PARM_POT), 0, 1023, 0, 127);    
  if(val != old_val)
  {
   old_val = val;
    val = val*2;
    strip.clear();
   // led_clr();
      for(uint8_t i=N_LEDS/2 - N_LED_LAMP/2; i<N_LEDS/2 + N_LED_LAMP/2; i++)  //включить все светодиоды с установленной яркостью
       {
        strip.setPixelColor(i, val, val, val);
        strip.setPixelColor(N_LEDS/2 - N_LED_LAMP/2, old_val, old_val, old_val); // крайние пиксели немного темнее основных
        strip.setPixelColor(N_LEDS/2 + N_LED_LAMP/2, old_val, old_val, old_val);
       }
    strip.show(); 
   } */
      strip.clear();
      for(uint8_t i=N_LEDS/2 - N_LED_LAMP/2; i<N_LEDS/2 + N_LED_LAMP/2; i++)  //включить N_LED_LAMP количество светодиодов 
       {
        strip.setPixelColor(i, 255, 255, 255);
       }

   while(true)
   {
    strip.show();
    button_pattern();
    if (pattern != PATTERN_LAMP) return; 
   } 
} 

//*****************изменяем размер светильника*******************
 void LedBright()
{   
 while(true)
 { 
  byte val = map(lastParm, 0, 255, 0, N_LEDS/2);
    strip.clear();
     for(byte i=0;  i<val; i++)  //включить все светодиоды с установленной яркостью
       {
        strip.setPixelColor(N_LEDS/2+i,  255, 255, 255);
        strip.setPixelColor(N_LEDS/2-i, 255, 255, 255); 
       }
    strip.show(); 
  
   button_pattern();
  if (pattern != PATTERN_BRIGHT) return;  
 } 
}

//*****************Стробоскоп*****************************
void Strobe(uint8_t FlashDelay)
{   
   while(true)
    { 
     strip.setBrightness(255);
      led_on();     
        delay(FlashDelay);
      led_clr();
        delay(lastParm+20);
  
       button_pattern();
         if (pattern != PATTERN_STROBE) return; 
    }  
}

//*****************Полицейская мигалка*****************************
void Police_Flash(uint8_t FlashDelay, uint8_t DarkDelay, uint8_t num_flash)
{   

   while(true)
    { 
      for (uint8_t i = 0; i < num_flash; i++)
      {
        led_red(); 
        delay(FlashDelay);
        led_clr();
        delay(FlashDelay);
      }
      delay(DarkDelay);
      
     for (uint8_t i = 0; i < num_flash; i++)
      {
        led_blue(); 
        delay(FlashDelay);
        led_clr();
        delay(FlashDelay);
      }
      delay(DarkDelay);
      
       button_pattern();
         if (pattern != PATTERN_POLICE)  return; 
    }  
}

//*****************тлеющие угли***********************************
void flame()
{
 while(true)
 { 
   for(uint8_t i=0; i<N_LEDS; i++)
   {
    strip.setPixelColor(i, strip.Color(random(128, 255), random(64), 0));
   }
    strip.setBrightness(random(5,64));
    strip.show(); 

    delay(random(100,200));  
    button_pattern();
  if (pattern != PATTERN_FLAME) return;  
 }
}  
 
//****************огонь пламя****************************************
void fire(int Cooling, uint8_t Sparking, uint8_t SpeedDelay) // Высота пламени, искры, скрость
{
 while(true) 
 {
  static uint8_t heatar[NUM_LED];
  int cooldown;
 
  for( int i = 0; i < N_LEDS; i++)   // Step 1.  Cool down every cell a little
  {
    cooldown = random(0,(Cooling/N_LEDS) + 2);
   
    if(cooldown>heatar[i]) 
    {
      heatar[i]=0;
    } 
    else 
    {
      heatar[i]=heatar[i]-cooldown;
    }
  }
 
    for( int k= N_LEDS - 1; k >= 2; k--) // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    {
      heatar[k] = (heatar[k - 1] + heatar[k - 2] + heatar[k - 2]) / 3;
    }
   
     if( random(255) < Sparking ) // Step 3.  Randomly ignite new 'sparks' near the bottom
      {
       int y = random(7);
       heatar[y] = heatar[y] + random(160,255);
      //heat[y] = random(160,255);
      }

    for( int j = 0; j < N_LEDS; j++)   // Step 4.  Convert heat to LED colors
    {
     setPixelHeatColor(j, heatar[j] );
    }

  strip.show();
  delay(15);
 button_pattern();
 if (pattern != PATTERN_FIRE) return; 
 } 
} 

void setPixelHeatColor (int Pixel, byte temperature) 
{
  byte t192 = round((temperature/255.0)*191);  // Scale 'heat' down from 0-255 to 0-191
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  if( t192 > 0x80)  // figure out which third of the spectrum we're in:
  {                     
   strip.setPixelColor(Pixel, 255, 255, heatramp);// hottest
  } else if( t192 > 0x40 ) 
  {            
    strip.setPixelColor(Pixel, 255, heatramp, 0); // middle
  } 
  else 
  {                               
    strip.setPixelColor(Pixel, heatramp, 0, 0);// coolest
  }
}

//**************эффект матрицы*********************
void matrix(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay)
{  
 while(true) 
 {
  for(int i = N_LEDS-1; i >= -N_LEDS; i--) //ДЛИНА ТРАЕКТОРИИ МЕТЕОРИТА
  {
   
    for(int j=N_LEDS-1; j>=0; j--)  // ослабление яркости всех светодиодов за один шаг
    {
      if( (!meteorRandomDecay) || (random(10)>5) ) 
      {
        fadeToBlack(j, meteorTrailDecay );   //ослабление яркости выбраного светодиода
      }
    }
   
    for(int j = meteorSize-1; j >=0; j--)     // рисуем метеорит
    {
      if( ( i-j <N_LEDS) && (i-j>=0) ) 
      {
        strip.setPixelColor(i-j, red, green, blue);
      }
    }
   
    strip.show();
   // delay(SpeedDelay);
    button_pattern();
  if (pattern != PATTERN_MATRIX) return; 
  }
  //-------------------------------
    for(int i = 0; i < N_LEDS; i++ ) //затираем предыдущую картинку
    {
      strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show(); //
  //delay(random(500)); //пауза для запуска следующего метеорита
 } 
}

void fadeToBlack(int ledNo, byte fadeValue) //затухание заданного пикселя
{
    uint32_t oldColor;
    uint8_t r, g, b;
   
    oldColor = strip.getPixelColor(ledNo);// получаем цвет заданного пикселя  
    r = (oldColor & 0x00ff0000UL) >> 16;  // извлекаем
    g = (oldColor & 0x0000ff00UL) >> 8;   // 3 байта цвета
    b = (oldColor & 0x000000ffUL);        // из переменной лонг

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    strip.setPixelColor(ledNo, r,g,b);
}

//***************лава лампа************************
void bouncing_balls(int BallCount, byte colors[][3]) 
{
while(true)
 {
  float Gravity = -0.81; //-0.1; -9.81; //гравитация макс 0.99 мин 0.01
  int StartHeight = 1;//начальная позиция
 
  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];//скорость удара
  float TimeSinceLastBounce[BallCount]; //время после последнего удара
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];//демпфирование
 
  for (int i = 0 ; i < BallCount ; i++) 
  {  
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight; 
    Position[i] = 0;
    ImpactVelocity[i] = ImpactVelocityStart; 
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.99 - float(i)/pow(BallCount,2);//0.90 
  }

  while (true) 
  {
    for (int i = 0 ; i < BallCount ; i++) 
    {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
 
      if ( Height[i] < 0 ) 
      {                      
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis(); 
 
        if ( ImpactVelocity[i] < 0.01 ) 
        {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (N_LEDS - 1) / StartHeight);
    }
 
    for (int i = 0 ; i < BallCount ; i++) 
    {
      strip.setPixelColor(Position[i], strip.Color(colors[i][0],colors[i][1],colors[i][2]));
    }
   
    strip.show();
    
      for(uint8_t j=0; j<N_LEDS; j++) 
       {
        strip.setPixelColor(j, strip.Color(0, 0, 0)); //онуляем ленту
       }  
      button_pattern();
    if (pattern != PATTERN_BALLS) return;  
  }
 }
  button_pattern();
  if (pattern != PATTERN_BALLS) return; 
}

//***************вспомогательные функции***********
 void led_clr()
 {
    for(uint8_t i=0; i<N_LEDS; i++)  //включить все светодиоды с установленной яркостью
       {
        strip.setPixelColor(i, 0, 0, 0);
       }
     strip.show();  
 }

 void led_on()
 {
    for(uint8_t i=0; i<N_LEDS; i++)  //включить все светодиоды с установленной яркостью
     {
       strip.setPixelColor(i, 255, 255, 255);
     }
     strip.show();
 }

  void led_red()
 {
    for(uint8_t i=0; i<(N_LEDS/2)+1; i++)  //включить все светодиоды с установленной яркостью
     {
       strip.setPixelColor(i, 255, 0, 0);
     }
     strip.show();
 }

  void led_blue()
 {
    for(uint8_t i=(N_LEDS/2)-1; i<N_LEDS; i++)  //включить все светодиоды с установленной яркостью
     {
       strip.setPixelColor(i, 0, 0, 255);
     }
     strip.show();
 }
