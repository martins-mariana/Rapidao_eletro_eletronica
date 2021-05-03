#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>

int main (void)
{
	int segundo = 0;                                                    // contador de segundo iniciado em zero 
    if (wiringPiSetupGpio() == -1)                                      // Inicializa o sistema wiringPi para usar o pinos de GPIO
    pinMode(19, OUTPUT);                                                // configura pino 19 como SAIDA
    pinMode(20,PWM_OUTPUT);                                             // seta o modo do pino para INPUT, OUTPUT ou PWM_OUTPUT
    pwmSetMode(PWM_MODE_MS);                                            // Configura o PWM para o modo mark:space
                                                                        // freq. da placa= 19.2MHz, logo colcoando pwmClock= 2400 e pwmRange= 8 , // PwmFrequency =19.2MHz/pwmClock/pwmRange
    pwmSetClock(2400);  // valor para 14,2857 Hz                        // seta o divisor para o clock do PWM para 2400Hz
    pwmSetRange (560);  // valor para 14,2857 Hz                        // seta o registrador de intervalo no gerador de PWM para 560Hz
 
     do{
		digitalWrite(5, HIGH);                                         // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
	    pwmWrite (12, 5);                                               // excreve o sinal pwm no pino 18,com largura de pulso de 10ms e frequencia de 100Hz
                                                                        // cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
        Sleep(1);                                                       // programa aguarda 1ms antes de incrementar a variavel segundo
        segundo++;                                                      // conta q quantidade de ms incrementados
    }
    while( segundo <= 1750);                                            // executa o pulso enquanto o contador não atinge o tempo                                                                     
  
  
  return 0;
}
