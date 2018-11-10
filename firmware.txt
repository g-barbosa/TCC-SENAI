//SISTEMA DE MONITORAMENTO DE ENERGIA ELETRICA "eLETROVIEW"
//Criado por Giovanne Barbosa 
//v1.7.1
//última atualização: 29/11/2017 21:43
//Modificações: melhorada a precisão dos calculos, criada funções de leitura de potencias

//BIBLIOTECAS
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "ADC_lib.h"
#include "lcd.h"
#include "Timer0_lib.h"
#include "USART.h"

//CONFIGURAÇÕES
#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF

//VELOCIDADE DE PROCESSAMENTO
#define _XTAL_FREQ 20000000

//ESTADOS
#define INICIO                          0
#define TELA_ELETROVIEW                 1
#define TELA_ELETROVIEW_ESPERA          2
#define TELA_MONITORA                   3
#define TELA_MONITORA_ESPERA            4
#define TELA_PRINCIPAL                  5
#define TELA_PRINCIPAL_ESPERA           6
#define TELA_POTENCIAS                  7
#define TELA_POTENCIAS_ESPERA           8
#define TELA_SERIAL                     9
#define TELA_SERIAL_ESPERA             10
#define TELA_TEMPO                     11
#define TELA_TEMPO_ESPERA              12

//PROTOTIPOS DE FUNÇÕES
int Ler_corrente();
int Ler_tensao();
void Mostrar_tensao();
void Mostrar_corrente();
int Mostrar_potencias();
int Ler_potencia_ativa();
int Ler_potencia_reativa();
int Ler_potencia_aparente();
int Mostrar_dados_serial();
int Serial();
void dicas_opcao();
void dicas_tempo();

//VARIAVEIS
float valor_corrente;  //variavel para armazenar corrente
float valor_tensao;    //variavel para armazenar tensao
unsigned int valor_potencia_ativa;  //variavel para armazenar potencia ativa
unsigned int valor_potencia_reativa; //variavel para armazenar potencia reativa
float valor_potencia_aparente;  //variavel para armazenar potencia aparente
unsigned char num_str [7];    //vetor para armazenar int
char telas = 0;               //variavel para o estado de telas
char dica = 1;                //variavel para alterar as dicas no display
char dica2 = 1;               //variavel para alterar as dicas na tela de tempo
int amostra = 10000;
int limite_corrente = 0;

//FUNÇÃO PRINCIPAL
void main()
{
    init_ADC(); //inicia leitura analogica
    init_LCD(); //inicia LCD
    init_timer(); //inicia timer0
    init_serial(); //inicia serial
    ADCON1 = 0b00001101; //define entradas analogicas AN0 e AN1
    TRISB  = 0X0F; //Metade do PORTB como entrada digital
    PORTB  = 0XFF; //PORTB inicia em nivel logico alto
    //INTCON2bits.RBPU = 0;  //ativa resistores de pullup (comentar, para simular)

    telas = INICIO; //telas = 0
 
    
    
    while(1)
    {
        switch ( telas )
        {
            case INICIO:
            case TELA_ELETROVIEW:
                LCD_clear();
                Write_LCD(3,0,"eLETROVIEW");
                loader(10000);                       //carrega o loader
                telas = TELA_ELETROVIEW_ESPERA;
                break;
            
            case TELA_ELETROVIEW_ESPERA:             
                if( !uloader() )                     //se loader zerar...
                    telas = TELA_MONITORA;           //vai para o proximo estado
                break;
            
            case TELA_MONITORA:
                LCD_clear();
                Write_LCD(2,0,"MONITORAMENTO");
                Write_LCD(3,1,"DE ENERGIA");
                loader(10000);
                telas = TELA_MONITORA_ESPERA;
                break;
            
            case TELA_MONITORA_ESPERA:
                if( !uloader() )
                    telas = TELA_PRINCIPAL;
                break;
            
            case TELA_PRINCIPAL:
                LCD_clear();
                Mostrar_tensao();
                Mostrar_corrente();
                dicas_opcao();
                loader(30000);
                telas = TELA_PRINCIPAL_ESPERA;
                break;
            
            case TELA_PRINCIPAL_ESPERA:
                if( !uloader() )
                {
                    dica++;
                    telas = TELA_PRINCIPAL;
                }
                    
                else if (tecla == 'A') telas = TELA_POTENCIAS;
                else if (tecla == 'B') telas = TELA_SERIAL;
                else if (tecla == 'C') telas = TELA_TEMPO;
                //else if (tecla == 'D') telas = TELA_SEM_FUNCAO;
                break;

            case TELA_POTENCIAS:
                LCD_clear();
                Mostrar_potencias();
                loader(50000);
                telas = TELA_POTENCIAS_ESPERA;
                break;
            
            case TELA_POTENCIAS_ESPERA:
                if ( !uloader() )
                    telas = TELA_POTENCIAS;
                else if (tecla == '*') telas = TELA_PRINCIPAL;
                break;
                
            case TELA_SERIAL:
                LCD_clear();
                Write_LCD(1,0,"Enviando dados");
                Write_LCD(1,1,"via serial...");
                Mostrar_dados_serial();
                loader(amostra);
                telas = TELA_SERIAL_ESPERA;
                break;
                
            case TELA_SERIAL_ESPERA:
                if( !uloader() )
                    telas = TELA_SERIAL;
                else if (tecla == '*') telas = TELA_PRINCIPAL;
                break;
                
            case TELA_TEMPO:
                LCD_clear();
                Write_LCD(0,0,"Tempo de amostra");
                dicas_tempo();
                loader(10000);
                telas = TELA_TEMPO_ESPERA;
                break;
                
            case TELA_TEMPO_ESPERA:
                if ( !uloader() )
                {
                    dica2++;
                    telas = TELA_TEMPO;
                }
                else if ( tecla == '*') telas = TELA_PRINCIPAL;
                else if (tecla == '1')
                {
                    amostra = 10000;
                    LCD_clear();
                    Write_LCD(5,0,"salvo!");
                    loader(5000);
                    telas = TELA_PRINCIPAL_ESPERA;
                }
                else if (tecla == '2')
                {
                    amostra = 50000;
                    LCD_clear();
                    Write_LCD(5,0,"salvo!");
                    loader(5000);
                    telas = TELA_PRINCIPAL_ESPERA;
                }
                else if (tecla == '3')
                {
                    amostra = 100000;
                    LCD_clear();
                    Write_LCD(5,0,"salvo!");
                    loader(5000);
                    telas = TELA_PRINCIPAL_ESPERA;
                }
                else if (tecla == '4')
                {
                    amostra = 150000;
                    LCD_clear();
                    Write_LCD(5,0,"salvo!");
                    loader(5000);
                    telas = TELA_PRINCIPAL_ESPERA;
                }
                break;
            
        }//end switch(telas)

    }//end while(1)
}//end void main


//FUNÇÕES DE LEITURA E RETORNO
//***********************************************
int Ler_tensao ( void )
{
    valor_tensao = ((5*ADC_Read(0))/0.020)/1023;  //calculo para conversão dos valores de tensão
}

void Mostrar_tensao ( void )
{
    Ler_tensao();
    num3str(valor_tensao,num_str);
    Write_LCD(0,0,"V=");
    Write_LCD(2,0,num_str);
    Write_LCD(5,0,"V");
}
//***********************************************
int Ler_corrente ( void )
{
    limite_corrente = ADC_Read(1);
    if(limite_corrente<512) limite_corrente = 512;
    valor_corrente = (limite_corrente-512)*66;
    if(valor_corrente>=1000) valor_corrente += 1000;
}

void Mostrar_corrente ( void )
{
    Ler_corrente();
    num2str(valor_corrente,num_str);
    Write_LCD(8,0,"I=");
    Write_LCD(10,0,num_str);
    Write_LCD(15,0,"A");
}
//************************************************
int Ler_potencia_ativa( void )
{
   Ler_potencia_aparente();
   valor_potencia_ativa = valor_potencia_aparente*0.99;
}

int Ler_potencia_aparente( void )
{
    Ler_tensao();
    Ler_corrente();
    valor_potencia_aparente = ((valor_tensao/1000) * (valor_corrente/1000));
    valor_potencia_aparente *= 1000;
}

Ler_potencia_reativa( void )
{
    Ler_potencia_aparente();
    valor_potencia_reativa = (valor_potencia_aparente*0.13917);
}

int Mostrar_potencias ( void )
{
    Ler_potencia_ativa();
    num3str(valor_potencia_ativa,num_str);
    Write_LCD(1,0,"W");
    Write_LCD(0,1,num_str);
    
    Ler_potencia_aparente();
    num3str(valor_potencia_aparente,num_str);
    Write_LCD(6,0,"VA");
    Write_LCD(6,1,num_str);
    
    Ler_potencia_reativa();
    num3str(valor_potencia_reativa,num_str);
    Write_LCD(13,0,"VAr");
    Write_LCD(13,1,num_str); 
}


//***********************************************
int Mostrar_dados_serial()
{
    Ler_tensao();
    num3str(valor_tensao,num_str);
    Write_Serial("TENSAO = ");
    Write_Serial(num_str);
    Write_Serial("V");
    Write_Serial("\n\r");
    
    Ler_corrente();
    num2str(valor_corrente,num_str);
    Write_Serial("CORRENTE = ");
    Write_Serial(num_str);
    Write_Serial("A");
    Write_Serial("\n\r");
    
    valor_potencia_aparente = ((valor_tensao/1000) * (valor_corrente/1000));
    valor_potencia_aparente *= 1000;
    num3str(valor_potencia_aparente,num_str);
    Write_Serial("POTENCIA APARENTE = ");
    Write_Serial(num_str);
    Write_Serial("VA");
    Write_Serial("\n\r");
    
    Ler_potencia_ativa();
    valor_potencia_ativa = valor_potencia_aparente*0.99;
    num3str(valor_potencia_ativa,num_str);
    Write_Serial("POTENCIA ATIVA = ");
    Write_Serial(num_str);
    Write_Serial("W");
    Write_Serial("\n\r");
    
    //Ler_potencia_reativa();
    valor_potencia_reativa = (valor_potencia_aparente*0.13917);
    num3str(valor_potencia_reativa,num_str);
    Write_Serial("POTENCIA REATIVA = ");
    Write_Serial(num_str);
    Write_Serial("VAr");
    Write_Serial("\n\r\n\r");
    
}
//***********************************************
//FUNÇÕES DE MENSAGENS
void dicas_opcao ( void )
{
    if (dica == 1)      Write_LCD(0,1,"A - POTENCIAS");
    else if (dica == 2) Write_LCD(0,1,"B - SERIAL   ");
    else if (dica == 3) Write_LCD(0,1,"C - TEMPO    ");
    else if (dica == 4)
    {
        Write_LCD(0,1,"D - N/A      ");
        dica = 0;
    }
}

void dicas_tempo ( void )
{
    if(dica2 == 1) Write_LCD(0,1,"1 - 1s    2 - 5s");
    else if (dica2 == 2)
    {
        Write_LCD(0,1,"3 - 10s  4 - 30s");
        dica2 = 0;
    }
}
//***********************************************
