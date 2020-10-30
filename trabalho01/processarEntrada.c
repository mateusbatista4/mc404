#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Exemplo

vetor1:
vetor2:   .word 0x10 # (hexadecimal) Comentario apos diretiva
vetor3:   .word 10 # (Decimal) Comentario apos diretiva
.word 10
.word 10  # Comentario apos diretiva
# Comentario sozinho

# vetor4: ADD 11

*/

// caso encontrar um "\n" no fim, retorna -1 tambem



int processarEntrada(char* entrada, unsigned tamanho){       
    char ** lines = malloc(tamanho*sizeof(char*));
    for (int i = 0; i < tamanho; i++) lines[i] = malloc(200*sizeof(char));
              
    char * line = strtok(entrada, "\n");
    int t = 0;
    while( line != NULL ) {
      for (int i = 0; i < 200; i++){ 
        lines[t][i] = line[i];
      }
      t++;      
      line = strtok(NULL, "\n");
    }
    // separação de linhas pronta
    
    
    char ** tokens = malloc(tamanho*sizeof(char*));
    for (int i = 0; i < tamanho; i++) tokens[i] = malloc(50*sizeof(char));

        

    int k = 0;
    t = 0;
    while(strlen(lines[k]) > 0){
        
        char *oldstr = malloc(sizeof(lines[k]));
        strcpy(oldstr,lines[k]);
        
        char * token = strtok(oldstr, " ");
        //printf("%s\n",token);
        
        while( token != NULL ) {
            for (int i = 0; i < 50; i++){ 
                tokens[t][i] = token[i];
            }
            t++;     
            token = strtok(NULL, " ");
            //if(token) printf("%s\n",token);
        }
        k++;
    }
    
         

    for (int i = 0; strlen(lines[i]) > 0; i++){
        printf("linha %d : %s\n",i,lines[i]);
    } 
    printf("\n");
    for (int i = 0; strlen(tokens[i]) > 0 ; i++){
        printf("token %d : %s\n",i,tokens[i]);
    } 
        
    
    return 0;
}

int main(){
    char string[1000] = "vetor1:\nvetor2:   .word 0x10 # (hexadecimal) Comentario apos diretiva\nvetor3:   .word 10 # (Decimal) Comentario apos diretiva\n.word 10\n.word 10  # Comentario apos diretiva\n# Comentario sozinho\n\n# vetor4: ADD 1";
    int tamanho = 200;
    
    printf("%s\n\n",string);

    processarEntrada(string,strlen(string));
}