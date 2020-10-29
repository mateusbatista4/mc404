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

int hasNewLine(char* string){ // retorna o endereco de "\n" se conter no meio; -1 caso contrario
    int len = strlen(string);
    for (int i = 0; i < len; i++) 
        if(string[i] == '\n' && i != len-1) 
            return i;
    return -1;
} // caso encontrar um "\n" no fim, retorna -1 tambem

int isDirective(char* str){
    if(strcmp(str[0],".") == 0) return 1;
    return 0;
}

int isLabel(char* str){
    //caso que quebra linha entre o rotulo e a diretiva ou comando
    int lineBreaks =  hasNewLine(str);
    if(lineBreaks > 0){
        if(strcmp(str[lineBreaks-1],":") == 0) return 1;
    }
    //caso que é apenas o rótulo
    
}
int isHex(char* str){

}
int isDec(char* str){

}
int isName(char* str){

}


int processarEntrada(char* entrada, unsigned tamanho){       
    char ** tokens = malloc(tamanho*sizeof(char*));
    for (int i = 0; i < tamanho; i++) tokens[i] = malloc(50*sizeof(char));
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
        for (int j = 0; tokens[i][j] != 0; j++){
            //printf("i:%d %ld\n",i,strlen(tokens[i]));
            // if(strcmp(&tokens[i][j],"#")==0){
            //     printf("asdf");
            // }        
        }
        printf("%d\n",hasNewLine(tokens[i])); 
    }
        // if(tokens[i][j]=='\n'){       
                
        //  }  

    for (int i = 0; strlen(tokens[i]) > 0; i++){
        printf("%d %s\n",i,tokens[i]);
    }     
    
    return 0;
}

int main(){
    char string[1000] = "vetor1:\nvetor2:   .word 0x10 # (hexadecimal) Comentario apos diretiva\nvetor3:   .word 10 # (Decimal) Comentario apos diretiva\n.word 10\n.word 10  # Comentario apos diretiva\n# Comentario sozinho\n\n# vetor4: ADD 1";
    int tamanho = 200;
    
    printf("%s",string);

    processarEntrada(string,strlen(string));
}