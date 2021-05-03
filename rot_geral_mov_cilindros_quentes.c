#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>

#define Eq1 1                                 					        // alimento quente 1
#define Eq2 2                                                           // servir aliemnto quente 1
#define Eq3 3                                                           // alimento quente 2
#define Eq4 4                                                           // servir alimento quente 2
#define Eq5 5                                                           // alimento quente 3
#define Eq6 6                                                           // servir alimento quente 3
#define Eq7 7                                                           // alimento quente 4  
#define Eq8 8                                                           // servir alimento quente 4
#define Eq9 9                                                           // alimento quente 5
#define Eq10 10                                                         // servir alimento quente 5
#define Eq11 11                                                         // alimento quente 6
#define Eq12 12                                                         // servir alimento quente 6
#define Eq13 13                                                         // alimento quente 7
#define Eq14 14                                                         // servir alimento quente 7
#define Eq15 15                                                         // alimento quente 8
#define Eq16 16                                                         // servir alimento quente 8
 
if (wiringPiSetupGpio() == -1)                                          // Inicializa o sistema wiringPi para usar o pinos de GPIO
pinMode(5, OUTPUT);                                                    // configura pino 19 como SAIDA
pinMode(24, OUTPUT);                                                    // configura pino 24 como SAIDA 
pinMode(8, INPUT);                                                      // configura pino 8 como ENTRADA
pinMode(12,PWM_OUTPUT);                                                 // seta o modo do pino para INPUT, OUTPUT ou PWM_OUTPUT
pwmSetMode(PWM_MODE_MS);                                                // Configura o PWM para o modo mark:space

   
volatile char Control_fsm_quente;                                       //variável responsável por armazenar o estado atual da máquina de estados
	void Executa_fsm_quente(void);                                          //função que contém o switch/case com a máquina de estados

		void Executa_fsm_quente(void)                                      
		{
			switch(Control_fsm_quente)                                  //executa a máquina de estados
			{ 
				case Eq1:
				{
					if(quentes[0]== 's')                                //deseja alimento 0.
					{                                                         
						Control_fsm_quente = Eq2;	                    // muda para estado de servir o alimento 0
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, LOW);  // MOVE PRATO           // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
						int inst = 0;                                   // contador de ms iniciado em zero                                                                     // freq. da placa= 19.2MHz, logo colcoando pwmClock= 2400 e pwmRange= 8 , // PwmFrequency =19.2MHz/pwmClock/pwmRange
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
					else if(quentes[1]=='n');                           // não deseja alimento 0
					{
						Control_fsm_quente = Eq3;                       // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
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
				case Eq2:                                               // estado de servir o alimento 0
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_quente = Eq3;                           // depois de tetrminado a rptina de servir, iniciar a rotina de mover o cilindro
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE 	// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 
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
				case Eq3:
				{
					if(quentes[1]== 's')                                //deseja alimento 0.
					{                                                           
						Control_fsm_quente = Eq4;	                    // muda para estado de servir o alimento 0
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, LOW);   // RODAR O PRATO       // b na seleção do demux 
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
					else if(quentes[1]=='n');                           // não deseja alimento 0
					{
						Control_fsm_quente = Eq5;                       // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
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
				case Eq4:
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_quente = Eq5;                           // iniciar a rotina de mover o cilindro
					int segundo = 0;                                    // contador de segundo iniciado em zero
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE 	// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 
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
				case Eq5:
				{
					if(quentes[2]== 's')                                //deseja alimento
					{                                                           
						Control_fsm_quente = Eq6;	                    // muda para estado de servir o alimento
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, LOW);    //MOVE O PRATO        // b na seleção do demux 
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
					else if(quentes[2]=='n');                           // não deseja alimento
					{
						Control_fsm_quente = Eq7;                       // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
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
				case Eq6:
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_quente = Eq7;                           // iniciar a rotina de mover o cilindro, DEPOIS DE SERVIR 
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE 	// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 
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
				case Eq7:
				{
					if(quentes[3]== 's')                                //deseja alimento
					{                                                           
						Control_fsm_quente = Eq8;	                    // muda para estado de servir o alimento
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
					else if(quentes[3]=='n');                           // não deseja alimento
					{
						Control_fsm_quente = Eq9;                       // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
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
				case Eq8:
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_quente = Eq9;                           // iniciar a rotina de mover o cilindro
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE 	// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 
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
				case Eq9:
				{
					if(quentes[4]== 's')                                //deseja alimento
					{                                                           
						Control_fsm_quente = Eq10;                      // muda para estado de servir o alimento
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
					else if(quentes[4]=='n');                           // não deseja alimento
					{
						Control_fsm_quente = Eq11;                      // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
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
				case Eq10:
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_quente = Eq11;                          // iniciar a rotina de mover o cilindro, depois de servir 
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE 	// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 
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
				case Eq11:
				{
					if(quentes[5]== 's')                                //deseja alimento
					{                                                           
						Control_fsm_quente = Eq12;                      // muda para estado de servir o alimento
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
					else if(quentes[5]=='n');                           // não deseja alimento
					{
						Control_fsm_quente = Eq13;                      // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
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
				case Eq12:
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_quente = Eq13;                          // iniciar a rotina de mover o cilindro
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE 	// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 
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
				case Eq13:
				{
					if(quentes[6]== 's')                                //deseja alimento
					{                                                           
						Control_fsm_quente = Eq14;                      // muda para estado de servir o alimento
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, LOW); //MOVE MOTOR PRATO       // b na seleção do demux 
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
					else if(quentes[6]=='n');                           // não deseja alimento
					{
						Control_fsm_quente = Eq15;                      // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 
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
						while( segundo <= 1750);                                // executa o pulso enquanto o contador não atinge o tempo 
					}
					break;
				}
				case Eq14:
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_quente = Eq15;                          // iniciar a rotina de mover o cilindro
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE 	// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 
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
				case Eq15:
				{
					if(quentes[7]== 's')                                //deseja alimento
					{                                                           
						Control_fsm_quente = Eq16;                      // muda para estado de servir o alimento
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
					else if(quentes[7]=='n');                           // não deseja alimento
					{
						Control_fsm_quente = Eq1;                       // iniciar a rotina de mover o cilindro
						digitalWrite(20, HIGH);                         // c na seleção do demux 
						digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE // b na seleção do demux 
						digitalWrite(16, HIGH);                         // a na seleção do demux 						
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
				case Eq16:
				{
					// inserir rotina de servir alimentos aqui 
					Control_fsm_quente = Eq1;                           // iniciar a rotina de mover o cilindro
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, HIGH);  //MOVE CILINDRO QUENTE 	// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 
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
  Control_fsm_quente = Eq1;                                             //estado inicial da variável de estado 
   while(1)
  {
	  Executa_fsm_quente();                                             //executa "para sempre" a máquina de estados , verificar quem habilita essa rotina
  }
  system("PAUSE"); 
  return 0;
}
