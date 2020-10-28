#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int processarEntrada(char* entrada, unsigned tamanho){       
    char ** tokens = malloc(tamanho*sizeof(char*));
    
    for (int i = 0; i < tamanho; i++){
        tokens[i] = malloc(50*sizeof(char));
    }
    
    char * token = strtok(entrada, " ");
    int t = 0;
    while( token != NULL ) {
      for (int i = 0; i < 50; i++){
          tokens[t][i] = token[i];
      }
      t++;      
      token = strtok(NULL, " ");
    }

    for (int i = 0; strlen(tokens[i]) > 0; i++){
        for (int j = 0; j < tokens[i][j] != 0; j++)
        {
            if(strcmp(&tokens[i][j],"#") == 0){
                printf("i:%d j:%d %c\n",i,j,tokens[i][j]);
            }
                
            
            
        }
        
    }


    for (int i = 0; strlen(tokens[i]) > 0; i++){
        printf(" %s\n",tokens[i]);
    }     
    
    return 0;
}

int main(){
    char string[1000] = "vetor1:\nvetor2:   .word 0x10 # (hexadecimal) Comentario apos diretiva\nvetor3:   .word 10 # (Decimal) Comentario apos diretiva\n.word 10\n.word 10  # Comentario apos diretiva\n# Comentario sozinho\n\n# vetor4: ADD 1";
    int tamanho = 200;
    
  

    processarEntrada(string,100);
}