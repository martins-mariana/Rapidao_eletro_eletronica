include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>

int main (void)
{ 
	
    if (wiringPiSetupGpio() == -1)                                      // Inicializa o sistema wiringPi para usar o pinos de GPIO
    pinMode(5, OUTPUT);                                                 // configura pino 5 como SAIDA
    pinMode(12,PWM_OUTPUT);                                             // seta o modo do pino para INPUT, OUTPUT ou PWM_OUTPUT
    pwmSetMode(PWM_MODE_MS);                                            // Configura o PWM para o modo mark:space
    
    int inst = 0;                                                       // contador de ms iniciado em zero                                                                     // freq. da placa= 19.2MHz, logo colcoando pwmClock= 2400 e pwmRange= 8 , // PwmFrequency =19.2MHz/pwmClock/pwmRange
    pwmSetClock(2400);  // valor para 20 Hz                             // seta o divisor para o clock do PWM para 2400Hz
    pwmSetRange (400);  // valor para 20 Hz                             // seta o registrador de intervalo no gerador de PWM para 400Hz
 
     do{
		digitalWrite(5, HIGH);                                          // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
	    pwmWrite (12, 5);                                               // excreve o sinal pwm no pino 12 
                                                                        // foi definido 6 rpm como velocidade. Precizamos girar 22,5 graus , logo 12,5 passos como 20 hz de frequencia durante 625ms
        Sleep(1);                                                       // programa aguarda 1ms antes de incrementar a variavel segundo
        inst++;                                                         // conta q quantidade de ms incrementados
    }
    while( inst <= 625);                                                // executa o pulso enquanto o contador não atinge o tempo                                                                     
  return 0;
}
