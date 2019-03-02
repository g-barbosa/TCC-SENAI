#define _XTAL_FREQ 20000000
#define RS PORTCbits.RC0
#define E  PORTCbits.RC1
#define LCD PORTD

void LCD_r()
{
    RS = 0;
    
    LCD = 0x14;
    __delay_ms(1);
    E = 1;
    __delay_ms(1);
    E = 0;
}

//MANDA CURSOR PRA HOME
void home()
{
    RS = 0;
    
    LCD = 0b00000010;
    __delay_ms(1);
    E = 1;
    __delay_ms(1);
    E = 0;
}

//ROTACIONA LCD
void move()
{
    RS = 0;
    
    LCD = 0b00010100;
    __delay_ms(1);
    E = 1;
    __delay_ms(1);
    E = 0;
    
}

//DEFINE POSIÇÃO
void gotoXY(unsigned char x, unsigned char y)
{
    unsigned char i = 0;
    home();
    for(i;i<(x+y*64);i++)
    {
        move();
    }
}

//INICIA LCD
void init_LCD()
{
    TRISC = 0X00;
    PORTC = 0X00;
    TRISD = 0X00;
    RS = 0;
    
    E  = 0;
    LCD = 0x38;
    __delay_ms(1);
    E = 1;
    __delay_ms(1);
    E = 0;
    
    RS = 0;
    
    E  = 0;
    LCD = 0x0C;
    __delay_ms(1);
    E = 1;
    __delay_ms(1);
    E = 0;
}

//LIMPA LCD
void LCD_clear (void)
{
    RS = 0;
   
    LCD = 0b00000001;
    __delay_ms(1);
    E = 1;
    __delay_ms(1);
    E = 0;
}

//ESCREVE CARACTERES
void print_LCD(unsigned char letra)
{
    LCD = letra;
    __delay_ms(1);
    RS = 1;
    __delay_ms(1);
    E = 1;
    __delay_ms(1);
    E = 0;
}


//ESCREVE STRINGS NO LCD
void Write_LCD(unsigned char x, unsigned char y, const char *ptr)
{
    gotoXY(x,y);
    while(*ptr)
        print_LCD(*ptr++);
}


void num2str(unsigned int n, unsigned char *string)
{
    //variaveis
    unsigned int aux,
        divisor = 10000;
    
    char i = 0;
  if( n )
  {
    for(;divisor;divisor/=10)          //divisor inicia em 10000, enquanto divisor for verdade, divisor = divisor /10
    {
        aux = n/divisor;                      //aux = variavel/divisor
        if (divisor == 100) string[i++] = '.';
        
        if (aux)                              //se aux for verdade
        {
            string[i] = (aux%10)+'0';         //string [0] = resto da conta aux /10, e soma com o caractere 0
            i++;                              //incrementa i
        }
        if ( divisor <= 10 )
            break;
    }
      string[i] = 0;                           //onde i parou, se torna 0
  }
  else
  {
        string[i] = '0';
        string[i+1] = 0;
  }
}


void num3str(unsigned int n, unsigned char *string)
{
    //variaveis
    unsigned int aux,
        divisor = 10000;
    
    char i = 0;
  if( n )
  {
    for(divisor;divisor;divisor/=10)          //divisor inicia em 10000, enquanto divisor for verdade, divisor = divisor /10
    {
        aux = n/divisor;                      //aux = variavel/divisor
        if (aux)                              //se aux for verdade
        {
            string[i] = (aux%10)+'0';         //string [0] = resto da conta aux /10, e soma com o caractere 0
            i++;                              //incrementa i
        }
    }
      string[i] = 0;                           //onde i parou, se torna 0
  }
  else
  {
        string[i] = '0';
        string[i+1] = 0;
  }
}
