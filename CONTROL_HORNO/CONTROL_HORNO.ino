#include <Arduino.h>
#include <TM1637Display.h>
#include "max6675.h"
// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3
#define vcc_screen 4
#define gnd_screen 5
#define ssr 6
#define gnd_potenciometro 9
#define vcc_potenciometro 10
#define ktcSO 11
#define ktcCS  12
#define ktcCLK  13
#define pot A5

TM1637Display display(CLK, DIO);
const uint8_t celsius[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,  // Circle
  //SEG_A | SEG_D | SEG_E | SEG_F   // C
};
const int offset=10;
int wishtemp=0;
int wish=0;//temperatura deseada
float temperatura=0; //Se crea una variable flotante para almacenar la temperatura
String grados="°C";
int tempahora=0;
int preset=0;
int periodo=200;
unsigned long tiempoahora=0;                                                                     
MAX6675 ktc(ktcCLK, ktcCS, ktcSO);//Se definen los pines a usar para conectar el modulo MAX6675 //const int setpoint=10;

void setup() {
 //tempahora=wishtemp;
  Serial.begin(9600);
  pinMode(vcc_screen, OUTPUT);//VCC SCREEN
  pinMode(gnd_screen, OUTPUT);//GND SCREEN
  pinMode(ssr, OUTPUT);//SSR
 //pinMode(A5, INPUT);//INPUT POTENCIOMETRO
  pinMode(gnd_potenciometro, OUTPUT);//GND POTENCIOMETRO
  pinMode(vcc_potenciometro, OUTPUT);//VCC POTENCIOMETRO
  pinMode(8, INPUT);//BOTON DE PARADA DE EMERGENCIA
  pinMode(7, OUTPUT);//1 LOGICO PARA PARADA DE EMERGENCIA
}

void loop() {  

if(temperatura<(wishtemp-offset))
{
  digitalWrite(ssr, HIGH);
}
else
{  
  digitalWrite(ssr, LOW);
}
while(millis()<tiempoahora+periodo)
{
}
if(wishtemp > tempahora+3) 
{
seleccion();
}
else if (wishtemp < tempahora-3)
{
seleccion();
}
//else{wishtemp=temperatura;}
display.setBrightness(0x0f);
digitalWrite(vcc_screen, HIGH);//VCC SCREEN
digitalWrite(gnd_screen, LOW);//GND SCREEN
digitalWrite(gnd_potenciometro, LOW);//GND POTENCIOMETRO
digitalWrite(vcc_potenciometro, HIGH);//VCC POTENCIOMETRO
wish=analogRead(pot); //lectura del potenciometro
/*temperatura DESEADA*/ wishtemp=map(wish, 0, 1023, 0, 200);//lectura del potenciometro traducido desde 0 a 400 grados centigrados
temperatura=ktc.readCelsius();//A ESTE VALOR SE LE DEBE APLICAR UNA REGRESION LINEAL.
//aca mostramos la temperatura en el display:
uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
display.setSegments(data);
display.showNumberDec(temperatura, false, 3, 0);
display.setSegments(celsius, 2, 3);
tiempoahora=millis();
//imprimimos la temperatura en el serial
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

