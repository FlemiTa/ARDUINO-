#include <EEPROM.h>        // libreria EEPROM
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <EEPROM.h>        // libreria EEPROM
#define RELE 5

RTC_DS3231 rtc;

LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7);

//temperatura
float SENSOR = 2;
float TEMPERATURA; 
float TMAX0;
float TMAX;
float TMAX2;
float TMIN0;
float TMIN;
float TMIN2;
float HMAX0;
float HMAX;
float HMAX2;
float HMIN0;
float HMIN;
float HMIN2;
float HUMEDAD;
DHT dht(SENSOR, DHT22);

//pulsador
int PULSADOR = 10;

//millis
unsigned long time;
unsigned long t = 0;
unsigned long t1 = 0;
int intervalo = 100;


void setup() {
  Serial.begin(9600);       // inicializa monitor serie a 9600 bps
  pinMode(PULSADOR, INPUT); // pulsador
  pinMode(RELE, OUTPUT);    // pin 5 como salida
  digitalWrite(RELE, HIGH);
  
  TMAX0 = EEPROM.get(0, TMAX0);    //carga en tmax0 el valor de temperatura maxima guardado
  TMIN0 = EEPROM.get(4, TMIN0);
  HMAX0 = EEPROM.get(10, HMAX0);    //carga en tmax0 el valor de temperatura maxima guardado
  HMIN0 = EEPROM.get(14, HMIN0);

  lcd.setBacklightPin (3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16, 2);
  lcd.clear();
  dht.begin();
  TMAX= dht.readTemperature();
  TMIN= dht.readTemperature();
  HMAX= dht.readHumidity();
  HMIN= dht.readHumidity();
}               

void loop() {         
 
 time = millis();
  
if (time-t>1000){   // delay de 1 segundo
 t=time;             //  
 
 TMIN2 = dht.readTemperature();
 TMAX2 = dht.readTemperature();
 HMIN2 = dht.readHumidity();
 HMAX2 = dht.readHumidity();
 TEMPERATURA = dht.readTemperature();
 HUMEDAD = dht.readHumidity();
 //lcd.clear();

  if ( TMAX2 > TMAX ){     //Cambia valor temperatura maxima
  (TMAX = TMAX2);
  }

  if ( TMIN2 < TMIN ){   //Cambia valor temperatura minima
  (TMIN = TMIN2);
  }
   if ( HMAX2 > HMAX ){     //Cambia valor temperatura maxima
  (HMAX = HMAX2);
  }

  if ( HMIN2 < HMIN ){   //Cambia valor temperatura minima
  (HMIN = HMIN2);
  }
  
 lcd.setCursor(0, 0);
 lcd.print("T");
 lcd.print(TEMPERATURA, 1);
 //lcd.setCursor(10, 0);
 lcd.print((char)223); // agrega el simbolo "°" 
 lcd.setCursor(7, 0);
 lcd.print("H");
 lcd.print(HUMEDAD, 0);
 //lcd.setCursor(13, 1);
 lcd.print("%");
 lcd.setCursor(0 , 1);
 lcd.print("MAX");
 lcd.print(TMAX0, 1);
 lcd.print(" ");
 lcd.print("MIN");
 //lcd.setCursor(12 , 1);
 lcd.print(TMIN0, 1);
 //delay (1000);

 /*if(digitalRead(PULSADOR) == HIGH){  // espera infinitamente hasta que se presione el pulsador
 TMAX0 = dht.readTemperature();
 EEPROM.put(0, TMAX0);
 digitalWrite(RELE, LOW);       // activa modulo de rele con nivel alto
 Serial.println( "Rele encendido" );   // muestra texto en monitor serie
 }
 }
*/


  
 if(digitalRead(PULSADOR) == HIGH){  // espera infinitamente hasta que se presione el pulsador
 TMAX0 = dht.readTemperature();      
 TMAX = dht.readTemperature();       // REINICIA LOS VALORES DE TEMPERATURA 
 TMIN0 = dht.readTemperature();
 TMIN = dht.readTemperature();
 
 HMAX0 = dht.readHumidity();      
 HMAX = dht.readHumidity();      // REINICIA LOS VALORES DE HUMEDAD 
 HMIN0 = dht.readHumidity();
 HMIN = dht.readHumidity();
 
 
 EEPROM.put(0, TMAX0);
 EEPROM.put(4, TMIN0);
 EEPROM.put(10, HMAX0);
 EEPROM.put(14, HMIN0);
 //digitalWrite(RELE, LOW);       // activa modulo de rele con nivel alto
 Serial.println( "Min/max reiniciadas" );   // muestra texto en monitor serie
 lcd.clear();
 }


 if (time-t1>10000) {
  t1=time;
  Serial.println("hola");

  if ( TMAX > TMAX0 ){     //Cambia valor temperatura maxima
  (TMAX0 = TMAX);
  }
  
  if ( TMIN < TMIN0 ){     //Cambia valor temperatura MINIMA
  (TMIN0 = TMIN);
  }
  if ( HMAX > HMAX0 ){     //Cambia valor temperatura maxima
  (HMAX0 = HMAX);
  }
  
  if ( HMIN < HMIN0 ){     //Cambia valor temperatura MINIMA
  (HMIN0 = HMIN);
  }
  
  EEPROM.put(0, TMAX0);     // almacena en direccion cero TEMPERATURA MAXIMA
  Serial.print("Temperatura maxima:");  // imprime texto
  Serial.println( EEPROM.get(0, TMAX0), 1 );     // obtiene valor de punto flotante                                          
  EEPROM.put(4, TMIN0);     // almacena en direccion diez TEMPERATURA MINIMA
  Serial.print("Temperatura minima:");  // imprime texto
  Serial.println( EEPROM.get(4, TMIN0), 1 );     // obtiene cadena de caracteres en 
  
  EEPROM.put(10, HMAX0);     // almacena en direccion cero TEMPERATURA MAXIMA
  Serial.print("Humedad maxima:");  // imprime texto
  Serial.println( EEPROM.get(10, HMAX0), 0 );     // obtiene valor de punto flotante                                            
  EEPROM.put(14, HMIN0);     // almacena en direccion diez TEMPERATURA MINIMA
  Serial.print("Humedad minima:");  // imprime texto
  Serial.println( EEPROM.get(14, HMIN0), 0 );     // obtiene cadena de caracteres en 
  }
 
 }
}
