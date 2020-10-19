#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

RTC_DS3231 rtc;

LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7);
float SENSOR = 2;
float TEMPERATURA; 
float HUMEDAD;

DHT dht(SENSOR, DHT22);

void setup() {

  Serial.begin(9600);
  lcd.setBacklightPin (3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16, 2);
  lcd.clear();
  dht.begin();

  if (! rtc.begin()) {       // si falla la inicializacion del modulo
 Serial.println("Modulo RTC no encontrado !");  // muestra mensaje de error
 while (1);         // bucle infinito que detiene ejecucion del programa
 }

}

void loop() {

 TEMPERATURA = dht.readTemperature();
 HUMEDAD = dht.readHumidity();
 
 DateTime fecha = rtc.now();      // funcion que devuelve fecha y horario en formato
                                 // DateTime y asigna a variable fecha
 Serial.print(fecha.day());     // funcion que obtiene el dia de la fecha completa
 Serial.print("/");       // caracter barra como separador
 Serial.print(fecha.month());     // funcion que obtiene el mes de la fecha completa
 Serial.print("/");       // caracter barra como separador
 Serial.print(fecha.year());      // funcion que obtiene el año de la fecha completa
 Serial.print(" ");       // caracter espacio en blanco como separador
 Serial.print(fecha.hour());      // funcion que obtiene la hora de la fecha completa
 Serial.print(":");       // caracter dos puntos como separador
 Serial.print(fecha.minute());      // funcion que obtiene los minutos de la fecha completa
 Serial.print(":");       // caracter dos puntos como separador
 Serial.println(fecha.second());    // funcion que obtiene los segundos de la fecha completa
 
  
 lcd.setCursor(0, 0);
 lcd.print("Temp: ");
 lcd.print(TEMPERATURA);
 lcd.setCursor(10, 0);
 lcd.print((char)223); // agrega el simbolo "°" 
 lcd.setCursor(0, 1);
 lcd.print("Humedad: ");
 lcd.print(HUMEDAD);
 lcd.setCursor(13, 1);
 lcd.print("%");
 delay (1000);
 
}
