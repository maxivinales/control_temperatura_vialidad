#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

// The amount of time (in milliseconds) between tests


TM1637Display display(CLK, DIO);
#include "max6675.h"
const uint8_t celsius[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,  // Circle
  //SEG_A | SEG_D | SEG_E | SEG_F   // C
};
int wishtemp=0;
int wish=0;//temperatura deseada
#define pot A5
float temperatura=0; //Se crea una variable flotante para almacenar la temperatura
String grados="°C";
int tempahora=0;
int preset=0;
//Se definen los pines a usar para conectar el modulo MAX6675
int periodo=200;
unsigned long tiempoahora=0;
int ktcSO = 11;
int ktcCS = 12;
int ktcCLK = 13;

                                                                                 // const int setpoint=10;
MAX6675 ktc(ktcCLK, ktcCS, ktcSO);




void setup() {
 // tempahora=wishtemp;

  pinMode(4, OUTPUT);//VCC SCREEN
  pinMode(5, OUTPUT);//GND SCREEN
  pinMode(6, OUTPUT);//SSR
 // pinMode(A5, INPUT);//INPUT POTENCIOMETRO
  pinMode(9, OUTPUT);//GND POTENCIOMETRO
  pinMode(10, OUTPUT);//VCC POTENCIOMETRO
  pinMode(8, INPUT);//BOTON DE PARADA DE EMERGENCIA
  pinMode(7, OUTPUT);//1 LOGICO PARA PARADA DE EMERGENCIA
Serial.begin(9600);

}

void loop() {
  
digitalWrite(7, HIGH);
if (digitalRead(8)==0)//parada de emergencia
{
if(temperatura<wishtemp)
   {digitalWrite(6, HIGH);
   }
   else
 {  digitalWrite(6, LOW);
   }
  while(millis()<tiempoahora+periodo)
   {
   }
   
   if(wishtemp > tempahora+3) 
 
 
{seleccion();
  }

    else if (wishtemp < tempahora-3)
{seleccion();
  }


  //else{wishtemp=temperatura;}
  
}
else {emergencia();}
 
display.setBrightness(0x0f);

digitalWrite(4, HIGH);//VCC SCREEN
digitalWrite(5, LOW);//GND SCREEN
digitalWrite(9, LOW);//GND POTENCIOMETRO
digitalWrite(10, HIGH);//VCC POTENCIOMETRO
wish=analogRead(pot); //lectura del potenciometro
                                               /*temperatura DESEADA*/ wishtemp=map(wish, 0, 1023, 0, 200);//lectura del potenciometro traducido desde 0 a 400 grados centigrados
temperatura=ktc.readCelsius();//A ESTE VALOR SE LE DEBE APLICAR UNA REGRESION LINEAL.



  uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
  display.setSegments(data);
  
  display.showNumberDec(temperatura, false, 3, 0);
  display.setSegments(celsius, 2, 3);
 
  
    tiempoahora=millis();
   
  
   Serial.print("Temperatura = "); 
   Serial.print(temperatura);
   Serial.println(" C"); 
   
}

void seleccion()
{
  int x;

  for (x=1; x<150; x++)
  {
  uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
  display.setSegments(data);
  
  display.showNumberDec(wishtemp, false, 3, 0);

  display.setSegments(celsius, 2, 3);

  
  wish=analogRead(pot); //lectura del potenciometro
wishtemp=map(wish, 0, 1023, 0, 200);//lectura del potenciometro traducido desde 0 a 400 grados centigrados
tempahora=wishtemp;
  /*
    tiempoahora=millis();
  // temperatura=ktc.readCelsius();
   
    while(millis()<tiempoahora+periodo)
    );
    Serial.println("LEYENDO TEMPERATURA DESEADA");
   Serial.println(wishtemp);
    }
   }
   tempahora=wishtemp;
   */
  }
}
void emergencia()
{
  int y=0;
  for (y=0; y<10000; y++){
    while (y>1){Serial.println(y);
    delay(1000);
    
digitalWrite(6, LOW);// apagar rele para siempre
  }
  }

}
