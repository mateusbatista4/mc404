#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <token.h>

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

// int defType(char* str){
//     if(str[strlen(str) - 1] == ":")
//         return 1;
    
// }


int processarEntrada(char* entrada, unsigned tamanho){           

    char ** lines = malloc(tamanho*sizeof(char*));
    for (int i = 0; i < tamanho; i++) lines[i] = malloc(200*sizeof(char)); //dar free
              
    // char * line = strtok(entrada, "\n");
    // int t = 0;
    // while( line != NULL ) {
     
    //   for (int i = 0; i < 200; i++){ 
    //     lines[t][i] = line[i];
    //   }
    //   t++;      
    //   line = strtok(NULL, "\n");
    // }
    int cont = 1;
    int t = 0;
    for (int i = 0; i < tamanho; i++){
        char line[200] = { NULL };
        int j = 0;
        while (t < strlen(entrada) && entrada[t] != '\n'){
            line[j] = entrada[t];
            j++;
            t++;
            if(entrada[t] == '\n' || entrada[t] == '\0') cont++;
        }
        for (int k = 0; k < 200; k++){ 
            lines[i][k] = line[k];
        }
        
        t++;
    
    } 
    printf("cont %d\n",cont);

    char ** tokens = malloc(tamanho*sizeof(char*));
    for (int i = 0; i < tamanho; i++) tokens[i] = malloc(50*sizeof(char)); // dar free

    int k = 0;
     t = 0;
    while(k < cont){
        char *oldstr = malloc(sizeof(lines[k])); 
        strcpy(oldstr,lines[k]);
        
        char * token = strtok(oldstr, " ");

        if (token != NULL && strchr(token, '#')){
            token = NULL;
        }
        
        while( token != NULL ) {
            for (int i = 0; i < 50; i++){ 
                tokens[t][i] = token[i]; // criar objetos do tipo Token?
            }
                        
            t++;     
            token = strtok(NULL, " ");
            if (token!= NULL && strchr(token, '#')){
                
                token = NULL;
            }
        }
        k++;
        free(oldstr);
    }
    
    
    for (int i = 0; strlen(tokens[i]) > 0 ; i++){
        
        printf("token %d : %s\n",i,tokens[i]);
    } 


    for (int i = 0; i < cont; i++){
        
        printf("linha %d : %s\n",i,lines[i]);
       
    }     
    
    return 0;
}

int main(){
    char string[1000] = "vetor1:\nvetor2:   .word 0x10 # (hexadecimal) Comentario apos diretiva\nvetor3:   .word 10 # (Decimal) Comentario apos diretiva\n.word 10\n.word 10  # Comentario apos diretiva\n# Comentario sozinho\n\n# vetor4: ADD 1";
    int tamanho = 10;
    
    printf("%s\n\n",string);

    processarEntrada(string,strlen(string));
}