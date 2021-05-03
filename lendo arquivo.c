#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

 
int main(void)
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
