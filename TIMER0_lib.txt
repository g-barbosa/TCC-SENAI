//BIBLIOTECA TIMER0


//VELOCIDADE DE PROCESSAMENTO
#define _XTAL_FREQ 20000000

//PINOS DO TECLADO
#define LINE1 PORTBbits.RB0
#define LINE2 PORTBbits.RB1
#define LINE3 PORTBbits.RB2
#define LINE4 PORTBbits.RB3
#define COLUNAS PORTB

int tscan = 0;
char tecla = 0;
char estado = 0;
unsigned int tempo = 0;
char teclado [4][4] = {{'1','2','3','A'},
                       {'4','5','6','B'},
                       {'7','8','9','C'},
                       {'*','0','#','D'}};

char xT,yT;

//FUNÇÃO PARA INICIAR TIMER0
void init_timer ( void )
{
    INTCONbits.GIE = 0; //liga o disjuntor geral
    TMR0L = 256-250; //quanto vai contar até o proximo estouro
    T0CON = 0b11000000; //configura os registradores
    INTCONbits.TMR0IE = 1; //liga disjuntor do timer
    INTCONbits.GIE = 1; //liga o disjuntor geral
}

//FUNÇÃO DE INTERRUPÇÃO
void interrupt delay ( void )
{
    if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) //se disjuntor do timer e flag estiverem setados
    {
        INTCONbits.TMR0IF = 0; //zera o flag
        TMR0L = 256-250; //contagem até o proximo estouro
        if(tempo)--tempo;
        if (tscan) --tscan;  //tempo da varredura
        
        switch ( estado )
        {
            case 0:
                yT = (yT+1) & 0x03;
                COLUNAS = ~(0x10 << yT);
                tscan = 100;
                estado = 1;
                break;
            case 1:
                if (!LINE1)
                {
                    xT = 0;
                    estado = 2;
                }
                else if (!LINE2)
                {
                    xT = 1;
                    estado = 2;
                }
                else if (!LINE3)
                {
                    xT = 2;
                    estado = 2;
                }
                else if (!LINE4)
                {
                    xT = 3;
                    estado = 2;
                }
                else if (!tscan) estado = 10;
                break;
            case 2:
                tecla = teclado [xT][yT];
                if(!tscan) estado = 10;
                break;
                
            case 10:
                tscan = 100;
                estado = 0;
                break;
                
            default:
                tscan = 100;
                estado = 0;
                break;
        }
    }
}

//FUNÇÃO PARA DEFINIR O DELAY DO TIMER
void loader ( unsigned int load )
{
    INTCONbits.TMR0IE = 0; //desliga disjuntor do timer
    tempo = load; //tempo copia o valor de load;
    INTCONbits.TMR0IE = 1; //liga disjuntor do timer
}

unsigned int uloader ( void )
{
    return ( tempo ); //descarrega o tempo que foi gerado em loader
}
