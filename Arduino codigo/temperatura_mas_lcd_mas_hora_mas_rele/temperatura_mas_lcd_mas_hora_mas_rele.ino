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
int HUMEDAD;
DHT dht(SENSOR, DHT22);

//inicio/apagado luces
# define RELE 5
bool evento_inicio = true;  // variable de control para inicio de evento con valor true
bool evento_fin = true;   // variable de control para finalizacion de evento con valor true
  
void setup() {
  
  pinMode(RELE, OUTPUT);    // pin 5 como salida
  digitalWrite(RELE, HIGH);
  
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
 lcd.print("T");
 lcd.print(TEMPERATURA, 1);
 //lcd.setCursor(10, 0);
 lcd.print((char)223); // agrega el simbolo "°" 
 lcd.setCursor(7, 0);
 lcd.print("H");
 lcd.print(HUMEDAD);
 //lcd.setCursor(13, 1);
 lcd.print("%");
 delay (1000);

 if ( fecha.hour() == 10 && fecha.minute() == 54 ){ // si hora = 14 y minutos = 30
    if ( evento_inicio == true ){     // si evento_inicio es verdadero
      digitalWrite(RELE, LOW);       // activa modulo de rele con nivel alto
      Serial.println( "Rele encendido" );   // muestra texto en monitor serie
      lcd.setCursor(11, 0);
      lcd.print("L On");
      //evento_inicio = false;        // carga valor falso en variable de control
    }             // para evitar ingresar mas de una vez
  }

 if ( fecha.hour() == 10 && fecha.minute() == 55 ){ // si hora = 15 y minutos = 30
    if ( evento_fin == true ){        // si evento_fin es verdadero
      digitalWrite(RELE, HIGH);        // desactiva modulo de rele con nivel bajo
      Serial.println( "Rele apagado" );     // muestra texto en monitor serie
      lcd.setCursor(11, 0);
      lcd.print("L Off");
      evento_fin = false;       // carga valor falso en variable de control
      evento_inicio = true;       // variables de control en verdadero
      evento_fin = true;
    }             // para evitar ingresar mas de una vez
 } 


 
}
