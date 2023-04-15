//UNIVERSIDAD DEL VALLE DE GUATEMALA
//IE2023 Programación de Microcontroladores
//Autor:		Mónica Alfaro
//Compilador:	pic-as (v2.36), MPLABX (v6.00)
//
//Programa:	PRELAB7  (1 SERVOS CON PWM)	
//
//				
//Dispositivo:	PIC16F887
//Hardware:	LEDs en el puerto D, botones en el puerto B
//
//Creado:	       10 de abril , 2023
//Última modificación:   10 de abril , 2023


#include <xc.h>
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

void main() {
    TRISCbits.TRISC2 = 0; // Configurar RC2 como salida

    PR2 = 199; // Establecer el valor del período para 20 ms (5 kHz)
    T2CONbits.T2CKPS0 = 0; // Configurar el preescaler del Timer2 como 1:1
    T2CONbits.T2CKPS1 = 0;
    T2CONbits.TMR2ON = 1; // Habilitar el Timer2

    CCP1CONbits.CCP1M = 0b1100; // Configurar el módulo CCP1 para PWM
    CCP1CONbits.DC1B = 0; // Establecer el bit menos significativo del ciclo de trabajo como 0
    CCPR1L = 75; // Establecer el valor del ciclo de trabajo para 1,5 ms (90 grados)

    while(1) {
        // El servo se posicionará en 90 grados
    }
}
