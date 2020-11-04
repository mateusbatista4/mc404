#include "montador.h"
#include "token.h"
#include <stdio.h>
#include "processarEntrada.c"
#include "token.c"

/*
    ---- Voce deve implementar essa função para a Parte 2! ----
    Utilize os tokens da estrutura de tokens para montar seu código!
    Retorna:
        *  1 caso haja erro na montagem; (imprima o erro em stderr)
        *  0 caso não haja erro.
 */
int emitirMapaDeMemoria(){
    int achado;
    
    for (int i = 0; i < getNumberOfTokens(); i++){ 
        printf("%s\n",recuperaToken(i)->palavra);
        if(recuperaToken(i)->tipo == Nome){
            int achado = 0;
            for (int j = 0; j < getNumberOfTokens(); j++){
                if(strcmp(recuperaToken(j)->palavra,recuperaToken(i)->palavra) == 0 && recuperaToken(j)->tipo == DefRotulo){
                    achado = 1;
                    break;
                }
            }
            if(!achado) return 0;
        }
    }
    


    /* printf("Voce deve implementar essa função para a Parte 2!");*/
    return 1;
}
int main(){
    char*  string = "vetor1:\nvetor2:   .word 0x10 # (hexadecimal) Comentario apos diretiva\nvetor3:   .word 10 # (Decimal) Comentario apos diretiva\n\n.word 10\n.word 10  # Comentario apos diretiva\n# Comentario sozinho\n\n# vetor4: ADD 1\nADD vetor1";
    processarEntrada(string,strlen(string));
    if(emitirMapaDeMemoria()) {
        printf("certp\n");
    }else{
        printf("errado\n");
    }  

}