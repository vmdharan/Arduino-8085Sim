
#include <LiquidCrystal.h>

const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/////////////////////////////////////////////////////
// Setup

// Accumulator
byte regA;

// Register pair B-C
byte regB;
byte regC;

// Register pair D-E
byte regD;
byte regE;

// Register pair H-L
byte regH;
byte regL;

// Temporary register
byte regTM;

// Instruction register
byte regIN;

// Flags register
byte regFL;

// Interrupts
bool EN_interrupt;

// Program counter
word regPC;

// Stack pointer
word regSP;

//--------//
// Memory //
//--------//
byte memory[0x010000];

// Temporary values
byte d1;
byte d2;

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

void decodeInstruction()
{
  byte instr = regIN;

  switch (instr)
  {
    /////////////////////////
    // Data Transfer Group //
    // 84 instructions     //
    /////////////////////////

    // Move Register
    // MOV r1, r2
    // Dest = Register B
    case 0x40:
      regB = regB;
      break;
    case 0x41:
      regB = regC;
      break;
    case 0x42:
      regB = regD;
      break;
    case 0x43:
      regB = regE;
      break;
    case 0x44:
      regB = regH;
      break;
    case 0x45:
      regB = regL;
      break;
    case 0x47:
      regB = regA;
      break;

    // Dest = Register C
    case 0x48:
      regC = regB;
      break;
    case 0x49:
      regC = regC;
      break;
    case 0x4A:
      regC = regD;
      break;
    case 0x4B:
      regC = regE;
      break;
    case 0x4C:
      regC = regH;
      break;
    case 0x4D:
      regC = regL;
      break;
    case 0x4F:
      regC = regA;
      break;

    // Dest = Register D
    case 0x50:
      regD = regB;
      break;
    case 0x51:
      regD = regC;
      break;
    case 0x52:
      regD = regD;
      break;
    case 0x53:
      regD = regE;
      break;
    case 0x54:
      regD = regH;
      break;
    case 0x55:
      regD = regL;
      break;
    case 0x57:
      regD = regA;
      break;

    // Dest = Register E
    case 0x58:
      regE = regB;
      break;
    case 0x59:
      regE = regC;
      break;
    case 0x5A:
      regE = regD;
      break;
    case 0x5B:
      regE = regE;
      break;
    case 0x5C:
      regE = regH;
      break;
    case 0x5D:
      regE = regL;
      break;
    case 0x5F:
      regE = regA;
      break;

    // Dest = Register H
    case 0x60:
      regH = regB;
      break;
    case 0x61:
      regH = regC;
      break;
    case 0x62:
      regH = regD;
      break;
    case 0x63:
      regH = regE;
      break;
    case 0x64:
      regH = regH;
      break;
    case 0x65:
      regH = regL;
      break;
    case 0x67:
      regH = regA;
      break;

    // Dest = Register L
    case 0x68:
      regL = regB;
      break;
    case 0x69:
      regL = regC;
      break;
    case 0x6A:
      regL = regD;
      break;
    case 0x6B:
      regL = regE;
      break;
    case 0x6C:
      regL = regH;
      break;
    case 0x6D:
      regL = regL;
      break;
    case 0x6F:
      regL = regA;
      break;

    // Dest = Register A
    case 0x78:
      regA = regB;
      break;
    case 0x79:
      regA = regC;
      break;
    case 0x7A:
      regA = regD;
      break;
    case 0x7B:
      regA = regE;
      break;
    case 0x7C:
      regA = regH;
      break;
    case 0x7D:
      regA = regL;
      break;
    case 0x7F:
      regA = regA;
      break;

    // Move from memory
    // MOV r, M
    case 0x46:
      regB = memory[(regH << 8) | regL];
      break;
    case 0x4E:
      regC = memory[(regH << 8) | regL];
      break;
    case 0x56:
      regD = memory[(regH << 8) | regL];
      break;
    case 0x5E:
      regE = memory[(regH << 8) | regL];
      break;
    case 0x66:
      regH = memory[(regH << 8) | regL];
      break;
    case 0x6E:
      regL = memory[(regH << 8) | regL];
      break;
    case 0x7E:
      regA = memory[(regH << 8) | regL];
      break;

    // Move to memory
    // MOV M, r
    case 0x70:
      memory[(regH << 8) | regL] = regB;
      break;
    case 0x71:
      memory[(regH << 8) | regL] = regC;
      break;
    case 0x72:
      memory[(regH << 8) | regL] = regD;
      break;
    case 0x73:
      memory[(regH << 8) | regL] = regE;
      break;
    case 0x74:
      memory[(regH << 8) | regL] = regH;
      break;
    case 0x75:
      memory[(regH << 8) | regL] = regL;
      break;
    case 0x77:
      memory[(regH << 8) | regL] = regA;
      break;

    // Move immediate
    // MVI r, data
    case 0x06:
      regB = d1;
      break;
    case 0x0E:
      regC = d1;
      break;
    case 0x16:
      regD = d1;
      break;
    case 0x1E:
      regE = d1;
      break;
    case 0x26:
      regH = d1;
      break;
    case 0x2E:
      regL = d1;
      break;
    case 0x3E:
      regA = d1;
      break;

    // Move to memory immediate
    // MVI M, data
    case 0x36:
      memory[(regH << 8) | regL] = d1;
      break;

    // Load register pair immediate
    // LXI rp, data 16
    case 0x01:
      regB = d2;
      regC = d1;
      break;
    case 0x11:
      regD = d2;
      regE = d1;
      break;
    case 0x21:
      regH = d2;
      regL = d1;
      break;
    case 0x31:
      regSP = (word)((d2 << 8) | d1);
      break;

    // Load Accumulator direct
    // LDA addr
    case 0x3A:
      regA = memory[(d2 << 8) | d1];
      break;

    // Store Accumulator direct
    // STA addr
    case 0x32:
      memory[(d2 << 8) | d1] = regA;
      break;

    // Load H and L direct
    // LHLD addr
    case 0x2A:
      regL = memory[(d2 << 8) | d1];
      regH = memory[((d2 << 8) | d1) + 1];
      break;

    // Store H and L direct
    // SHLD addr
    case 0x22:
      memory[(d2 << 8) | d1] = regL;
      memory[((d2 << 8) | d1) + 1] = regH;
      break;

    // Load accumulator indirect (RP using only B-C or D-E)
    // LDAX rp
    case 0x0A:
      regA = memory[(regB << 8) | regC];
      break;
    case 0x1A:
      regA = memory[(regD << 8) | regE];
      break;

    // Store accumulator indirect (RP using only B-C or D-E)
    // STAX rp
    case 0x02:
      memory[(regB << 8) | regC] = regA;
      break;
    case 0x12:
      memory[(regD << 8) | regE] = regA;
      break;

    // Exchange H and L with D and E
    // XCHG
    case 0xEB:
      regTM = regH;
      regH = regD;
      regD = regTM;

      regTM = regL;
      regL = regE;
      regE = regTM;
      break;

    //////////////////////
    // Arithmetic Group //
    // 65 instructions  //
    //////////////////////

    /////////////////////
    // Logical Group   //
    // 47 instructions //
    /////////////////////

    /////////////////////
    // Branch Group    //
    // 36 instructions //
    /////////////////////

    ///////////////////////////////////////////
    // Stack, I/O, and Machine Control Group //
    // 18 instructions                       //
    ///////////////////////////////////////////

    // Push (Cannot use RP = SP)
    // PUSH rp
    case 0xC5:
      break;
    case 0xD5:
      break;
    case 0xE5:
      break;

    // Push processor status word
    // PUSH PSW
    case 0xF5:
      break;

    // Pop (Cannot use RP = SP)
    case 0xC1:
      break;
    case 0xD1:
      break;
    case 0xE1:
      break;

    // Pop processor status word
    // POP PSW
    case 0xF1:
      break;

    // Exchange stack top with H
    // XTHL
    case 0xE3:
      regTM = regL;
      regL = memory[regSP];
      memory[regSP] = regTM;

      regTM = regH;
      regH = memory[regSP + 1];
      memory[regSP + 1] = regTM;
      break;

    // Move HL to SP
    // SPHL
    case 0xF9:
      regSP = (word)((regH << 8) | regL);
      break;

    // Input
    // IN port
    case 0xDB:
      break;

    // Output
    // OUT port
    case 0xD3:
      break;

    // Enable interrupts
    // EI
    case 0xFB:
      EN_interrupt = true;
      break;

    // Disable interrupts
    // DI
    case 0xF3:
      EN_interrupt = false;
      break;

    // Halt
    // HLT
    case 0x76:
      break;

    // No op
    // NOP
    case 0x00:
      break;

    // Read Interrupt Masks
    // RIM
    case 0x20:
      break;

    // Set Interrupt Masks
    // SIM
    case 0x30:
      break;

    ////////////////////////////
    // 250 instructions total //
    ////////////////////////////
    default:
      break;
  }
}

void run8085()
{
  // Fetch

  // Decode

  // Execute
}

void setup()
{
  lcdSetup();
}

void loop()
{
  lcdMain();

  run8085();
}
