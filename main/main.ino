

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
int flag_Z;
int flag_P;
int flag_S;
int flag_CY;
int flag_AC;

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

// Convert a byte to a bit-string representation.
// High order bit is in index 0.
// Low order bit is in index 7.
char *convertFromByte(byte byteVal)
{
  int i;
  int n = 128;
  int b = (int)byteVal & 0x000000FF;
  char *bufferVal = new char[8];

  for (i = 0; i < 8; i++)
  {
    if ((b / n) < 1.0)
    {
      bufferVal[i] = (char)0;
    }
    else
    {
      bufferVal[i] = (char)1;
      b = b - n;
    }

    n = n / 2;
  }

  return bufferVal;
}

// Convert a bit-string to a byte representation
byte convertToByte(char *bytebuffer)
{
  int i;
  int n = 0;

  for (i = 0; i < 8; i++)
  {
    if (bytebuffer[i] == 1)
    {
      n += (1 << (7 - i));
    }
  }

  return (byte)n;
}

// Update Carry flag.
void updateCarryFlag(int carry)
{
  // Set Carry flag.
  if (carry == 1)
  {
    flag_CY = 1;
  }
  else
  {
    flag_CY = 0;
  }
}

// Update Sign flag.
void updateSignFlag(char *rb)
{
  // Set Sign flag.
  if (rb[0] == 1)
  {
    flag_S = 1;
  }
  else
  {
    flag_S = 0;
  }
}

// Update Zero flag.
void updateZeroFlag(byte result)
{
  // Set Zero flag.
  if (result == 0x00)
  {
    flag_Z = 1;
  }
  else
  {
    flag_Z = 0;
  }
}

// Update Parity flag.
void updateParityFlag(char *rb)
{
  int i;

  // Set Parity flag.
  flag_P = 0;
  for (i = 0; i < 8; i++)
  {
    if (rb[i] == 1)
    {
      flag_P++;
    }
  }

  if ((flag_P % 2) == 0)
  {
    flag_P = 1;
  }
  else
  {
    flag_P = 0;
  }
}

// Add two bytes
byte addTwoBytes(byte b1, byte b2, int CF)
{
  int i;
  byte result;
  int carry = 0;

  // Add with carry.
  if (CF == 1)
  {
    carry = flag_CY;
  }

  char *op1 = convertFromByte(b1);
  char *op2 = convertFromByte(b2);
  char *rb = new char[8];

  flag_AC = 0;

  for (i = 7; i >= 0; i--)
  {
    // Add two '1's.
    if ((op1[i] == 1) & (op2[i] == 1))
    {
      if (carry == 1)
      {
        rb[i] = (char)1;
      }
      else
      {
        rb[i] = (char)0;
      }
      carry = 1;

      // Update Auxiliary carry.
      if (i == 4)
      {
        flag_AC = 1;
      }
    }
    // Add two '0's.
    else if ((op1[i] == 0) & (op2[i] == 0))
    {
      if (carry == 1)
      {
        rb[i] = (char)1;
      }
      else
      {
        rb[i] = (char)0;
      }
      carry = 0;
    }
    // Add '1' and '0'
    else
    {
      if (carry == 1)
      {
        rb[i] = (char)0;
        carry = 1;

        // Update Auxiliary carry.
        if (i == 4)
        {
          flag_AC = 1;
        }
      }
      else
      {
        rb[i] = (char)1;
      }
    }
  }

  updateSignFlag(rb);
  updateCarryFlag(carry);

  result = convertToByte(rb);

  updateZeroFlag(result);
  updateParityFlag(rb);

  return result;
}

// Returns the Ones Complement.
char* onesComplement(char* b)
{
  int i;
  char* result = new char[8];

  for (i = 0; i < 8; i++)
  {
    if (b[i] == (char) 0)
    {
      result[i] = (char) 1;
    }
    else
    {
      result[i] = (char) 0;
    }
  }

  return result;
}


// Subtract byte
byte subtractByte(byte b1, byte b2)
{
  int i;
  byte result;
  int carry = 0;

  char* op1 = convertFromByte(b1);
  char* op2 = onesComplement(convertFromByte(b2));
  char* rb = new char[8];

  flag_AC = 0;

  for (i = 7; i >= 0; i--)
  {
    // Add two '1's.
    if ((op1[i] == 1) & (op2[i] == 1))
    {
      if (carry == 1)
      {
        rb[i] = (char)1;
      }
      else
      {
        rb[i] = (char)0;
      }
      carry = 1;

      // Update Auxiliary carry.
      if (i == 4)
      {
        flag_AC = 1;
      }
    }
    // Add two '0's.
    else if ((op1[i] == 0) & (op2[i] == 0))
    {
      if (carry == 1)
      {
        rb[i] = (char)1;
      }
      else
      {
        rb[i] = (char)0;
      }
      carry = 0;
    }
    // Add '1' and '0'
    else
    {
      if (carry == 1)
      {
        rb[i] = (char)0;
        carry = 1;

        // Update Auxiliary carry.
        if (i == 4)
        {
          flag_AC = 1;
        }
      }
      else
      {
        rb[i] = (char)1;
      }
    }
  }

  char* rb2 = new char[8];

  if (carry == 1)
  {
    rb2 = addCarry(rb);
    carry = 0;
  }
  else
  {
    rb2 = rb;
  }

  updateSignFlag(rb2);
  updateCarryFlag(carry);

  result = convertToByte(rb2);

  updateZeroFlag(result);
  updateParityFlag(rb2);

  return result;
}

// Add carry to byte.
char* addCarry(char* b)
{
  int i;
  char* result = new char[8];
  int carry = 1;

  for (i = 7; i >= 0; i--)
  {
    if ((b[i] == (char)0) && (carry == 1))
    {
      result[i] = (char)1;
      carry = 0;
    }
    else if ((b[i] == (char)0) && (carry == 0))
    {
      result[i] = (char)0;
    }
    else if ((b[i] == (char)1) && (carry == 1))
    {
      result[i] = (char)0;
      carry = 1;

      // Update Auxiliary carry.
      if (i == 4)
      {
        flag_AC = 1;
      }
    }
    else if ((b[i] == (char)1) && (carry == 0))
    {
      result[i] = (char)1;
    }
  }

  return result;
}

/////////////////////////////////////////////////////


void run8085()
{
  // Fetch

  // Decode

  // Execute

  regA = 0x13;
  regIN = 0x47;
  decodeInstruction();

  regC = 0x10;
  regIN = 0x91;
  decodeInstruction();
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
