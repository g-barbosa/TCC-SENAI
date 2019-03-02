//BIBLIOTECA USART


//VELOCIDADE DE PROCESSAMENTO
#define _XTAL_FREQ 20000000

//FUNÇÃO PARA INICIAR O USART
void init_serial()
{
    TXSTAbits.BRGH = 1;
    TXSTAbits.SYNC = 0;
    BAUDCONbits.BRG16 = 0;
    SPBRGH = 0;
    SPBRG = 129;
    RCSTAbits.SPEN = 1;
    
    TRISCbits.RC7 = 1;
    TRISCbits.RC6 = 1;
    
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
}

//ESCREVE CARACTERES NO SERIAL
void print_TX ( unsigned char c)
{
    TXREG = c;
    while( !TXSTAbits.TRMT );
}

//ESCREVE STRING NO SERIAL
void Write_Serial ( const char *pont )
{
   while(*pont) print_TX (*pont++); 
}

unsigned char RXavaiable ( void )
{
    return (PIR1bits.RCIF);
}

unsigned char getRX ( void )
{
    return ( RCREG );
}