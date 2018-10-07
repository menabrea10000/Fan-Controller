/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC16F1455
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"

uint16_t DUTYC = 0;
uint16_t ValoreADC = 0;
uint16_t Temp;

void main(void)
{
    SYSTEM_Initialize();
    ADC_SelectChannel(channel_AN3);
    PWM1_LoadDutyValue(DUTYC);

    while (1)
    {
        __delay_ms(1000);
        if (!LATCbits.LATC0){                     //Interruttore
            ADC_StartConversion();
            while(!ADC_IsConversionDone());
            ValoreADC = ADC_GetConversionResult();
            Temp = (ValoreADC - 82) / 4;          //Calcolo Temperatura MCP9701A

            if (Temp > 60){
                DUTYC = 0xFFFF;
            }
            else if (Temp < 30){
                DUTYC = 0x0000;
            }
            else {
                DUTYC = (34 * Temp) - 1024;       //Funzione Retta Temp/PWM
            }
        }
        else {
            ADC_StartConversion();
            while(!ADC_IsConversionDone());
            ValoreADC = ADC_GetConversionResult();
            if (ValoreADC <= 307){
                DUTYC = 0x0000;
            }
            else {
                DUTYC = ValoreADC;
            }
        }
        PWM1_LoadDutyValue(DUTYC);
        
        /*          Spegnimento Peripheral PWM
        if (DUTYC){
            if (!PWM1CONbits.PWM1OUT) PWM1CONbits.PWM1OE = 1;
            PWM1_LoadDutyValue(DUTYC);
        }
        else {
            if (PWM1CONbits.PWM1OUT) PWM1CONbits.PWM1OE = 0;
        }
        */
    }
}