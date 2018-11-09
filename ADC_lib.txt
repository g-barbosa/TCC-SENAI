//BIBLIOTECA DO CONVERSOR A/D


//VELOCIDADE DE PROCESSAMENTO
#define _XTAL_FREQ 20000000

//FUNÇÃO PARA INICIAR O CONVERSOR A/D
void init_ADC ( void )
{
    ADCON0 = 0b00000001;          //liga e configura o conversor A/D
                                  //seleciona quais pinos são analogicos
    
    TRISA = 0XFF;                //PORTA são entradas digitais
    PORTA = 0X00;
    CMCON = 7;                   //Desliga os comparadores
}

//FUNÇÃO DE LEITURA ANALOGICA
int ADC_Read (unsigned char canal)
{
    if(canal > 7) return 0;      //se canal for maior que 7, ele nao retorna
    ADCON0 &= 0XC5;
    ADCON0 |= canal <<3;
    ADCON2bits.ADFM = 1;
    __delay_ms(2);
    ADCON0bits.GO_nDONE = 1;                //liga a conversão
    while( ADCON0bits.GO_nDONE );             //enquanto a conversão estiver acontecendo...
    return ((ADRESH<<8)+ADRESL);   //armazena valor
}