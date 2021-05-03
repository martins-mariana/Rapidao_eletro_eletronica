#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>

#define E0 0                                 					        //define o estado inicial
#define E1 1                                                            //define o estado no qual tem de haver a decisão sobre movimentar os cilindros quentes  
#define E2 2                                                            //define o estado no qual será movimentado o cilindro quente
#define E3 3                                                            //define o estado no qual tem de haver a decisão sobre movimentar cilindro frios
#define E4 4                                                            //define o estado no qual será movimentado o cilindro frio 
#define E5 5                                                            //define o estado no final e reinicio do fuso
 
if (wiringPiSetupGpio() == -1)                                          // Inicializa o sistema wiringPi para usar o pinos de GPIO
pinMode( 8, INPUT);                                                     // configura pino 8 como ENTRADA sensor de feedback balança
pinMode( 5, OUTPUT);                                                    // configura pino 5 como SAIDA
pinMode(20, OUTPUT);    //c                                             // configura pino 5 como SAIDA
pinMode(19, OUTPUT);    //b                                             // configura pino 5 como SAIDA
pinMode(16, OUTPUT);    //a                                             // configura pino 5 como SAIDA
pinMode(12,PWM_OUTPUT);                                                 // seta o modo do pino para INPUT, OUTPUT ou PWM_OUTPUT
pwmSetMode(PWM_MODE_MS);                                                // Configura o PWM para o modo mark:space
 
volatile char ControleMaquinaEstado;                                    //variável responsável por armazenar o estado atual da máquina de estados
void ExecutaMaquinaDeEstados(void);                                     //função que contém o switch/case com a máquina de estados
void ExecutaMaquinaDeEstados(void)                                      
{
	
	switch(ControleMaquinaEstado)                                       //executa a máquina de estados
    {
    case E0:
    {
		digitalWrite(20, LOW);                                          // c na seleção do demux
		digitalWrite(19, HIGH);          //MOTOR DO FUSO                // b na seleção do demux
		digitalWrite(16, LOW);                                          // a na seleção do demux
		if(q != 0 || f != 0) 
		{                                                               // muda de estado casa haja algum pedidO 
			ControleMaquinaEstado = E1;                                 // passa para proximo estado, movimenta o fuso da posição inicial ate o cilindro quente 
		    int seg = 0;                                                // contador de segundo iniciado em zero 
		    pwmSetClock(2400);  // valor para 1000Hz                    // seta o divisor para o clock do PWM
		    pwmSetRange (8);    // valor para 1000Hz                    // seta o registrador de intervalo no gerador de PWM
		    do
		    {
				digitalWrite( 5, HIGH);                                 // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário                  
				pwmWrite (12, 5);                                       // excreve o sinal pwm no pino 12,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo , 1000 x 1400ms = 1400 passo = 350mm
				Sleep(1);                                               // programa aguarda 1ms antes de incrementar a variavel segundo
				seg++;                                                  // conta q quantidade de ms incrementados
			}while( seg <= 1400);                                       // executa o pulso enquanto o contador não atinge o tempo                                                                     
	    }
	    else
	    {
			ControleMaquinaEstado = E0;                                 // continua no estado atual 
		}
        break;
    }
    case E1:
    {
		digitalWrite(20, LOW);                                          // c na seleção do demux
		digitalWrite(19, HIGH);          //MOTOR DO FUSO                // b na seleção do demux
		digitalWrite(16, LOW);                                          // a na seleção do demux
		
		if(q == 0)
		{                                                               // somente muda de estado se a tecla s for a entrada
            ControleMaquinaEstado = E3;                                 // não deseja alimentos quentes, muda para o caister de frios
            int seg = 0;                                                // contador de segundo iniciado em zero 
		    pwmSetClock(2400);  // valor para 1000Hz                    // seta o divisor para o clock do PWM
		    pwmSetRange (8);    // valor para 1000Hz                    // seta o registrador de intervalo no gerador de PWM

			do
			{ 
				digitalWrite(5, HIGH);                                 // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
				pwmWrite (12, 5);                                       // excreve o sinal pwm no pino 12,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo , 1000 x 3200ms = 3200 passo = 800mm
				Sleep(1);                                               // programa aguarda 1ms antes de incrementar a variavel segundo
				seg++;                                                  // conta q quantidade de ms incrementados
			}while( seg <= 3200);                                       // executa o pulso enquanto o contador não atinge o tempo                                                                     
	    }														                                                                
        else
        {
            ControleMaquinaEstado = E2;                                 // mudou para estado E2, dejesa alimentos quetes
        }
        break;
    }
	case E2:
    {
		//ROTINA DE MOVIMENTO DO CILINDROS QUENTES AQUI                 // DEPOIS QUE TERMINA DE SERVIR MOVE O CILINDRO
		digitalWrite(20, LOW);                                          // c na seleção do demux
		digitalWrite(19, HIGH);          //MOTOR DO FUSO                // b na seleção do demux
		digitalWrite(16, LOW);                                          // a na seleção do demux
		ControleMaquinaEstado = E3;                                     // muda para estado E3, rotina de movimentar os cilindros de alimentos terminada 
		
		int seg = 0;                                                    // contador de segundo iniciado em zero                                     // Configura o PWM para o modo mark:space
		pwmSetClock(2400);  // valor para 1000Hz                        // seta o divisor para o clock do PWM
		pwmSetRange (8);   // valor para 1000Hz                         // seta o registrador de intervalo no gerador de PWM
		do
		{
			digitalWrite(5, HIGH);                                      // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
		    pwmWrite (12, 5);                                           // excreve o sinal pwm no pino 12,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo , 1000 x 3200ms = 3200 passos = 800mm
			Sleep(1);                                                   // programa aguarda 1ms antes de incrementar a variavel segundo
			seg++;                                                      // conta q quantidade de ms incrementados
		}while( seg <= 3200); 
		break;                                                          // executa o pulso enquanto o contador não atinge o tempo                                                                     
	}
    case E3: 
    {
		digitalWrite(20, LOW);                                          // c na seleção do demux
		digitalWrite(19, HIGH);          //MOTOR DO FUSO                // b na seleção do demux
		digitalWrite(16, LOW);                                          // a na seleção do demux
		if(f == 0)
		{                                                               // somente muda de estado se a tecla s for a entrada
            ControleMaquinaEstado = E5;                                 // não deseja alimentos quentes, muda para o caister de frios
            int seg = 0;                                                // contador de segundo iniciado em zero 
    	    pwmSetClock(2400);  // valor para 1000Hz                    // seta o divisor para o clock do PWM
		    pwmSetRange (8);    // valor para 1000Hz                    // seta o registrador de intervalo no gerador de PWM
			do
			{ 
				digitalWrite(5, HIGH);                                  // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
				pwmWrite (12, 5);                                       // excreve o sinal pwm no pino 12,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo , 1000 x 3200ms = 3200 passo = 800mm
				Sleep(1);                                               // programa aguarda 1ms antes de incrementar a variavel segundo
				seg++;                                                  // conta q quantidade de ms incrementados
			}
			while( seg <= 1400);                                        // executa o pulso enquanto o contador não atinge o tempo                                                                     
	    }														                                                                
        else
        {
            ControleMaquinaEstado = E4;                                 // mudou para estado E2, dejesa alimentos quetes
        }
        break;   
    }
    case E4:
    {
 		//ROTINA DE MOVIMENTO DO CILINDROS QUENTES AQUI                 // DEPOIS QUE TERMINA DE SERVIR MOVE O CILINDRO
        ControleMaquinaEstado = E5;                                     // movimenta o sufo para o ponto final
        digitalWrite(20, LOW);                                          // c na seleção do demux
		digitalWrite(19, HIGH);          //MOTOR DO FUSO                // b na seleção do demux
		digitalWrite(16, LOW);                                          // a na seleção do demux
		
		int seg = 0;                                                    // contador de segundo iniciado em zero 
		pwmSetClock(2400);  // valor para 1000Hz                        // seta o divisor para o clock do PWM
		pwmSetRange (8);    // valor para 1000Hz                        // seta o registrador de intervalo no gerador de PWM
		do
		{
			digitalWrite(5, HIGH);                                     // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
			pwmWrite (12, 5);                                           // excreve o sinal pwm no pino 12,com largura de pulso de 1ms e frequencia de 1000Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo , 100 x 14 = 1400 passo = 350mm
			Sleep(1);                                                   // programa aguarda 1ms antes de incrementar a variavel segundo
			seg++;                                                      // conta q quantidade de ms incrementados
		}while( seg <= 1400) ;                                          // executa o pulso enquanto o contador não atinge o tempo                                                                     
        break; 
    }
	case E5:
    {
		digitalWrite(20, LOW);                                          // c na seleção do demux
		digitalWrite(19, HIGH);          //MOTOR DO FUSO                // b na seleção do demux
		digitalWrite(16, LOW);                                          // a na seleção do demux
		
		if(digitalRead(8) == LOW)                                       // sensor de prato ponto , n digitado prato no local , permanece no estado
		{
			ControleMaquinaEstado = E5;                                 // continua no estado atual, ate retirar o prato.
        }
        else
        {
			ControleMaquinaEstado = E0;                                 // prato ok, volta ao estado inicial
		    int seg = 0;                                                // contador de segundo iniciado em zero 
		    pwmSetClock(2400);  // valor para 1000Hz                    // seta o divisor para o clock do PWM
		    pwmSetRange (8);    // valor para 1000Hz                    // seta o registrador de intervalo no gerador de PWM
			do
			{
				digitalWrite(5, LOW);                                   // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
				pwmWrite (12, 5);                                       // excreve o sinal pwm no pino 12,com largura de pulso de 1ms e frequencia de 1000Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo , 1000 x 6 = 6000 passo = 1500mm
				Sleep(1);                                               // programa aguarda 1ms antes de incrementar a variavel segundo
				seg++;                                                  // conta q quantidade de ms incrementados
			}
			while( seg <= 6000);                                        // executa o pulso enquanto o contador não atinge o tempo                                                                     
		}
		break;
    } 
  }
}

//programa principal de execução 
int main(int argc, char *argv[])
{
  ControleMaquinaEstado = E0;                                           //estado inicial da variável de estado 
  while(1)
  {
    ExecutaMaquinaDeEstados();                                          //executa "para sempre" a máquina de estados , verificar quem habilita essa rotina
  }
  system("PAUSE"); 
  return 0;
}

