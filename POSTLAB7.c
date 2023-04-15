//UNIVERSIDAD DEL VALLE DE GUATEMALA
//IE2023 Programación de Microcontroladores
//Autor:		Mónica Alfaro
//Compilador:	pic-as (v2.36), MPLABX (v6.00)
//
//Programa:	POSTLAB7  (2 SERVOS CON PWM Y LED SIN PWM)	
//
//				
//Dispositivo:	PIC16F887
//Hardware:	SERVOMOTORES, POTENCIOMETRO Y LED
//
//Creado:	       10 de abril , 2023
//Última modificación:   10 de abril , 2023



// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits    
#pragma config WDTE = OFF       // Watchdog Timer Enable bit  
#pragma config PWRTE = OFF      // Power-up Timer Enable bit  
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit    
#pragma config CP = OFF         // Code Protection bit  
#pragma config CPD = OFF        // Data Code Protection bit  
#pragma config BOREN = OFF      // Brown Out Reset Selection bits  
#pragma config IESO = OFF       // Internal External Switchover bit  
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
#pragma config LVP = ON         // Low Voltage Programming Enable bit 

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit 
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 

//Librerias  
#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000 //frecuencia del cristal en microsegundos
char    selector;  //Variable para identificar los potenciometros

/************************** VECTOR DE INTERRUPCIONES **************************/
//Interrupcion con timer2
void __interrupt()isr(void){
    
    //Acá cargamos los valores que permitirán el movimiento del potenciómetro
    //a través de una interrupción. 
    
    if(PIR1bits.ADIF == 1){ 
        
        if(selector == 1){
            CCPR1L = (ADRESH >> 1) + 128; //Se asigna valor del pot1 al servo1
        }
        else if(selector == 0){
            CCPR2L = (ADRESH >> 1) + 128; //Se asigna valor del pot2 al servo2h
        }
                
        PIR1bits.ADIF = 0;  //Se baja la banndera
        
    }

}
/*************************** CÓDIGO PRINCIPAL  ********************************/

void main(void){
    
//DEFINICIÓN DE ENTRADAS Y SALIDAS
//entradas analogicas en AN0 y AN1, las demas digitales
    ANSEL = 0b00000011; 
    ANSELH = 0x00;
    
//Dos entradas y las demas seran salidass
    TRISA = 0b00000111; 
    TRISC = 0x00; 
    TRISD = 0x00; 
    
    ADCON0 = 0b00001001; // ADC encendido, canal AN2 seleccionado
    ADCON1 = 0b00000000; // Justificado a la izquierda, VDD como referencia positiva, VSS como referencia negativa

    
//CONFIGURACIÓN ADC
//Modulo ADC                 
    ADCON1bits.ADFM = 0;    
    ADCON1bits.VCFG0 = 0;   
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADCS = 0b10; 
    ADCON0bits.CHS = 0;     
    __delay_us(50);
    ADCON0bits.ADON = 1;    
    
//CONFIGURACIÓN PWM
//Configuracion del PWM, entrada RC2 CCP1 
    TRISCbits.TRISC2 = 1;   
    TRISDbits.TRISD5 = 1;   
//Valor del PWM
    PR2 = 249;    //valor inicial de la oscilacion          
//Modo del PWM   
    CCP1CONbits.P1M = 0;    
    CCP1CONbits.CCP1M = 0b1100; 
    CCPR1L = 0x0f;          
    CCP2CONbits.CCP2M = 0;    
    CCP2CONbits.CCP2M = 0b1100; 
    CCPR1L = 0x0f;          
    CCPR2L = 0x0f;          
    
    CCP1CONbits.DC1B = 0;
    CCP2CONbits.DC2B0 = 0;
    CCP2CONbits.DC2B1 = 0;
    
    
 //CONFIGURACIÓN DE TIMERS   
 //Configuracion del Timmer2
    PIR1bits.TMR2IF = 0;     
    T2CONbits.T2CKPS = 0b11; 
    T2CONbits.TMR2ON = 1;    
    while(PIR1bits.TMR2IF == 0);    
    PIR1bits.TMR2IF = 0;
    TRISCbits.TRISC2 = 0;           
    TRISDbits.TRISD5 = 0;     
    
//CONFIGURACIÓN DE RELOJ   
//Configuracionn del reloj interno  
    OSCCONbits.IRCF = 0b0111 ;  
    OSCCONbits.SCS = 1;         
 
 //CONFIGURACIÓN DE INTERRUPCIONES   
 //configuracion interrupciones
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1; 
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    ADCON0bits.GO = 1;  
//Valor inicial a variable
    selector = 1;
/************************** LOOP PRINCIPAL ************************************/

//Loop 
    while (1) {
        
        //ADC LED
        
         ADCON0bits.GO_DONE = 1; // Iniciar la conversión ADC
        while(ADCON0bits.GO_DONE); // Esperar a que la conversión termine
        unsigned int resultado_adc = (ADRESH << 8) | ADRESL; // Leer el resultado del ADC
        unsigned int duty_cycle = resultado_adc >> 2; // Dividir por 4 para ajustar a 8 bits (256 niveles)

        for(int i = 0; i < 256; i++) { // Generar señal PWM manual
            if(i < duty_cycle) {
                PORTCbits.RC0 = 1; // Encender la LED
            } else {
                PORTCbits.RC0 = 0; // Apagar la LED
            }
        }
    
        
        
        //acaba adc led
        
        
        
        //Hacemos uso de los principios de ADC que vimos en el laboratorio 
        //anterior. 
        
        //Leer el valor del ADC de cada potenciómetro 
        if(ADCON0bits.GO == 0){   
          //Revisar el selector de canal
            if(ADCON0bits.CHS == 0){
                selector = 0;
                //Pasa a canal 1
                ADCON0bits.CHS = 1;  
            }
            else if(ADCON0bits.CHS == 1){
                selector = 1;
                //Pasa a canal 0
                ADCON0bits.CHS = 0;  
            }
            //Delay para estabilización de lectura
            __delay_us(50);     
            //Revisión de valor               
            ADCON0bits.GO = 1;
        }
    }
    return;   
}