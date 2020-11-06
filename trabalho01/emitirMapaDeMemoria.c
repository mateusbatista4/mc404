#include "montador.h"
#include "token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "processarEntrada.c"
#include "token.c"

/*
    ---- Voce deve implementar essa função para a Parte 2! ----
    Utilize os tokens da estrutura de tokens para montar seu código!
    Retorna:
        *  1 caso haja erro na montagem; (imprima o erro em stderr)
        *  0 caso não haja erro.
 */
typedef struct RotuloEndereco{
    Token* rotulo;
    int end;
    int dirEsq; // dir 1, esq 0
} RotuloEndereco;

typedef struct PosicaoAtual{
    int end ;
    int dirEsq ;
} PosicaoAtual;

char* instructions[17] = {"LD","LDINV","LDABS","LDMQ","LDMQMX","STORE","JUMP","JGE","ADD","ADDABS","SUB","SUBABS","MULT","DIV","LSH","RSH","STOREND"};
char* codigo[17] = {"01","02","03","0A","09","21",NULL,NULL,"05","07","06","08","0B","0C","14","15",NULL};
int parameters[17] = {1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,1};

char* directives[5] = {".SET",".ORG",".ALIGN",".WFILL",".WORD"};
int direParam[5] = {2,1,1,2,1};

char* toUpper(char* str){
    int size = strlen(str);
    char* new = malloc(size*sizeof(char));
    for (int i = 0; i < size; i++){
        new[i] = toupper(str[i]);
    }
    return new;
}

int getEnd(char* str){
    char* nova = toUpper(str);
    for (int i = 0; i < 17; i++){
        if(strcmp(instructions[i],nova) == 0) return i;
    }
    return -1;
}


int emitirMapaDeMemoria(){
    int achado;
    int numeroRotulos;
    
    for (int i = 0; i < getNumberOfTokens(); i++){ 
        
        if(recuperaToken(i)->tipo == DefRotulo) numeroRotulos++;
        
        if(recuperaToken(i)->tipo == Nome){
            achado = 0;
            
            char novo[strlen(recuperaToken(i)->palavra) + 1];
            strcpy(novo,recuperaToken(i)->palavra);
            novo[strlen(recuperaToken(i)->palavra)] = ':';
            
            for (int j = 0; j < i; j++){
                if(strcmp(recuperaToken(j)->palavra,novo) == 0 && recuperaToken(j)->tipo == DefRotulo){
                    achado = 1;
                    break;
                }
            }
            if(!achado) {
                fprintf( stderr, "ERRO: Rótulo ou sı́mbolo usado mas não definido: %s\n",recuperaToken(i)->palavra);
                return 1;
            }
        }
    }
    
    // AAA DD DDD DD DDD  length: 13
    int numberOfLines = recuperaToken(getNumberOfTokens()-1)->linha;
    
    
    
    // começar atribuindo endereço aos rótulos 

    char** memoryMap = malloc(400*sizeof(char*));
    for (int i = 0; i < 400; i++) memoryMap[i] = malloc(5*sizeof(char*));

    RotuloEndereco rotuloEnd[numeroRotulos];
    
    PosicaoAtual pos; pos.end = 0; pos.dirEsq = 0;

    
    
    

    
    printf("%s\n",toUpper("test"));
    
    /* printf("Voce deve implementar essa função para a Parte 2!");*/
    return 0;
}
int main(){
    char*  string = "vetor1:\nvetor2:   .word 0x10 # (hexadecimal) Comentario apos diretiva\n   .word 10 # (Decimal) Comentario apos diretiva\n\n.word 10\n.word 10  # Comentario apos diretiva\n# Comentario sozinho\n\n# vetor4: ADD 1\nADD vetor1";
    processarEntrada(string,strlen(string));
    //imprimeListaTokens();

    if(!emitirMapaDeMemoria()) {
        printf("Certo\n");
    }else{
        printf("Errado\n");
    }  

}