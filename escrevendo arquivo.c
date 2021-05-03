#include <stdio.h>
#include <stdlib.h>
int main(void)
{
  FILE *pont_arq;                                                       // cria variável ponteiro para o arquivo
  char pedido[17], inicio; 
  int i=0;                                                              // variável do tipo string
  
  
  do
  {
	printf("Deseja fazer seu pedido ? (s/n)\n");
    scanf("%s", &inicio);
  }
  while (inicio != 's'); 
  
  
  pont_arq = fopen("novo_pedido.txt", "w");                             //abrindo o arquivo com tipo de abertura w
  
  if(pont_arq == NULL)                                                  //testando se o arquivo foi realmente criado
  {
  printf("Erro na abertura do pedido, tente novamente mais tarde!");
  }
  for(i=0; i<=15; i=i+1)                                                //Percorrendo o vetor de char e escrevendo a resposta  na sring
  {
	  printf("deseja o alimento #%d ? (s/n)\n", i);
	  scanf("%s", &pedido[i]);
  }
  
  printf("%s\n" , pedido);  
  
  fprintf(pont_arq, "%s", pedido);                                      //usando fprintf para armazenar a string no arquivo
  fclose(pont_arq);                                                     //usando fclose para fechar o arquivo
  
  printf("Pedido realizado com sucesso!");

  return(0);
}
