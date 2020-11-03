#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "token.c"
/* Exemplo

vetor1:
vetor2:   .word 0x10 # (hexadecimal) Comentario apos diretiva
vetor3:   .word 10 # (Decimal) Comentario apos diretiva
.word 10
.word 10  # Comentario apos diretiva
# Comentario sozinho

# vetor4: ADD 11

*/
int isDecimal(char* p){
    for (int i = 0; i < strlen(p); i++){
        if(!isdigit(p[i])) return 0;
    }
    return 1;
    
}

int isInstruction(char* str){
    char *oldstr = malloc(sizeof(str)); 
    strcpy(oldstr,str);
    for (int i = 0; i < strlen(str); i++){
        oldstr[i] = toupper(oldstr[i]);
        
    }

    char* instructions[17] = {"LD", "LDINV", "LDABS", "LDMQ", "LDMQMX", "STORE", "JUMP",
    "JGE" ,"ADD", "ADDABS", "SUB", "SUBABS", "MULT", "DIV", "LSH", "RSH", "STOREND"};
    for (int i = 0; i < 17; i++){
        if(strcmp(oldstr,instructions[i]) == 0) {
           
            return 1;
        }
    }
    free(oldstr);
    return 0;
}

int defType(char* str){
    //printf("token: %s\n",str);
    if(str[strlen(str) - 1] == ':') {// Rotulo
        return 1;
    }
    
    if(str[0] == '.'){ // directive
        return 4;
    }
    if(isInstruction(str)){ // instruction
        return 5;
    }
    if(strlen(str) >= 2 && isdigit(str[0]) && str[1] == 'x'){ // Hexadecimal
        return 2;
    }
    if(isDecimal(str)){ // integger
        return 3;
    }
    return 6; // name
}


int processarEntrada(char* entrada, unsigned tamanho){           

    char ** lines = malloc(tamanho*sizeof(char*));
    for (int i = 0; i < tamanho; i++) lines[i] = malloc(200*sizeof(char)); //dar free
              
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
            
            if(strlen(token) > 0){
                switch (defType(token)){
                    case 1:
                        adicionarToken(DefRotulo,token,k);
                        printf("linha: %d , Rotulo, tk: %s\n",k,token);
                        break;
                    case 2:
                        adicionarToken(Hexadecimal,token,k);
                        printf("linha: %d , Hex, tk: %s\n",k,token);
                        break;
                    case 3:
                        adicionarToken(Decimal,token,k);
                        printf("linha: %d , Dec, tk: %s\n",k,token);
                        break;
                    case 4:
                        adicionarToken(Diretiva,token,k);
                        printf("linha: %d , Dire, tk: %s\n",k,token);
                        break;
                    case 5:
                        adicionarToken(Instrucao,token,k);
                        printf("linha: %d , Instru, tk: %s\n",k,token);
                        break;
                    case 6:
                        adicionarToken(Nome,token,k);
                        printf("linha: %d , Nome, tk: %s\n",k,token);
                        break;
                    return -1;
                    
                }
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
    
    
    // for (int i = 0; strlen(tokens[i]) > 0 ; i++){   
    //     printf("token %d : %s\n",i,tokens[i]);
    // } 
    // for (int i = 0; i < cont; i++){ 
    //     printf("linha %d : %s\n",i,lines[i]);
    // }     
    return 0;
}

int main(){
    char string[1000] = "\nADD 0x10\nSUB 20\n\nadd k           #Erro na Parte 2! Usado, mas não definido!\nSUB 15";
    int tamanho = 10;


    processarEntrada(string,strlen(string));
}