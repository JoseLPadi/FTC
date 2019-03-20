#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/* Create some custom graphic characters to make a simple border */
uint8_t TopLeftCorner[]  = {0x00,0x00,0x0f,0x08,0x0b,0x0a,0x0a,0x0a};
uint8_t TopRightCorner[]  = {0x00,0x00,0x1e,0x02,0x1a,0x0a,0x0a,0x0a};
uint8_t BottomLeftCorner[]  = {0x0a,0x0a,0x0b,0x08,0x0f,0x00,0x00,0x00};
uint8_t BottomRightCorner[]  = {0x0a,0x0a,0x1a,0x02,0x1e,0x00,0x00,0x00};
uint8_t VerticalBar[]  = {0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a};

/* Initialise the LiquidCrystal library. The default address is 0x27 
   and this is a 20 x 4 line display */
LiquidCrystal_I2C lcd(0x27,20,4);// en mi caso LCD 20x4 en direccion 27


void setup() 
{
  /* Initialise the LCD */
  lcd.init();
  /* Transfer the custom characters into the LCD's memory */
  lcd.createChar(0, TopLeftCorner);
  lcd.createChar(1, TopRightCorner);
  lcd.createChar(2, BottomLeftCorner);
  lcd.createChar(3, BottomRightCorner);
  lcd.createChar(4, VerticalBar);
}

/* Main program loop */
void loop() 
{
  /* Make sure the backlight is turned on */
  lcd.backlight();

  /* Use the custom graphic characters to draw a border */
  lcd.setCursor(0, 0);
  lcd.write(0); //Write the top left graphic 
  lcd.print("==================");
  lcd.write(1); //Write the top right graphic 
  lcd.setCursor(0, 1);
  lcd.write(4); //Write the vertical bar graphic 
  lcd.setCursor(0, 2);
  lcd.write(4); //Write the vertical bar graphic 
  lcd.setCursor(19, 1);
  lcd.write(4); //Write the vertical bar graphic 
  lcd.setCursor(19, 2);
  lcd.write(4); //Write the vertical bar graphic 
  lcd.setCursor(0, 3);
  lcd.write(2); //Write the bottom left graphic 
  lcd.print("==================");
  lcd.write(3); //Write the bottom right graphic 
  
  /* Display some text inside the border */
  while (1)
  {
    lcd.setCursor(2,1); 
    lcd.print("HOBBY COMPONENTS");
    lcd.setCursor(2,2); 
    lcd.print("**HELLO WORLD**");
    delay(500);
    lcd.setCursor(2,1); 
    lcd.print("                ");
    lcd.setCursor(2,2); 
    lcd.print("               ");
    delay(500);
  }
}
