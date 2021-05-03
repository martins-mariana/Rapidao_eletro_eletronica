#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>

#define Ef1 1                                 					        // alimento quente 1
#define Ef2 2                                                           // servir aliemnto quente 1
#define Ef3 3                                                           // alimento quente 2
#define Ef4 4                                                           // servir alimento quente 2
#define Ef5 5                                                           // alimento quente 3
#define Ef6 6                                                           // servir alimento quente 3
#define Ef7 7                                                           // alimento quente 4  
#define Ef8 8                                                           // servir alimento quente 4
#define Ef9 9                                                           // alimento quente 5
#define Ef10 10                                                         // servir alimento quente 5
#define Ef11 11                                                         // alimento quente 6
#define Ef12 12                                                         // servir alimento quente 6
#define Ef13 13                                                         // alimento quente 7
#define Ef14 14                                                         // servir alimento quente 7
#define Ef15 15                                                         // alimento quente 8
#define Ef16 16                                                         // servir alimento quente 8
 
if (wiringPiSetupGpio() == -1)                                          // Inicializa o sistema wiringPi para usar o pinos de GPIO
pinMode(5, OUTPUT);                                                    // configura pino 19 como SAIDA
pinMode(24, OUTPUT);                                                    // configura pino 24 como SAIDA 
pinMode(8, INPUT);                                                      // configura pino 8 como ENTRADA
pinMode(12,PWM_OUTPUT);                                                 // seta o modo do pino para INPUT, OUTPUT ou PWM_OUTPUT
pwmSetMode(PWM_MODE_MS);                                                // Configura o PWM para o modo mark:space

   
volatile char Control_fsm_frio;                                      	//variável responsável por armazenar o estado atual da máquina de estados
	void Executa_fsm_frio(void);                                          	//função que contém o switch/case com a máquina de estados
		
		void Executa_fsm_frio(void)                                      
		{
			switch(Control_fsm_frio)                                    //executa a máquina de estados
			{ 
				case Ef1:
				{
					if(frios[0]== 's')                                  //deseja alimento 0.
					{                                                           
						Control_fsm_frio = Ef2;	                        // muda para estado de servir o alimento 0
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, LOW);  //MOVE MOTOR PRATO      // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
						int inst = 0;                                   // rotina de movimentaro motor do prato                                                                    // freq. da placa= 19.2MHz, logo colcoando pwmClock= 2400 e pwmRange= 8 , // PwmFrequency =19.2MHz/pwmClock/pwmRange
						pwmSetClock(2400);  // valor para 20 Hz         // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (400);  // valor para 20 Hz         // seta o registrador de intervalo no gerador de PWM para 400Hz
						do{
							digitalWrite(5, HIGH);                      // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                           // excreve o sinal pwm no pino 12, para mover o motor do prato 
																		// foi definido 6 rpm como velocidade. Precizamos girar 22,5 graus , logo 12,5 passos como 20 hz de frequencia durante 625ms
							Sleep(1);                                   // programa aguarda 1ms antes de incrementar a variavel segundo
							inst++;                                     // conta q quantidade de ms incrementados
						}
						while( inst <= 625);                            // executa o pulso enquanto o contador não atinge o tempo
					}
					else if(frios[1]=='n');                             // não deseja alimento 0
					{
						Control_fsm_frio = Ef3;                         // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);   //MOVE CILINDRO FRIO  // b na seleção do demux 
						digitalWrite(16, LOW);                          // a na seleção do demux 
						int segundo = 0;                                // contador de segundo iniciado em zero
						pwmSetClock(2400);  // valor para 14,2857 Hz    // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (560);  // valor para 14,2857 Hz    // seta o registrador de intervalo no gerador de PWM para 560Hz
						do
						{
							digitalWrite(5, HIGH);                     // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                           // excreve o sinal pwm no pino 18,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
							Sleep(1);                                   // programa aguarda 1ms antes de incrementar a variavel segundo
							segundo++;                                  // conta q quantidade de ms incrementado
						}
						while( segundo <= 1750);                        // executa o pulso enquanto o contador não atinge o tempo 
					}
					break;
				}
				case Ef2:                                               // estado de servir o alimento 0
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_frio = Ef3;                             // iniciar a rotina de mover o cilindro
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH); //MOVE CILINDRO FRIO  		// b na seleção do demux 
					digitalWrite(16, LOW);                         		// a na seleção do demux 
					int segundo = 0;                                    // contador de segundo iniciado em zero
					pwmSetClock(2400);  // valor para 14,2857 Hz        // seta o divisor para o clock do PWM para 2400Hz
					pwmSetRange (560);  // valor para 14,2857 Hz        // seta o registrador de intervalo no gerador de PWM para 560Hz
					do
					{
						digitalWrite(5, HIGH);                          // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
						pwmWrite (12, 5);                               // excreve o sinal pwm no pino 12,com largura de pulso de 10ms e frequencia de 100Hz
																	    // cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
					Sleep(1);                                           // programa aguarda 1ms antes de incrementar a variavel segundo
					segundo++;                                          // conta q quantidade de ms incrementado
					}
					while( segundo <= 1750);                            // executa o pulso enquanto o contador não atinge o tempo 	
					break;
				}
				case Ef3:
				{
					if(frios[1]== 's')                                  //deseja alimento 0.
					{                                                           
						Control_fsm_frio = Ef4;	                        // muda para estado de servir o alimento 0
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, LOW);    //move o prato        // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
						int inst = 0;                                   // rotina de movimentaro motor do prato                                                                    // freq. da placa= 19.2MHz, logo colcoando pwmClock= 2400 e pwmRange= 8 , // PwmFrequency =19.2MHz/pwmClock/pwmRange
						pwmSetClock(2400);  // valor para 20 Hz         // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (400);  // valor para 20 Hz         // seta o registrador de intervalo no gerador de PWM para 400Hz
						do{
							digitalWrite(5, HIGH);                      // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                           // excreve o sinal pwm no pino 12, para mover o motor do prato 
																		// foi definido 6 rpm como velocidade. Precizamos girar 22,5 graus , logo 12,5 passos como 20 hz de frequencia durante 625ms
							Sleep(1);                                   // programa aguarda 1ms antes de incrementar a variavel segundo
							inst++;                                     // conta q quantidade de ms incrementados
						}
						while( inst <= 625);                            // executa o pulso enquanto o contador não atinge o tempo
					}
					else if(frios[1]=='n');                             // não deseja alimento 0
					{
						Control_fsm_frio = Ef5;                         // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);   //MOVE CILINDRO FRIO  // b na seleção do demux 
						digitalWrite(16, LOW);                          // a na seleção do demux 
						int segundo = 0;                                // contador de segundo iniciado em zero
						pwmSetClock(2400);  // valor para 14,2857 Hz    // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (560);  // valor para 14,2857 Hz    // seta o registrador de intervalo no gerador de PWM para 560Hz
						do
						{
							digitalWrite(5, HIGH);                     // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                           // excreve o sinal pwm no pino 18,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
							Sleep(1);                                   // programa aguarda 1ms antes de incrementar a variavel segundo
							segundo++;                                  // conta q quantidade de ms incrementado
						}
						while( segundo <= 1750);                        // executa o pulso enquanto o contador não atinge o tempo 
					}
					break;
				}		  
				case Ef4:
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_frio = Ef5;                             // iniciar a rotina de mover o cilindro
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH); //MOVE CILINDRO FRIO  		// b na seleção do demux 
					digitalWrite(16, LOW);                         		// a na seleção do demux 
					int segundo = 0;                                    // contador de segundo iniciado em zero
					pwmSetClock(2400);  // valor para 14,2857 Hz        // seta o divisor para o clock do PWM para 2400Hz
					pwmSetRange (560);  // valor para 14,2857 Hz        // seta o registrador de intervalo no gerador de PWM para 560Hz
					do
					{
						digitalWrite(5, HIGH);                          // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
						pwmWrite (12, 5);                               // excreve o sinal pwm no pino 12,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
						Sleep(1);                                       // programa aguarda 1ms antes de incrementar a variavel segundo
						segundo++;                                      // conta q quantidade de ms incrementado
						}
						while( segundo <= 1750);                        // executa o pulso enquanto o contador não atinge o tempo 	
					break;
				}  
				case Ef5:
				{
					if(frios[2]== 's')                                  //deseja alimento
					{                                                           
						Control_fsm_frio = Ef6;	                        // muda para estado de servir o alimento
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, LOW);                          // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
						int inst = 0;                                   // rotina de movimentaro motor do prato                                                                    // freq. da placa= 19.2MHz, logo colcoando pwmClock= 2400 e pwmRange= 8 , // PwmFrequency =19.2MHz/pwmClock/pwmRange
						pwmSetClock(2400);  // valor para 20 Hz         // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (400);  // valor para 20 Hz         // seta o registrador de intervalo no gerador de PWM para 400Hz
						do{
							digitalWrite(5, HIGH);                      // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                           // excreve o sinal pwm no pino 12, para mover o motor do prato 
																		// foi definido 6 rpm como velocidade. Precizamos girar 22,5 graus , logo 12,5 passos como 20 hz de frequencia durante 625ms
							Sleep(1);                                   // programa aguarda 1ms antes de incrementar a variavel segundo
							inst++;                                     // conta q quantidade de ms incrementados
						}
						while( inst <= 625);                            // executa o pulso enquanto o contador não atinge o tempo
					}
					else if(frios[2]=='n');                             // não deseja alimento
					{
						Control_fsm_frio = Ef7;                         // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);   //MOVE CILINDRO FRIO  // b na seleção do demux 
						digitalWrite(16, LOW);                          // a na seleção do demux 
						int segundo = 0;                                // contador de segundo iniciado em zero
						pwmSetClock(2400);  // valor para 14,2857 Hz    // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (560);  // valor para 14,2857 Hz    // seta o registrador de intervalo no gerador de PWM para 560Hz
						do
						{
							digitalWrite(5, HIGH);                      // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                           // excreve o sinal pwm no pino 18,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
							Sleep(1);                                   // programa aguarda 1ms antes de incrementar a variavel segundo
							segundo++;                                  // conta q quantidade de ms incrementado
						}
						while( segundo <= 1750);                        // executa o pulso enquanto o contador não atinge o tempo 
					}
					break;
				}
				case Ef6:
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_frio= Ef7;                              // iniciar a rotina de mover o cilindro
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH); //MOVE CILINDRO FRIO  		// b na seleção do demux 
					digitalWrite(16, LOW);                         		// a na seleção do demux 
					int segundo = 0;                                    // contador de segundo iniciado em zero
					pwmSetClock(2400);  // valor para 14,2857 Hz        // seta o divisor para o clock do PWM para 2400Hz
					pwmSetRange (560);  // valor para 14,2857 Hz        // seta o registrador de intervalo no gerador de PWM para 560Hz
					do
					{
						digitalWrite(5, HIGH);                          // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
						pwmWrite (12, 5);                               // excreve o sinal pwm no pino 12,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
						Sleep(1);                                       // programa aguarda 1ms antes de incrementar a variavel segundo
						segundo++;                                      // conta q quantidade de ms incrementado
					}
					while( segundo <= 1750);                            // executa o pulso enquanto o contador não atinge o tempo 	
					break;
				}
				case Ef7:
				{
					if(frios[3]== 's')                                  //deseja alimento
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, LOW);  //MOVE MOTOR PRATO      // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
						int inst = 0;                                   // rotina de movimentaro motor do prato                                                                    // freq. da placa= 19.2MHz, logo colcoando pwmClock= 2400 e pwmRange= 8 , // PwmFrequency =19.2MHz/pwmClock/pwmRange
						pwmSetClock(2400);  // valor para 20 Hz         // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (400);  // valor para 20 Hz         // seta o registrador de intervalo no gerador de PWM para 400Hz
						do{
							digitalWrite(5, HIGH);                      // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                           // excreve o sinal pwm no pino 12, para mover o motor do prato 
																		// foi definido 6 rpm como velocidade. Precizamos girar 22,5 graus , logo 12,5 passos como 20 hz de frequencia durante 625ms
							Sleep(1);                                   // programa aguarda 1ms antes de incrementar a variavel segundo
							inst++;                                     // conta q quantidade de ms incrementados
						}
						while( inst <= 625);                            // executa o pulso enquanto o contador não atinge o tempo
					{                                                           
						Control_fsm_frio = Ef8;	                        // muda para estado de servir o alimento 
					}
					else if(frios[3]=='n');                             // não deseja alimento
					{
						Control_fsm_frio = Ef9;                         // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);   //MOVE CILINDRO FRIO  // b na seleção do demux 
						digitalWrite(16, LOW);                          // a na seleção do demux 
						int segundo = 0;                                // contador de segundo iniciado em zero
						pwmSetClock(2400);  // valor para 14,2857 Hz    // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (560);  // valor para 14,2857 Hz    // seta o registrador de intervalo no gerador de PWM para 560Hz
						do
						{
							digitalWrite(5, HIGH);                             // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                                   // excreve o sinal pwm no pino 18,com largura de pulso de 10ms e frequencia de 100Hz
																				// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
							Sleep(1);                                           // programa aguarda 1ms antes de incrementar a variavel segundo
							segundo++;                                          // conta q quantidade de ms incrementado
						}
						while( segundo <= 1750);                                // executa o pulso enquanto o contador não atinge o tempo 
					}
					break;
				}
				case Ef8:
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_frio = Ef9;                             // iniciar a rotina de mover o cilindro
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH); //MOVE CILINDRO FRIO  		// b na seleção do demux 
					digitalWrite(16, LOW);                         		// a na seleção do demux 
					int segundo = 0;                                    // contador de segundo iniciado em zero
					pwmSetClock(2400);  // valor para 14,2857 Hz        // seta o divisor para o clock do PWM para 2400Hz
					pwmSetRange (560);  // valor para 14,2857 Hz        // seta o registrador de intervalo no gerador de PWM para 560Hz
					do
					{
						digitalWrite(5, HIGH);                          // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
						pwmWrite (12, 5);                               // excreve o sinal pwm no pino 12,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
						Sleep(1);                                       // programa aguarda 1ms antes de incrementar a variavel segundo
						segundo++;                                      // conta q quantidade de ms incrementado
					}
					while( segundo <= 1750);                            // executa o pulso enquanto o contador não atinge o tempo 
					break;                              
				}
				case Ef9:
				{ 
					if(frios[4]== 's')                                  //deseja alimento
					{                                                           
						Control_fsm_frio = Ef10;                        // muda para estado de servir o alimento
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, LOW);  //MOVE MOTOR PRATO      // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
						int inst = 0;                                   // rotina de movimentaro motor do prato                                                                    // freq. da placa= 19.2MHz, logo colcoando pwmClock= 2400 e pwmRange= 8 , // PwmFrequency =19.2MHz/pwmClock/pwmRange
						pwmSetClock(2400);  // valor para 20 Hz         // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (400);  // valor para 20 Hz         // seta o registrador de intervalo no gerador de PWM para 400Hz
						do{
							digitalWrite(5, HIGH);                      // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                           // excreve o sinal pwm no pino 12, para mover o motor do prato 
																		// foi definido 6 rpm como velocidade. Precizamos girar 22,5 graus , logo 12,5 passos como 20 hz de frequencia durante 625ms
							Sleep(1);                                   // programa aguarda 1ms antes de incrementar a variavel segundo
							inst++;                                     // conta q quantidade de ms incrementados
						}
						while( inst <= 625);                            // executa o pulso enquanto o contador não atinge o tempo 
					}
					else if(frios[4]=='n');                             // não deseja alimento
					{
						Control_fsm_frio = Ef11;                        // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);   //MOVE CILINDRO FRIO  // b na seleção do demux 
						digitalWrite(16, LOW);                          // a na seleção do demux 
						int segundo = 0;                                // contador de segundo iniciado em zero
						pwmSetClock(2400);  // valor para 14,2857 Hz    // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (560);  // valor para 14,2857 Hz    // seta o registrador de intervalo no gerador de PWM para 560Hz
						do
						{
							digitalWrite(5, HIGH);                      // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                           // excreve o sinal pwm no pino 18,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
							Sleep(1);                                   // programa aguarda 1ms antes de incrementar a variavel segundo
							segundo++;                                  // conta q quantidade de ms incrementado
						}
						while( segundo <= 1750);                        // executa o pulso enquanto o contador não atinge o tempo 
					}
					break;
				}
				case Ef10:
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_frio = Ef11;                            // iniciar a rotina de mover o cilindro
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH); //MOVE CILINDRO FRIO  		// b na seleção do demux 
					digitalWrite(16, LOW);                         		// a na seleção do demux 
					int segundo = 0;                                    // contador de segundo iniciado em zero
					pwmSetClock(2400);  // valor para 14,2857 Hz        // seta o divisor para o clock do PWM para 2400Hz
					pwmSetRange (560);  // valor para 14,2857 Hz        // seta o registrador de intervalo no gerador de PWM para 560Hz
					do
					{
						digitalWrite(5, HIGH);                          // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
						pwmWrite (12, 5);                               // excreve o sinal pwm no pino 12,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
						Sleep(1);                                       // programa aguarda 1ms antes de incrementar a variavel segundo
						segundo++;                                      // conta q quantidade de ms incrementado
					}
					while( segundo <= 1750);                            // executa o pulso enquanto o contador não atinge o tempo 	
					break;    
				}
				case Ef11:
				{
					if(frios[5]== 's')                                  //deseja alimento
					{                                                           
						Control_fsm_frio = Ef12;                        // muda para estado de servir o alimento
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, LOW);  //MOVE MOTOR PRATO      // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
						int inst = 0;                                   // rotina de movimentaro motor do prato                                                                    // freq. da placa= 19.2MHz, logo colcoando pwmClock= 2400 e pwmRange= 8 , // PwmFrequency =19.2MHz/pwmClock/pwmRange
						pwmSetClock(2400);  // valor para 20 Hz         // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (400);  // valor para 20 Hz         // seta o registrador de intervalo no gerador de PWM para 400Hz
						do{
							digitalWrite(5, HIGH);                      // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                           // excreve o sinal pwm no pino 12, para mover o motor do prato 
																		// foi definido 6 rpm como velocidade. Precizamos girar 22,5 graus , logo 12,5 passos como 20 hz de frequencia durante 625ms
							Sleep(1);                                   // programa aguarda 1ms antes de incrementar a variavel segundo
							inst++;                                     // conta q quantidade de ms incrementados
						}
						while( inst <= 625);                            // executa o pulso enquanto o contador não atinge o tempo  
					}
					else if(frios[5]=='n');                             // não deseja alimento
					{
						Control_fsm_frio = Ef13;                        // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);   //MOVE CILINDRO FRIO  // b na seleção do demux 
						digitalWrite(16, LOW);                          // a na seleção do demux 
						int segundo = 0;                                // contador de segundo iniciado em zero
						pwmSetClock(2400);  // valor para 14,2857 Hz    // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (560);  // valor para 14,2857 Hz    // seta o registrador de intervalo no gerador de PWM para 560Hz
						do
						{
							digitalWrite(5, HIGH);                      // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                           // excreve o sinal pwm no pino 18,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
							Sleep(1);                                   // programa aguarda 1ms antes de incrementar a variavel segundo
							segundo++;                                  // conta q quantidade de ms incrementado
						}
						while( segundo <= 1750);                        // executa o pulso enquanto o contador não atinge o tempo 
					}
					break;
				}
				case Ef12:
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_frio = Ef13;                            // iniciar a rotina de mover o cilindro
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH); //MOVE CILINDRO FRIO  		// b na seleção do demux 
					digitalWrite(16, LOW);                         		// a na seleção do demux 
					int segundo = 0;                                    // contador de segundo iniciado em zero
					pwmSetClock(2400);  // valor para 14,2857 Hz        // seta o divisor para o clock do PWM para 2400Hz
					pwmSetRange (560);  // valor para 14,2857 Hz        // seta o registrador de intervalo no gerador de PWM para 560Hz
					do
					{
						digitalWrite(5, HIGH);                          // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
						pwmWrite (12, 5);                               // excreve o sinal pwm no pino 12,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
						Sleep(1);                                       // programa aguarda 1ms antes de incrementar a variavel segundo
						segundo++;                                      // conta q quantidade de ms incrementado
					}
					while( segundo <= 1750);                            // executa o pulso enquanto o contador não atinge o tempo 	
					break;    
				}
				case Ef13:
				{
					if(frios[6]== 's')                                  //deseja alimento
					{                                                           
						Control_fsm_frio = Ef14;                        // muda para estado de servir o alimento
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, LOW);  //MOVE MOTOR PRATO      // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
						int inst = 0;                                   // rotina de movimentaro motor do prato                                                                    // freq. da placa= 19.2MHz, logo colcoando pwmClock= 2400 e pwmRange= 8 , // PwmFrequency =19.2MHz/pwmClock/pwmRange
						pwmSetClock(2400);  // valor para 20 Hz         // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (400);  // valor para 20 Hz         // seta o registrador de intervalo no gerador de PWM para 400Hz
						do{
							digitalWrite(5, HIGH);                      // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                           // excreve o sinal pwm no pino 12, para mover o motor do prato 
																		// foi definido 6 rpm como velocidade. Precizamos girar 22,5 graus , logo 12,5 passos como 20 hz de frequencia durante 625ms
							Sleep(1);                                   // programa aguarda 1ms antes de incrementar a variavel segundo
							inst++;                                     // conta q quantidade de ms incrementados
						}
						while( inst <= 625);                            // executa o pulso enquanto o contador não atinge o tempo 
					}
					else if(frios[6]=='n');                             // não deseja alimento
					{
						Control_fsm_frio = Ef15;                        // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);   //MOVE CILINDRO FRIO  // b na seleção do demux 
						digitalWrite(16, LOW);                          // a na seleção do demux 
						int segundo = 0;                                // contador de segundo iniciado em zero
						pwmSetClock(2400);  // valor para 14,2857 Hz    // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (560);  // valor para 14,2857 Hz    // seta o registrador de intervalo no gerador de PWM para 560Hz
						do
						{
							digitalWrite(5, HIGH);                      // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                           // excreve o sinal pwm no pino 18,com largura de pulso de 10ms e frequencia de 100Hz
																	    // cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
							Sleep(1);                                   // programa aguarda 1ms antes de incrementar a variavel segundo
							segundo++;                                  // conta q quantidade de ms incrementado
						}
						while( segundo <= 1750);                        // executa o pulso enquanto o contador não atinge o tempo 
					}
					break;
				}
				case Ef14:
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_frio = Ef15;                            // iniciar a rotina de mover o cilindro
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH); //MOVE CILINDRO FRIO  		// b na seleção do demux 
					digitalWrite(16, LOW);                         		// a na seleção do demux 
					int segundo = 0;                                    // contador de segundo iniciado em zero
					pwmSetClock(2400);  // valor para 14,2857 Hz        // seta o divisor para o clock do PWM para 2400Hz
					pwmSetRange (560);  // valor para 14,2857 Hz        // seta o registrador de intervalo no gerador de PWM para 560Hz
					do
					{
						digitalWrite(5, HIGH);                          // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
						pwmWrite (12, 5);                               // excreve o sinal pwm no pino 12,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
						Sleep(1);                                       // programa aguarda 1ms antes de incrementar a variavel segundo
						segundo++;                                      // conta q quantidade de ms incrementado
					}
					while( segundo <= 1750);                            // executa o pulso enquanto o contador não atinge o tempo 	
					break;  
				}
				case Ef15:
				{
					if(frios[7]== 's')                                  //deseja alimento
					{                                                           
						Control_fsm_frio = Ef16;                        // muda para estado de servir o alimento
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, LOW);  //MOVE MOTOR PRATO      // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
						int inst = 0;                                   // rotina de movimentaro motor do prato                                                                    // freq. da placa= 19.2MHz, logo colcoando pwmClock= 2400 e pwmRange= 8 , // PwmFrequency =19.2MHz/pwmClock/pwmRange
						pwmSetClock(2400);  // valor para 20 Hz         // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (400);  // valor para 20 Hz         // seta o registrador de intervalo no gerador de PWM para 400Hz
						do{
							digitalWrite(5, HIGH);                      // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                           // excreve o sinal pwm no pino 12, para mover o motor do prato 
																		// foi definido 6 rpm como velocidade. Precizamos girar 22,5 graus , logo 12,5 passos como 20 hz de frequencia durante 625ms
							Sleep(1);                                   // programa aguarda 1ms antes de incrementar a variavel segundo
							inst++;                                     // conta q quantidade de ms incrementados
						}
						while( inst <= 625);                            // executa o pulso enquanto o contador não atinge o tempo 
					}
					else if(frios[7]=='n');                             // não deseja alimento
					{
						Control_fsm_frio = Ef1;                         // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);   //MOVE CILINDRO FRIO  // b na seleção do demux 
						digitalWrite(16, LOW);                          // a na seleção do demux 
						int segundo = 0;                                // contador de segundo iniciado em zero
						pwmSetClock(2400);  // valor para 14,2857 Hz    // seta o divisor para o clock do PWM para 2400Hz
						pwmSetRange (560);  // valor para 14,2857 Hz    // seta o registrador de intervalo no gerador de PWM para 560Hz
						do
						{
							digitalWrite(5, HIGH);                      // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
							pwmWrite (12, 5);                           // excreve o sinal pwm no pino 18,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
							Sleep(1);                                   // programa aguarda 1ms antes de incrementar a variavel segundo
							segundo++;                                  // conta q quantidade de ms incrementado
						}
						while( segundo <= 1750);                        // executa o pulso enquanto o contador não atinge o tempo 
					}
					break;
				}
				case Ef16:
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_frio = Ef1;                             // iniciar a rotina de mover o cilindro
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH); //MOVE CILINDRO FRIO  		// b na seleção do demux 
					digitalWrite(16, LOW);                         		// a na seleção do demux 
					int segundo = 0;                                    // contador de segundo iniciado em zero
					pwmSetClock(2400);  // valor para 14,2857 Hz        // seta o divisor para o clock do PWM para 2400Hz
					pwmSetRange (560);  // valor para 14,2857 Hz        // seta o registrador de intervalo no gerador de PWM para 560Hz
					do
					{
						digitalWrite(5, HIGH);                          // Determina o sentido de giro do motor. Nível alto representa sentido horário e nível baixo representa sentido anti-horário
						pwmWrite (12, 5);                               // excreve o sinal pwm no pino 12,com largura de pulso de 10ms e frequencia de 100Hz
																		// cada pulso do motor equivale a 0,25mm do fuso , e cada pico do PWM é um pulso do motor logo: 14,2857142857 Hz x 1750 ms = 25 passos = 45 graus
						Sleep(1);                                       // programa aguarda 1ms antes de incrementar a variavel segundo
						segundo++;                                      // conta q quantidade de ms incrementado
					}
					while( segundo <= 1750);                            // executa o pulso enquanto o contador não atinge o tempo 	
					break; 
				} 
			}
		}

 

//programa principal de execução 
int main(int argc, char *argv[])
{
  Control_fsm_frio = Ef1;                                             //estado inicial da variável de estado 
   while(1)
  {
	  Executa_fsm_frio();                                             //executa "para sempre" a máquina de estados , verificar quem habilita essa rotina
  }
  system("PAUSE"); 
  return 0;
}
