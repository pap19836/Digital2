//Proyecto2 Video Juego Pokemon
//***************************************************************************************************************************************
/* Librería para el uso de la pantalla ILI9341 en modo 8 bits
   Basado en el código de martinayotte - https://www.stm32duino.com/viewtopic.php?t=637
   Adaptación, migración y creación de nuevas funciones: Pablo Mazariegos y José Morales
   Con ayuda de: José Guerra
   Modificaciones y adaptación: Diego Morales
   IE3027: Electrónica Digital 2 - 2021
*/
//***************************************************************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"


#include <SPI.h>
#include <SD.h>

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
// Functions Prototypes
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset);

void MapSD(char x[]);
int ASCII_Hex(int a);

void choose_pokemon(uint8_t controller, uint8_t turn);
void main_menu(uint8_t player);
void attack_menu(uint8_t player);
void bag_menu(uint8_t player);
void change_pokemon(uint8_t player);
void run(uint8_t player);
void clean_menu(void);
uint8_t controller(uint8_t controller_number);
void send_pokemon(uint8_t player, uint8_t id);
int damage_calculation (struct attack attack);

File root;
File pic;
uint8_t maps[640];
uint8_t counter;
uint8_t counter_compare;
uint8_t push0 = 31;
bool debounce0 = 0;
uint8_t push1 = 17;
bool debounce1 = 0;


enum types {NORMAL, GRASS, FIRE, WATER, PSYCHIC};
//Attack struct definition
struct attack{
  char name[50];
  int power;
  bool range;      //0 for physical, 1 for special
  int type;
};
//Pokemon Struct definition
struct pokemon {
  //stats
  char name[50];
  int type;
  int id;
  int hp;
  int original_hp;
  int attack;
  int defense;
  int spattack;
  int spdefense;
  int speed;
  int accuracy;

  //attacks
  struct attack attack1;
  struct attack attack2;
  struct attack attack3;

  //iamge
  int width;
  int height_f;
  int height_b;
  
}bulbasaur, charmander, squirtle, abra;


//active pokemons
  //Pokemons seleccionados
  struct pokemon p1pokemon1;
  struct pokemon p1pokemon2;
  struct pokemon p2pokemon1;
  struct pokemon p2pokemon2;
  //Lugares de pokemons
  struct pokemon p1_field_pokemon;
  struct pokemon p1_backup_pokemon;
  struct pokemon p2_field_pokemon;
  struct pokemon_p2_backup_pokemon;
  
  struct pokemon active_pokemon;
  struct pokemon passive_pokemon;

  //Pokemon Images
  extern uint8_t bulbasaur_front[];
  extern uint8_t bulbasaur_back[];
  extern uint8_t charmander_front[];
  extern uint8_t charmander_back[];
  extern uint8_t squirtle_front[];
  extern uint8_t squirtle_back[];
  extern uint8_t abra_front[];
  extern uint8_t abra_back[];

  //controller variables
  uint8_t button = 0;
  uint8_t done = 0;

  float hp_bar_1;
  float hp_bar_2;
  int damage;
  uint8_t effectiveness;
//***************************************************************************************************************************************
// Initialization
//***************************************************************************************************************************************
void setup() {
  pinMode(push0, INPUT_PULLUP);//push 1
  pinMode(push1, INPUT_PULLUP);//push 2

  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

  SPI.setModule(0);       //Set SPI module for the SD Card
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
  //Check for correct SD Init
  if (!SD.begin(32)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  root = SD.open("/");

  //Begin LCD init
  Serial.println("Start");
  LCD_Init();
  LCD_Clear(0x00);

  //variable init
  //Bulbasaur info
  strcpy(bulbasaur.name, "Bulbasaur");
  bulbasaur.type = GRASS;
  //Stats
  bulbasaur.id = 1;
  bulbasaur.hp = 45;
  bulbasaur.original_hp = 45;
  bulbasaur.attack = 49;
  bulbasaur.defense = 49;
  bulbasaur.spattack = 65;
  bulbasaur.spdefense = 65;
  bulbasaur.speed = 45;
  bulbasaur.accuracy = 100;
  //Attacks
  strcpy(bulbasaur.attack1.name, "TACKLE");
  bulbasaur.attack1.type = NORMAL;
  bulbasaur.attack1.range = 0;
  bulbasaur.attack1.power = 40;
  strcpy(bulbasaur.attack2.name, "GROWL");
  bulbasaur.attack2.type = NORMAL;
  bulbasaur.attack2.range = 1;
  bulbasaur.attack2.power = 50;
  strcpy(bulbasaur.attack3.name, "VINE WHIP");
  bulbasaur.attack3.type = GRASS;
  bulbasaur.attack3.range = 1;
  bulbasaur.attack3.power = 45;
  //image
  bulbasaur.width = 60;
  bulbasaur.height_f = 60;
  bulbasaur.height_b = 42;

  //charmander Info
  //Stats
  strcpy(charmander.name, "Charmander");
  charmander.type = FIRE;
  charmander.id = 2;
  charmander.hp = 39;
  charmander.original_hp = 39;
  charmander.attack = 52;
  charmander.defense = 43;
  charmander.spattack = 60;
  charmander.spdefense = 50;
  charmander.speed = 65;
  charmander.accuracy = 100;
  //Attacks
  strcpy(charmander.attack1.name, "SCRATCH");
  charmander.attack1.type = NORMAL;
  charmander.attack1.range = 0;
  charmander.attack1.power = 40;
  strcpy(charmander.attack2.name, "GROWL");
  charmander.attack2.type = NORMAL;
  charmander.attack1.range = 1;
  charmander.attack2.power = 50;
  strcpy(charmander.attack3.name, "EMBER");
  charmander.attack3.type = FIRE;
  charmander.attack1.range = 1;
  charmander.attack3.power = 40;
  //Image
  charmander.width = 60;
  charmander.height_f = 65;
  charmander.height_b = 54;


  //Squrtile Info
  //stats
  strcpy(squirtle.name, "Squirtle");
  squirtle.type = WATER;
  squirtle.id = 3;
  squirtle.hp = 44;
  squirtle.original_hp = 44;
  squirtle.attack = 48;
  squirtle.defense = 65;
  squirtle.spattack = 50;
  squirtle.spdefense = 64;
  squirtle.speed = 43;
  squirtle.accuracy = 100;
  //Attacks
  strcpy(squirtle.attack1.name, "TACKLE");
  squirtle.attack1.type = NORMAL;
  squirtle.attack1.range = 0;
  squirtle.attack1.power = 40;
  strcpy(squirtle.attack2.name, "DEFENSE CURL");
  squirtle.attack2.type = NORMAL;
  squirtle.attack1.range = 0;
  squirtle.attack2.power = 50;
  strcpy(squirtle.attack3.name, "WATER GUN");
  squirtle.attack3.type = WATER;
  squirtle.attack1.range = 0;
  squirtle.attack3.power = 40;
  //Image
  squirtle.width = 60;
  squirtle.height_f = 58;
  squirtle.height_b = 44;


  //Abra Info
  //Stats
  strcpy(abra.name, "Abra");
  abra.type = PSYCHIC;
  abra.id = 4;
  abra.hp = 25;
  abra.original_hp = 25;
  abra.attack = 20;
  abra.defense = 15;
  abra.spattack = 105;
  abra.spdefense = 55;
  abra.speed = 90;
  abra.accuracy = 100;
  //Attacks
  strcpy(abra.attack1.name, "TACKLE");
  abra.attack1.type = NORMAL;
  abra.attack1.range = 0;
  abra.attack1.power = 40;
  strcpy(abra.attack2.name, "CONFUSION");
  abra.attack2.type = NORMAL;
  abra.attack2.range = 1;
  abra.attack2.power = 25;
  strcpy(abra.attack3.name, "PSYCHIC");
  abra.attack3.type = PSYCHIC;
  abra.attack3.range = 1;
  abra.attack3.power = 40;
  //Image
  abra.width = 60;
  abra.height_f = 57;
  abra.height_b = 60;


  //MapSD("Intro.txt");
  //delay(2000);
 



}
//***************************************************************************************************************************************
// Loop
//***************************************************************************************************************************************
void loop() {

  //Selection Menu
  while(1){
    LCD_Clear(0xFFFF);
    String P1_choose_1 = "Player 1, choose 1st Pokemon";
    String P1_choose_2 = "Player 1, choose 2nd Pokemon";
    String P2_choose_1 = "Player 2, choose 1st Pokemon";
    String P2_choose_2 = "Player 2, choose 2nd Pokemon";
    V_line(160, 60, 180, 0x0000);
    H_line(0, 150, 320, 0x0000);
    LCD_Bitmap(50, 65, bulbasaur.width, bulbasaur.height_f, bulbasaur_front);
    LCD_Print(bulbasaur.name, 50, 65+bulbasaur.height_f+5, 1,0x0000, 0xFFFF);
    LCD_Bitmap(210, 65, charmander.width, charmander.height_f, charmander_front);
    LCD_Print(charmander.name, 210, 65+charmander.height_f+5, 1, 0x0000, 0xFFFF);
    LCD_Bitmap(50, 155, squirtle.width, squirtle.height_f, squirtle_front);
    LCD_Print(squirtle.name, 50, 155+squirtle.height_f+5, 1, 0x0000, 0xFFFF);
    LCD_Bitmap(210, 155, abra.width, abra.height_f, abra_front);
    LCD_Print(abra.name, 210, 155+abra.height_f+5, 1, 0x0000, 0xFFFF);
    uint8_t choose_turn = 0;

    FillRect(0, 0, 320, 60, 0x0000);
    LCD_Print(P1_choose_1, 20, 20, 1, 0xffff, 0x0000);
    button = controller(1);
    Serial.println("TEST");
    Serial.println(button);
    choose_pokemon(button, choose_turn);
    choose_turn++;
    button = 0;
    Serial.println(p1pokemon1.name);
    Serial.println(choose_turn);
    
    FillRect(0, 0, 320, 60, 0x0000);
    LCD_Print(P2_choose_1, 20, 20, 1, 0xffff, 0x0000);
    button = controller(2);
    choose_pokemon(button, choose_turn);
    choose_turn++;
    button = 0;
    Serial.println(p2pokemon1.name);
    Serial.println(choose_turn);
    
    FillRect(0, 0, 320, 60, 0x0000);
    LCD_Print(P1_choose_2, 20, 20, 1, 0xffff, 0x0000);
    button = controller(1);
    choose_pokemon(button, choose_turn);
    choose_turn++;
    button = 0;
    Serial.println(p1pokemon2.name);
    Serial.println(choose_turn);
    
    FillRect(0, 0, 320, 60, 0x0000);
    LCD_Print(P2_choose_2, 20, 20, 1, 0xffff, 0x0000);
    button = controller(2);
    choose_pokemon(button, choose_turn);
    choose_turn++;
    button = 0;
    Serial.println(p2pokemon2.name);
    Serial.println(choose_turn);
    break;
  }

  
  LCD_Clear(0x0000);
  LCD_Print("BATTLE!", 110, 110, 2, 0xFFFF, 0x0000);
  delay(2000);

  LCD_Clear(0xFFFF);
  p1_field_pokemon = p1pokemon1;
  p2_field_pokemon = p2pokemon1;
  send_pokemon(1, p1_field_pokemon.id);
  send_pokemon(2, p2_field_pokemon.id);
  Serial.println(p1_field_pokemon.name);
  Serial.println(p2_field_pokemon.name);
  
  while(done == 0){
    if(p1_field_pokemon.speed >= p2_field_pokemon.speed){
      FillRect(0, 170, 160, 20, 0xad75);
      LCD_Print("Player 1 Turn", 15, 175, 1, 0x0000, 0xad75);
      active_pokemon = p1_field_pokemon;
      passive_pokemon = p2_field_pokemon;
      main_menu(1);
//      if(p2_field_pokemon.hp == 0) change_pokemon(2);
//      if(p2pokemon1.hp == 0 && p2pokemon2.hp == 0) done = 1;
//      if(done != 0) break;
      FillRect(0, 170, 160, 20, 0xad75);
      LCD_Print("Player 2 Turn", 15, 175, 1, 0x0000, 0xad75);
      active_pokemon = p2_field_pokemon;
      passive_pokemon = p1_field_pokemon;
      main_menu(2);
//      if(p1_field_pokemon.hp == 0) change_pokemon(1);
//      if(p1pokemon1.hp == 0 && p1pokemon2.hp == 0) done = 2;
    }
    else{
      FillRect(0, 170, 160, 20, 0xad75);
      LCD_Print("Player 2 Turn", 15, 175, 1, 0x0000, 0xad75);
      active_pokemon = p2_field_pokemon;
      main_menu(2);
//      if(p1_field_pokemon.hp == 0) change_pokemon(1);
//      if(p1pokemon1.hp == 0 && p1pokemon2.hp == 0) done = 2;
//      if(done != 0) break;
      FillRect(0, 170, 160, 20, 0xad75);
      LCD_Print("Player 1 Turn", 15, 175, 1, 0x0000, 0xad75);
      active_pokemon = p1_field_pokemon;
      main_menu(1);
//      if(p2_field_pokemon.hp == 0) change_pokemon(2);
//      if(p2pokemon1.hp == 0 && p2pokemon2.hp == 0) done = 1;
    } 
  }

   button = 0;

   while(button == 0){
    if(done == 1){
      LCD_Clear(0x0000);
      LCD_Print("PLAYER 1 WINS!", 40, 110, 2, 0xFFFF, 0x0000);
      LCD_Print("Player 1 press any button to play again", 5, 140, 1, 0xFFFF, 0x0000);
    }
    else if(done == 2){
      LCD_Clear(0x0000);
      LCD_Print("PLAYER 2 WINS!", 40, 110, 2, 0xFFFF, 0x0000);
      LCD_Print("Player 1 press any button to play again", 5, 140, 1, 0xFFFF, 0x0000);
    }
    button = controller(1);
   }
  

}
//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER)
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40 | 0x80 | 0x20 | 0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
  //  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c) {
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
    }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//***************************************************************************************************************************************
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
    LCD_DATA(c >> 8);
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//***************************************************************************************************************************************
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8);
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y + h, w, c);
  V_line(x  , y  , h, c);
  V_line(x + w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 = x + w;
  y2 = y + h;
  SetWindows(x, y, x2 - 1, y2 - 1);
  unsigned int k = w * h * 2 - 1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
      k = k - 2;
    }
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background)
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;

  if (fontSize == 1) {
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if (fontSize == 2) {
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }

  char charInput ;
  int cLength = text.length();
  //Serial.println(cLength, DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength + 1];
  text.toCharArray(char_array, cLength + 1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    //Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1) {
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2) {
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 = x + width;
  y2 = y + height;
  SetWindows(x, y, x2 - 1, y2 - 1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k + 1]);
      //LCD_DATA(bitmap[k]);
      k = k + 2;
    }
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 =   x + width;
  y2 =    y + height;
  SetWindows(x, y, x2 - 1, y2 - 1);
  int k = 0;
  int ancho = ((width * columns));
  if (flip) {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width - 1 - offset) * 2;
      k = k + width * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k - 2;
      }
    }
  }
  else {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width + 1 + offset) * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k + 2;
      }
    }
  }
  digitalWrite(LCD_CS, HIGH);
}

void MapSD(char x[]) {
  pic = SD.open(x, FILE_READ);  //Open image
  int hex1 = 0;
  int val1 = 0;
  int val2 = 0;
  int mapar = 0;
  int vert = 0;
  if (pic) { //If the image file opened
    Serial.println("Leyendo el archivo...");
    while (vert < 239) {
      mapar = 0;
      while (mapar < 640) {
        hex1 = pic.read();
        if (hex1 == 120) { //Check for the x of 0xFF hex
          val1 = pic.read();  //Read the next 2 characters which will be the hex code
          val2 = pic.read();
          val1 = ASCII_Hex(val1);
          val2 = ASCII_Hex(val2);
          maps[mapar] = (val1 * 16) + val2;
          mapar++;
        }
      }
      LCD_Bitmap(0, vert, 320, 1 , maps);
      vert++;
    }
    pic.close();
    Serial.println("DONE");
  }

  else {
    Serial.println("No se pudo encontrar el archivo");
    pic.close();
  }
}

int ASCII_Hex(int a) {
  switch (a) {
    case 48:
      return 0;
    case 49:
      return 1;
    case 50:
      return 2;
    case 51:
      return 3;
    case 52:
      return 4;
    case 53:
      return 5;
    case 54:
      return 6;
    case 55:
      return 7;
    case 56:
      return 8;
    case 57:
      return 9;
    case 97:
      return 10;
    case 98:
      return 11;
    case 99:
      return 12;
    case 100:
      return 13;
    case 101:
      return 14;
    case 102:
      return 15;
    default:
      return 0;
  }
}
void choose_pokemon(uint8_t choice, uint8_t turn){
  switch(choice){
    case 1:
      FillRect(0, 0, 320, 60, 0x0000);
      LCD_Print("You chose Bulbasaur", 20, 20, 1, 0xffff, 0x0000);
      FillRect(0, 60, 159, 89, 0x8430);
      LCD_Bitmap(50, 65, bulbasaur.width, bulbasaur.height_f, bulbasaur_front);
      LCD_Print(bulbasaur.name, 50, 65+bulbasaur.height_f+5, 1,0x0000, 0xFFFF);
      delay(2000);
      FillRect(0, 60, 159, 89, 0xFFFF);
      LCD_Bitmap(50, 65, bulbasaur.width, bulbasaur.height_f, bulbasaur_front);
      LCD_Print(bulbasaur.name, 50, 65+bulbasaur.height_f+5, 1,0x0000, 0xFFFF);
      switch(turn){
        case 0:
          p1pokemon1 = bulbasaur;
          break;
        case 1:
          p2pokemon1 = bulbasaur;
          break;
        case 2:
          p1pokemon2 = bulbasaur;
          break;
        case 3:
          p2pokemon2 = bulbasaur;
          break;
      }
      break;
    case 2:
      FillRect(0, 0, 320, 60, 0x0000);
      LCD_Print("you chose charmander", 20, 20, 1, 0xffff, 0x0000);
      FillRect(161, 60, 159, 89, 0x8430);
      LCD_Bitmap(210, 65, charmander.width, charmander.height_f, charmander_front);
      LCD_Print(charmander.name, 210, 65+charmander.height_f+5, 1, 0x0000, 0xFFFF);
      delay(2000);
      FillRect(161, 60, 159, 89, 0xFFFF);
      LCD_Bitmap(210, 65, charmander.width, charmander.height_f, charmander_front);
      LCD_Print(charmander.name, 210, 65+charmander.height_f+5, 1, 0x0000, 0xFFFF);
      switch(turn){
        case 0:
          p1pokemon1 = charmander;
          break;
        case 1:
          p2pokemon1 = charmander;
          break;
        case 2:
          p1pokemon2 = charmander;
          break;
        case 3:
          p2pokemon2 = charmander;
          break;
      }
      break;
    case 3:
      FillRect(0, 0, 320, 60, 0x0000);
      LCD_Print("You chose squirtle", 20, 20, 1, 0xffff, 0x0000);
      FillRect(0, 151, 159, 89, 0x8430);
      LCD_Bitmap(50, 155, squirtle.width, squirtle.height_f, squirtle_front);
      LCD_Print(squirtle.name, 50, 155+squirtle.height_f+5, 1, 0x0000, 0xFFFF);
      delay(2000);
      FillRect(0, 151, 159, 89, 0xFFFF);
      LCD_Bitmap(50, 155, squirtle.width, squirtle.height_f, squirtle_front);
      LCD_Print(squirtle.name, 50, 155+squirtle.height_f+5, 1, 0x0000, 0xFFFF);
      switch(turn){
        case 0:
          p1pokemon1 = squirtle;
          break;
        case 1:
          p2pokemon1 = squirtle;
          break;
        case 2:
          p1pokemon2 = squirtle;
          break;
        case 3:
          p2pokemon2 = squirtle;
          break;
      }
      break;
    case 4:
      FillRect(0, 0, 320, 60, 0x0000);
      LCD_Print("You chose abra", 20, 20, 1, 0xffff, 0x0000);
      FillRect(161, 151, 159, 89, 0x8430);
      LCD_Bitmap(210, 155, abra.width, abra.height_f, abra_front);
      LCD_Print(abra.name, 210, 155+abra.height_f+5, 1, 0x0000, 0xFFFF);
      delay(2000);
      FillRect(161, 151, 159, 89, 0xFFFF);
      LCD_Bitmap(210, 155, abra.width, abra.height_f, abra_front);
      LCD_Print(abra.name, 210, 155+abra.height_f+5, 1, 0x0000, 0xFFFF);
      switch(turn){
        case 0:
          p1pokemon1 = abra;
          break;
        case 1:
          p2pokemon1 = abra;
          break;
        case 2:
          p1pokemon2 = abra;
          break;
        case 3:
          p2pokemon2 = abra;
          break;
      }
      break;
  }
}

void clean_menu(void){
  FillRect(0, 190, 320, 80, 0xad75);
  H_line(0, 190, 320, 0x0000);
  H_line(0, 215, 320, 0x0000);
  V_line(160, 170, 60, 0x000);
}

void write_on_menu(char string[]){
  FillRect(0, 190, 320, 80, 0xad75);
  LCD_Print(string, 10, 200, 1, 0x0000, 0xad75);
}

void main_menu(uint8_t player){
  clean_menu();
  LCD_Print("ATTACK", 35, 195, 2, 0x0000, 0xad75);
  LCD_Print("BAG", 230, 195, 2, 0x0000, 0xad75);
  LCD_Print("POKEMON", 30, 220, 2, 0x0000, 0xad75);
  LCD_Print("RUN", 230, 220, 2, 0x0000, 0xad75);
  button = controller(player);
  switch(button){
    case 1:
      attack_menu(player);
      break;
    case 2:
      bag_menu(player);
      break;
    case 3:
      change_pokemon(player);
      break;
    case 4:
      run(player);
      break;
  }
}
void attack_menu(uint8_t player){
  clean_menu();
  LCD_Print(active_pokemon.attack1.name, 10, 195, 2, 0x0000, 0xad75);
  LCD_Print(active_pokemon.attack2.name, 170, 195, 2, 0x0000, 0xad75);
  LCD_Print(active_pokemon.attack3.name, 10, 220, 2, 0x0000, 0xad75);
  LCD_Print("BACK", 170, 220, 2, 0x0000, 0xad75);
  button = controller(player);
  switch(button){
    case 1:
      damage = damage_calculation(active_pokemon.attack1);
      write_on_menu("You used...");
      delay(1000);
      write_on_menu(active_pokemon.attack1.name);
      passive_pokemon.hp = passive_pokemon.hp-damage;
      delay(1000);
      break;
    case 2:
      damage = damage_calculation(active_pokemon.attack2);
      write_on_menu("You used...");
      delay(1000);
      write_on_menu(active_pokemon.attack2.name);
      passive_pokemon.hp = passive_pokemon.hp-damage;
      delay(1000);
      break;
    case 3:
      damage = damage_calculation(active_pokemon.attack3);
      write_on_menu("You used...");
      delay(1000);
      write_on_menu(active_pokemon.attack3.name);
      passive_pokemon.hp = passive_pokemon.hp-damage;
      delay(1000);
      if(effectiveness == 2) write_on_menu("IT'S SUPER EFFECTIVE!");
      if(effectiveness == 0) write_on_menu("IT'S NOT VERY EFFECTIVE...");
      delay(1000);
      break;  
    case 4:
      main_menu(player);
      break;
  }
  switch(player){
    case 1:
      p2_field_pokemon = passive_pokemon;
      send_pokemon(2, p2_field_pokemon.id);
      break;
    case 2:
      p1_field_pokemon = passive_pokemon;
      send_pokemon(1, p1_field_pokemon.id);
      break;
  }
}

void bag_menu(uint8_t player){
  clean_menu();
  LCD_Print("POTION", 25, 195, 2, 0x0000, 0xad75);
  LCD_Print("BACK", 170, 220, 2, 0x0000, 0xad75);
}

void change_pokemon(uint8_t player){
  switch(player){
    case 1:
      if(p1_field_pokemon.id == p1pokemon1.id){
        p1pokemon1 = p1_field_pokemon;
        p1_field_pokemon = p1pokemon2;
        write_on_menu("Come back!");
        delay(1000);
        send_pokemon(1, p1_field_pokemon.id);
        write_on_menu(" GO!");
        delay(1000);
      }
      else if(p1_field_pokemon.id == p1pokemon2.id){
        p1pokemon2 = p1_field_pokemon;
        p1_field_pokemon = p1pokemon1;
        write_on_menu("Come back!");
        delay(1000);
        send_pokemon(1, p1_field_pokemon.id);
        write_on_menu(" GO!");
        delay(1000);
      }
      break;
    case 2:
      if(p2_field_pokemon.id == p2pokemon1.id){
        p2pokemon1 = p2_field_pokemon;
        p2_field_pokemon = p2pokemon2;
        write_on_menu("Come back!");
        delay(1000);
        send_pokemon(2, p2_field_pokemon.id);
        write_on_menu(" GO!");
        delay(1000);
      }
      else if(p2_field_pokemon.id == p2pokemon2.id){
        p2pokemon2 = p2_field_pokemon;
        p2_field_pokemon = p2pokemon1;
        write_on_menu("Come back!");
        delay(1000);
        send_pokemon(2, p2_field_pokemon.id);
        write_on_menu(" GO!");
        delay(1000);
      }
      break;
  }
}

void run(uint8_t player){
  if(player == 1){
    done = 2;
  }
  else if( player == 2){
    done = 1;
  }
  
}
uint8_t controller(uint8_t controller_number){
  Serial2.begin(9600);
  Serial3.begin(9600);
  uint8_t x;
  if (controller_number == 1){
    while (Serial2.available()==0);
    if (Serial2.available() > 0) {
    // read the incoming byte:
    x = Serial2.read();
    Serial.print("CONTROL 1: ");
    Serial.println(x, DEC);
    }
  }
 else if (controller_number == 2){
    while (Serial3.available()==0);
    if (Serial3.available() > 0) {
    // read the incoming byte:
    x = Serial3.read();
    Serial.print("CONTROL 2: ");
    Serial.println(x, DEC);
  }
 }
 Serial2.end();
 Serial3.end();
 return x;
}
void send_pokemon(uint8_t player, uint8_t id){
  switch(player){
    case 1:
      FillRect(0, 85, 320, 85, 0xFFFF);
      LCD_Print(p1_field_pokemon.name, 170, 130, 1, 0x0000, 0xFFFF);
      
      hp_bar_1 = 100*p1_field_pokemon.hp/p1_field_pokemon.original_hp;
      H_line(170, 145, (int)hp_bar_1, 0xF800);
      
      switch(id){
        case 1:
          LCD_Bitmap(40, 110, bulbasaur.width, bulbasaur.height_b, bulbasaur_back);
          break;
        case 2:
          LCD_Bitmap(40, 110, charmander.width, charmander.height_b, charmander_back);
          break;
        case 3:
          LCD_Bitmap(40, 110, squirtle.width, squirtle.height_b, squirtle_back);
          break;
        case 4:
          LCD_Bitmap(40, 110, abra.width, abra.height_b, abra_back);
          break;
      }
      break;
    case 2:
      FillRect(0, 0, 320, 85, 0xFFFF);
      LCD_Print(p2_field_pokemon.name, 30, 20, 1, 0x0000, 0xFFFF);

      hp_bar_2 = 100*p2_field_pokemon.hp/p2_field_pokemon.original_hp;
      H_line(30, 35, (int)hp_bar_2, 0xF800);
      switch(id){
        case 1:
          LCD_Bitmap(200, 20, bulbasaur.width, bulbasaur.height_f, bulbasaur_front);
          break;
        case 2:
          LCD_Bitmap(200, 20, charmander.width, charmander.height_f, charmander_front);
          break;
        case 3:
          LCD_Bitmap(200, 20, squirtle.width, squirtle.height_f, squirtle_front);
          break;
        case 4:
          LCD_Bitmap(200, 20, abra.width, abra.height_f, abra_front);
          break;
      }
      break;
  }
}
int damage_calculation (struct attack attack){
  float x = 0;
  int A;
  int D;
  if(attack.range == 0){
     A = active_pokemon.attack;
     D = passive_pokemon.defense;
  }
  else if(attack.range ==1){
     A = active_pokemon.spattack;
     D = passive_pokemon.spdefense;
  }
  x = (((4*attack.power*A/D)/50)+2);
  //STAB
  if(active_pokemon.type == attack.type) x = x*1.5;
  x = type_advantage(x, attack);
  return (int)x;
}

float type_advantage(float value, struct attack attack){
  float out;
  if(passive_pokemon.type == GRASS){
    switch(attack.type){
      case FIRE:
        out = 2*value;
        effectiveness = 2;
        return out;
      case WATER:
        out = value/2;
        effectiveness = 0;
        return out;
      default:
        out = value;
        effectiveness = 1;
        return out;
    }
  }
  if(passive_pokemon.type == FIRE){
    switch(attack.type){
      case WATER:
        out = 2*value;
        effectiveness = 2;
        return out;
      case GRASS:
        out = value/2;
        effectiveness = 0;
        return out;
      default:
        out = value;
        return out;
        effectiveness = 1;
    }
    
  }if(passive_pokemon.type == WATER){
    switch(attack.type){
      case GRASS:
        out = 2*value;
        effectiveness = 2;
        return out;
      case FIRE:
        out = value/2;
        effectiveness = 0;
        return out;
      default:
        out = value;
        effectiveness = 1;
        return out;
    }    
  }
}
