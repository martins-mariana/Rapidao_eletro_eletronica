#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>0

#define E0 0                                 					        //estado inicial de star/stop
#define E1 1                                                            //decisão alimentos quente
#define E2 2                                                            //rotina de servir alimentos quentes
#define E3 3                                                            //decisão alimentos frios
#define E4 4                                                            //rotina de servir alimentos frios 
#define E5 5                                                            //retirada do prato e reinicio do fuso

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

#define Ef1 1                                 					        // alimento frio  1
#define Ef2 2                                                           // servir alimento frio  1
#define Ef3 3                                                           // alimento frio 2
#define Ef4 4                                                           // servir alimento frio 2
#define Ef5 5                                                           // alimento frio 3
#define Ef6 6                                                           // servir alimento frio 3
#define Ef7 7                                                           // alimento frio 4  
#define Ef8 8                                                           // servir alimento frio 4
#define Ef9 9                                                           // alimento frio 5
#define Ef10 10                                                         // servir alimento frio 5
#define Ef11 11                                                         // alimento frio 6
#define Ef12 12                                                         // servir alimento frio 6
#define Ef13 13                                                         // alimento frio 7
#define Ef14 14                                                         // servir alimento frio 7
#define Ef15 15                                                         // alimento frio 8
#define Ef16 16                                                         // servir alimento frio 8
 
if (wiringPiSetupGpio() == -1)                                          // Inicializa o sistema wiringPi
pinMode(23, INPUT);                                                     // configura pino 23 como ENTRADA 
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
	int main(void)                                                      // função para leitura do arquivo 
	{
	  FILE *pont_arq;
	  char itens[17]={ }, quentes[9]={ }, frios[9]={ };
	  int a=0, q=0, f=0;

		pont_arq = fopen("novo_pedido.txt","r");                            //abre o arquivo para leitura
		if (pont_arq == NULL)                                               // verifica se foi aberto corretamente 
		{
			printf("Erro ao tentar abrir pedido!");
			return(0);
		}
		else
		{
			printf("Preparado itens do pedido  \n\n");                      // se abriu corretamente 
			for (a=0; a<=15; a++)                                           // percorre toda alinha
			{
				itens[a] = fgetc(pont_arq);                                 //le do caracter no arquivo apontado e salva na posição da strin
			}
		}
	  
		for(a=0; a<=7;a++)                                                  // verificando se pediu algum alimento quente
		{
			quentes[a]=itens[a];                                            //pedido quente armazenado
			frios[a]=itens[(a+8)];                                          //pedido frio armazenado
			
			if( quentes[a]=='s')                                            // verificando se pediu algum alimento quente
			{
				q++;
			}
			if( frios[a] == 's')                                            // verificando se pediu algum alimento frio
			{
				f++;
			} 
		}
	   
	  fclose(pont_arq);                                                     //fechando o arquivo
	  
	  return(0);
	}
	
	switch(ControleMaquinaEstado)                                         //executa a máquina de estados
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
		volatile char Control_fsm_quente = Eq1;                         //variável responsável por armazenar o estado atual da máquina de estados
		void Executa_fsm_quente(void);                                  //função que contém o switch/case com a máquina de estados

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
						int inst = 0;                                   // contador de ms iniciado em zero             
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
					//SELECIONA MOTOR DOS ALIMENTOS 
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, LOW);  							// b na seleção do demux 
					digitalWrite(16, LOW);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					 
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
					//SELECIONA MOTOR DOS ALIMENTOS 
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, LOW);  							// b na seleção do demux 
					digitalWrite(16, LOW);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					 
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
					//SELECIONA MOTOR DOS ALIMENTOS 
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, LOW);  							// b na seleção do demux 
					digitalWrite(16, LOW);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					 
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
					//SELECIONA MOTOR DOS ALIMENTOS 
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, LOW);  							// b na seleção do demux 
					digitalWrite(16, LOW);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					 
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
					//SELECIONA MOTOR DOS ALIMENTOS 
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, LOW);  							// b na seleção do demux 
					digitalWrite(16, LOW);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					 
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
					//SELECIONA MOTOR DOS ALIMENTOS 
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, LOW);  							// b na seleção do demux 
					digitalWrite(16, LOW);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					 
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
					//SELECIONA MOTOR DOS ALIMENTOS 
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, LOW);  							// b na seleção do demux 
					digitalWrite(16, LOW);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					 
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
					//SELECIONA MOTOR DOS ALIMENTOS 
					digitalWrite(20, HIGH);                         	// c na seleção do demux 
					digitalWrite(19, LOW);  							// b na seleção do demux 
					digitalWrite(16, LOW);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					 
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
		volatile char Control_fsm_frio= Ef1;                                 //variável responsável por armazenar o estado atual da máquina de estados
		void Executa_fsm_frio(void);                                    //função que contém o switch/case com a máquina de estados
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
					//SELECIONA MOTOR DOS ALIMENTOS FRIOS 
					digitalWrite(20, LOW);                          	// c na seleção do demux 
					digitalWrite(19, HIGH);  							// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					 
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
					//SELECIONA MOTOR DOS ALIMENTOS FRIOS 
					digitalWrite(20, LOW);                          	// c na seleção do demux 
					digitalWrite(19, HIGH);  							// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					 
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
					//SELECIONA MOTOR DOS ALIMENTOS FRIOS 
					digitalWrite(20, LOW);                          	// c na seleção do demux 
					digitalWrite(19, HIGH);  							// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					  
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
					//SELECIONA MOTOR DOS ALIMENTOS FRIOS 
					digitalWrite(20, LOW);                          	// c na seleção do demux 
					digitalWrite(19, HIGH);  							// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					 
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
					//SELECIONA MOTOR DOS ALIMENTOS FRIOS 
					digitalWrite(20, LOW);                          	// c na seleção do demux 
					digitalWrite(19, HIGH);  							// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					  
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
					//SELECIONA MOTOR DOS ALIMENTOS FRIOS 
					digitalWrite(20, LOW);                          	// c na seleção do demux 
					digitalWrite(19, HIGH);  							// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					 
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
					//SELECIONA MOTOR DOS ALIMENTOS FRIOS 
					digitalWrite(20, LOW);                          	// c na seleção do demux 
					digitalWrite(19, HIGH);  							// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					 
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
					//SELECIONA MOTOR DOS ALIMENTOS FRIOS 
					digitalWrite(20, LOW);                          	// c na seleção do demux 
					digitalWrite(19, HIGH);  							// b na seleção do demux 
					digitalWrite(16, HIGH);                        		// a na seleção do demux 					
					
					// INSERIR AQUI ROTINA DE SERVIR ALIMENTOS ( MARIANA ) 
					  
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
		//prato pronto (algum sinal de alerta)  
		
		digitalWrite(20, LOW);                                          // c na seleção do demux
		digitalWrite(19, HIGH);          //MOTOR DO FUSO                // b na seleção do demux
		digitalWrite(16, LOW);                                          // a na seleção do demux
		                                                                
		if(digitalRead(23) == LOW)                                      // saida baixa , esta detectando o prato , não retorna o fuso  
		{                                                               // Sensor E18-D80NK , possui saida normamente alta , se detectar um abejto aciona a saida ,muda para baixo 
			ControleMaquinaEstado = E5;                                 // continua no estado atual, ate retirar o prato.
        }
        else 														    //saida alta , objeto não detectado mais , o prato retirado 
        {
			ControleMaquinaEstado = E0;                                 // prato retirado, fuso volta ao estado inicial
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

