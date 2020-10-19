#include <EEPROM.h>        // libreria EEPROM
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <EEPROM.h>        // libreria EEPROM

# define RELE 5

RTC_DS3231 rtc;

LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7);


// temperatura
float SENSOR = 2;
float TEMPERATURA; 
float TMAX0;
float TMAX;
float TMAX2;
float TMIN0;
float TMIN = 90;
float TMIN2;
int HUMEDAD;
DHT dht(SENSOR, DHT22);

// pulsador
int PULSADOR = 10; 
int press = 0;
boolean toggle = true;

//millis
unsigned long time;
unsigned long t = 0;
unsigned long tminmax = 0;
int intervalo = 100;


void setup() {
  Serial.begin(9600);       // inicializa monitor serie a 9600 bps
  pinMode(PULSADOR, INPUT); // pulsador
  pinMode(RELE, OUTPUT);    // pin 5 como salida
  digitalWrite(RELE, HIGH);
 
 
 /* EEPROM.put(0, TMAX);     // almacena en direccion cero TEMPERATURA MAXIMA
  EEPROM.put(3, TMIN);     // almacena en direccion diez TEMPERATURA MINIMA
  */
  
  TMAX0 = EEPROM.get(0, TMAX0);
  TMIN0 = EEPROM.get(4, TMIN0);
 
  lcd.setBacklightPin (3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16, 2);
  lcd.clear();
  dht.begin();
  TMAX= dht.readTemperature();
  TMIN= dht.readTemperature();
}               // direccion diez y muestra

void loop() {         // funcion loop() declarada pero sin contenido
 
 time = millis();


 
  
  /*
  EEPROM.put(0, TMAX0);
  */
  
 
if (time-t>1000){   // delay de 1 segundo
 t=time;             //  
 
 TMIN2 = dht.readTemperature();
 TMAX2 = dht.readTemperature();
 TEMPERATURA = dht.readTemperature();
 HUMEDAD = dht.readHumidity();
 //lcd.clear();

  if ( TMAX2 > TMAX ){     //Cambia valor temperatura maxima
  (TMAX = TMAX2);
  }

  if ( TMIN2 < TMIN ){   //Cambia valor temperatura minima
  (TMIN = TMIN2);
  }
  
 
 lcd.setCursor(0, 0);
 lcd.print("T");
 lcd.print(TEMPERATURA, 1);
 //lcd.setCursor(10, 0);
 lcd.print((char)223); // agrega el simbolo "°" 
 lcd.setCursor(7, 0);
 lcd.print("H");
 lcd.print(HUMEDAD);
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

 if(digitalRead(PULSADOR) == HIGH){  // espera infinitamente hasta que se presione el pulsador
 TMAX0 = dht.readTemperature();
 EEPROM.put(0, TMAX0);
 digitalWrite(RELE, LOW);       // activa modulo de rele con nivel alto
 Serial.println( "Rele encendido" );   // muestra texto en monitor serie
 }
 
 

 
 
}


 



 
 if (time-tminmax>10000) {
  tminmax=time;
  Serial.println("hola");

  if ( TMAX > TMAX0 ){     //Cambia valor temperatura maxima
  (TMAX0 = TMAX);
  }
  if ( TMIN < TMIN0 ){     //Cambia valor temperatura MINIMA
  (TMIN0 = TMIN);
  }
  
  EEPROM.put(0, TMAX0);     // almacena en direccion cero TEMPERATURA MAXIMA
  Serial.print("Temperatura maxima:");  // imprime texto
  Serial.println( EEPROM.get(0, TMAX0), 1 );     // obtiene valor de punto flotante
                                            
  EEPROM.put(4, TMIN0);     // almacena en direccion diez TEMPERATURA MINIMA
  Serial.print("Temperatura minima:");  // imprime texto
  Serial.println( EEPROM.get(4, TMIN0), 1 );     // obtiene cadena de caracteres en 
  
  }

 
  

}
