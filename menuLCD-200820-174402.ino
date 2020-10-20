#include <LiquidCrystal.h>

//LCD display pins
#define LCD_RS 12
#define LCD_EN 11
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3 
#define LCD_D7 2

//Rotary 1 encoder (dial) pins
#define ROT1_EN_A 7  
#define ROT1_EN_B 10
#define BTN_MENU A3

#define BTN_BACK A0

#define VIDEO 2

int Estado = 1; 
int Sig_Estado = 1;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);


void setup()
{
  Serial.begin(9600);
  pinMode(BTN_MENU, INPUT);   
  pinMode(BTN_BACK, INPUT);   
  pinMode(ROT1_EN_A,INPUT);
  pinMode(ROT1_EN_B,INPUT);
  digitalWrite(ROT1_EN_A, HIGH);//Enabl epull up resistors
  digitalWrite(ROT1_EN_B, HIGH);

  
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);


  
}

void loop()
{
  Estado = Sig_Estado;

  if(Estado == 1)
  {
      int menu;
      String arrayMenu[] = {"oop1","Opn2","Opcn3","Opcionhjhhhhhh4","Oon5","Language/Idioma"};
      int size = sizeof(arrayMenu) / sizeof(arrayMenu[0]);
      
      menu = menuANTIFALLOSLENTO(arrayMenu,size); // no hace falta poner el numero 6, se calcula automaticamente. Solo debes de cambiar arrayMenu

      if(menu == -1)Sig_Estado = 1;
      else if(menu == 1)Sig_Estado = 2;
      else if(menu == 2)Sig_Estado = 3;
      else if(menu == 3)Sig_Estado = 4;
      else if(menu == 4)Sig_Estado = 5;
      else if(menu == 5)Sig_Estado = 6;
      else if(menu == 6)Sig_Estado = 7;
    
  }
  else if( Estado == VIDEO)
  {
    lcd.clear();
    lcd.print("Opcion1");delay(1000);
    Sig_Estado = 1;
  }
  else if( Estado == 3)
  {
      int menu;
      String arrayMenu[] = {"SubOpcion1","SubOpcion2"};
      int size = sizeof(arrayMenu) / sizeof(arrayMenu[0]);
      menu = menuANTIFALLOSLENTO(arrayMenu,size);

      if(menu == -1)Sig_Estado = 1;
      else if(menu == 1)Sig_Estado = 31;
      else if(menu == 2)Sig_Estado = 32;
  }
      else if( Estado == 31)
      {
        lcd.clear();
        lcd.print("SubOpcion2-1");delay(1000);
        Sig_Estado = 3;
      }
      else if( Estado == 32)
      {
        lcd.clear();
        lcd.print("SubOpcion2-2");delay(1000);
        while(digitalRead(BTN_BACK) == 0);
        Sig_Estado = 3;
      }
  else if( Estado == 4)
  {
    lcd.clear();
    lcd.print("Opcion3");delay(1000);
    Sig_Estado = 1;
  }
  else if( Estado == 5)
  {
    lcd.clear();
    lcd.print("Opcion4");delay(1000);
    Sig_Estado = 1;
  }
  else if( Estado == 6)
  {
    lcd.clear();
    lcd.print("Opcion5");delay(1000);
    Sig_Estado = 1;
  }
  else if( Estado == 7)
  {
    lcd.clear();
    lcd.print("IDIOMAS");delay(1000);
    Sig_Estado = 1;
  }
  
}


int menuANTIFALLOSLENTO(String *arrayMenu,int size)
{

  //Vamos a marcar en que tiempo se hizo cualquier cambio y si se hizo un cambio hace muy poco tiempo y se pulso, ese cambio le damos por malo. ok?
  //Pintamos el cursor y marcamos la primera opcion
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("> ");
  float opcion = 1;  //del 1 al 1.75 Opcion 1  //Del 2  al 2.75 opcion 2
  int extraOpcion = 0;
  float incremento = 0.5;
  
  //Pinta los 4 primeros del menu como mucho
  for(int x = 0; x < size && x <= 3 ; x++) // 
  {
    lcd.setCursor(2,x);
    lcd.print(arrayMenu[x]);
  }
  
  delay(500);

  byte DialPos1 = 0;
  byte Last_DialPos1 = 0;

  unsigned long tiempoCambioIncremento = 0;
  unsigned long tiempoCambioDecremento = 0;

  //Si pulsamos el boton central sale del bucle
  while(digitalRead(BTN_MENU) == 0)
  {
    //Read the status of the dial
    DialPos1 = (digitalRead(ROT1_EN_B) << 1) | digitalRead(ROT1_EN_A);

    // If the dial Position has changed
    if(DialPos1 == 3 && (Last_DialPos1 == 1 ||Last_DialPos1 == 2) )
    {
      //Is the dial being turned clockwise ?
      if (DialPos1 == 3 && Last_DialPos1 == 1)
      {
        if(opcion <size)
        {
          opcion += incremento;
          tiempoCambioIncremento = millis();
        }
      }
      //Is the dial being turned counter-clockwise ?
      if (DialPos1 == 3 && Last_DialPos1 == 2)
      {
        if(opcion>1)
        {
          opcion -=incremento;
          tiempoCambioDecremento = millis();
        }
      }

      //Si sobrepasamos el limite por debajo
      if(opcion < 1 + extraOpcion)
        extraOpcion--;
      //Si sobrepasamos el limite por encima
      if(opcion > 4 + extraOpcion)
        extraOpcion++;

      //Pintamos de nuevo el menu
      lcd.clear();
      for(int x = extraOpcion; x < size && x <= (3+extraOpcion) ; x++) // 
      {
        lcd.setCursor(2,x - extraOpcion);
        lcd.print(arrayMenu[x]);
      }
      //Pintamos el cursor
      lcd.setCursor(0,opcion-1-extraOpcion);
      lcd.print(">");




    }

    //Si pulsamos el boton de atras, salimos del bucle y devolvemos -1
    if(digitalRead(BTN_BACK))
    {    
      //opcion = -1;
      return -1;  //break
    }



    //Remember the last position of the dial so we know when it has changed
    Last_DialPos1 = DialPos1;
  }

  //Aqui hemos salido del bucle ya que hemos pulsado el boton Enter o ATRAS
  //En cualquier caso no debería haberse movido ninguna tecla en 250ms?
  if(millis() - tiempoCambioIncremento < 250)
    opcion -= incremento;//Corregimos aunque no de tiempo a pintarla, ya que salimos del bucle y cambiamos de estado
  else if(millis() - tiempoCambioDecremento < 250)
    opcion += incremento;

  return opcion;
}
