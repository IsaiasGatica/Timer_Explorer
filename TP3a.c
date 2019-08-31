/* 
Plantilla inicial para
Código C 
*/

// Includes
#define FOSC 32000000        // Frecuencia del clock del micro
#define FCY FOSC/2          // Frecuencia de instrucciones usada por  "__delay_ms()"
#include <p24fj128ga010.h>
#include <math.h>
#include "LCD.h"
#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>

#define S6 PORTDbits.RD7        // Explorer 16 pulsador s6
#define S3 PORTDbits.RD6    // Explorer 16 pulsador s3 (pulsador ubicado a la izquierda)
#define S4 PORTDbits.RD13       // Explorer 16 pulsador s4 (pulsador ubicado a la derecha)
#define LED_D8 LATAbits.LATA5   // Explorer 16 LED D8
#define LED_D9 LATAbits.LATA6  // Explorer 16 LED D9
#define LED_D10 LATAbits.LATA7  // 
#define LED_D7 LATAbits.LATA4   // Explorer 16 LED D7


//Configuración de bits para Explorer-16
_CONFIG1( JTAGEN_OFF        // deshabilito interface JTAG
        & GCP_OFF           // deshabilito proteción de código general
        & GWRP_OFF          // deshabilito protección escritura flash
        & ICS_PGx2         // ICSP interface (2=default)
        & FWDTEN_OFF)       // deshabilito watchdog timer
_CONFIG2( IESO_OFF          // deshabilito 2 velocidades de start up
        & FCKSM_CSDCMD      // deshabilito clock-swithcing/monitor
        & FNOSC_PRIPLL      // primary oscillator: habilito PLL
        & POSCMOD_XT)       // primary oscillator: modo XT



//Definiciones
//Declaracion de variables
        unsigned char Bandera=0;

//Prototipos de funciones
        void Configiniciales();
        void ConfigTMR();
        void ConfigAD();
//Prototipos de atención a las interrupciónes
        
        void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
{
            LED_D9=0;
            LED_D8=0;
            LED_D10=1;
            __delay_ms(1000);
             IFS0bits.T3IF = 0; 
}
                
         void __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void)
{
            LED_D8=0;
            LED_D10=0;
            LED_D9=1;
            __delay_ms(250);
             IFS1bits.T5IF = 0; 
}       
         void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
             if (S3==0){
                 
              IPC2bits.T3IP=0b110;      //TIMER3
              IPC7bits.T5IP=0b101;     //TIMER5
              SRbits.IPL=0b001;         //CPU
             
                 
             }
             if (S6==0){
              IPC2bits.T3IP=0b110;      //TIMER3
              SRbits.IPL=0b101;         //CPU
              IPC7bits.T5IP=0b001;     //TIMER5
              
                 
                 
             }
             
             if (S4==0){
              IPC7bits.T5IP=0b110;     //TIMER5
              IPC2bits.T3IP=0b101;      //TIMER3
              SRbits.IPL=0b001;         //CPU
                 
                 
             }
             
IFS1bits.CNIF=0;
}
         
         
        
        
        

//Programa principal


int main(void)
{
    Configiniciales();
    ConfigTMR();
    ConfigAD();
    //InitLCD();
  
    
    
    while (1){
        
        LED_D9=0;
        LED_D10=0;
                
        LED_D8=1;
   

        
    }
    
    
   


}


void Configiniciales(){
    
    AD1PCFG = 0xFFFF;
    TRISDbits.TRISD6=1;
    TRISDbits.TRISD7=1;
    TRISDbits.TRISD13=1;
    TRISDbits.TRISD7=1;
    TRISAbits.TRISA5=0;
    TRISAbits.TRISA6=0;
    TRISAbits.TRISA7=0;
    
    
    CNEN1bits.CN15IE=1; //Change interruption asociado al Puerto D6
    CNEN2bits.CN16IE=1; //Change interruption asociado al Puerto D7
    CNEN2bits.CN19IE=1; //Change interruption asociado al Puerto D13
    IPC4bits.CNIP=111; //Prioridad para Change interruption (CONSULTAR_SOLO PROTEUS ¿?)
    
    IEC1bits.CNIE=1; //Interrupciones por CN habilitada
    IFS1bits.CNIF=0; //Bandera de CN en cero
    
    
  

}

void ConfigAD(){
    AD1CON1bits.FORM=0b01;
    
    
    
    
    
    
    
    
    
}



void ConfigTMR(){
    
    
//TIMER3 interrumpe cada 1 segundo
    
//T3CON = 0x00;
//TMR3 = 0x00; 
//PR3 = 62500; 
//IPC2bits.T3IP = 0x01;
//IFS0bits.T3IF = 0; 
//IEC0bits.T3IE = 1; 
//T3CONbits.TCKPS=11;
//T3CONbits.TON = 1; 


//Interrupcion cada 2 segundos, con un registro de 32 bits, TMR2+TMR3    
    
    
T2CON = 0x00; 
T3CON = 0x00;
TMR3 = 0x00; 
TMR2 = 0x00; 
PR3 = 0b0000000000000001; 
PR2 = 0b1110100001001000; 
IPC2bits.T3IP = 0x01; //Nivel de prioridad del TMR3
IFS0bits.T3IF = 0; //Limpio bandera del TMR3
IEC0bits.T3IE = 1; //Habilito interrupcion del TMR3
T2CONbits.T32 = 1; //Habilito modo de 32bit
T2CONbits.TCKPS=0b11; //prescaler en 256
T2CONbits.TCS=0;    //Fosc/2
T2CONbits.TON = 1; //Inicio 32-bit


//Interrupcion cada x segundos, con un registro de 32 bits, TMR4+TMR5  

         
//T4CON = 0x00; 
//T5CON = 0x00;
//TMR4 = 0x00; 
//TMR5 = 0x00; 
//PR5 = 0000000000000000; 
//PR4 = 1110100001001000; 
//IPC7bits.T5IP = 0x01; 
//IFS1bits.T5IF = 0; 
//IEC1bits.T5IE = 1; 
//T4CONbits.T32 = 1; 
//T4CONbits.TCKPS=11;
//T4CONbits.TON = 1; 
         

//TIMER5 interrumpe cada 500m segundo
    
T5CON = 0x00;
TMR5 = 0x00; 
PR5 = 31250; 
IPC7bits.T5IP = 0x01; 
IFS1bits.T5IF = 0; 
IEC1bits.T5IE = 1; 
T5CONbits.TCKPS=0b11; 
T2CONbits.TCS=0; 
T5CONbits.TON = 1;    
        

//SRbits.IPL=100; //Prioridad de la CPU, en esta caso de 4










    

}

