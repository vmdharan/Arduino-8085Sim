
#include <LiquidCrystal.h>

const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/////////////////////////////////////////////////////

void lcdSetup()
{
  lcd.begin(16, 2);
}

void lcdShowReg(char regName, byte reg)
{
  char regLabel[3] = {regName, ':', '\0'};
  lcd.print(regLabel);
  lcd.print(reg);
  lcd.print(" ");
}

void lcdShowReg2(char *regName, word reg)
{
  char regLabel[4] = {regName[0], regName[1], ':', '\0'};
  lcd.print(regLabel);
  lcd.print(reg);
  lcd.print(" ");
}

void lcdMain()
{
  lcd.setCursor(0, 0);
  lcdShowReg('A', regA);
  lcdShowReg('B', regB);
  lcdShowReg('C', regC);

  lcd.setCursor(0, 1);
  lcdShowReg('I', regIN);
  lcdShowReg('D', regD);
  lcdShowReg('E', regE);

  delay(3000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcdShowReg('H', regH);
  lcdShowReg('L', regL);

  lcd.setCursor(0, 1);
  lcdShowReg('F', regFL);
  lcdShowReg('T', regTM);

  delay(3000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcdShowReg2(new char[3] {'P', 'C'}, regPC);
  lcd.setCursor(0, 1);
  lcdShowReg2(new char[3] {'S', 'P'}, regSP);

  delay(3000);

  /*
    for(int pos=0; pos < 8; pos++) {
    lcd.scrollDisplayLeft();
    delay(400);
    }
    delay(1000);

    for(int pos=0; pos < 8; pos++) {
    lcd.scrollDisplayRight();
    delay(400);
    }
  */

  //delay(1000);
  lcd.clear();
}
