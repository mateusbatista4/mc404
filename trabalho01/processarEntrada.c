#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>


// Função  para verivicar se um digito é decimal
int isDecimal(char* p){
    for (int i = 0; i < strlen(p); i++){
        if(!isdigit(p[i])) return 0;
    }
    return 1;
    
}
//Função para verificar se uma String 'str' é uma instrução: retirn 1 se sim, zero caso contrário
int isInstruction(char* str){ 
    char *oldstr = malloc(sizeof(str));  // nova string, alocada para se tornar maiuscula
    strcpy(oldstr,str);

    for (int i = 0; i < strlen(str); i++){ // comparar sempre com a string em Mauisculo
        oldstr[i] = toupper(oldstr[i]);
        
    }

    char* instructions[17] = {"LD", "LDINV", "LDABS", "LDMQ", "LDMQMX", "STORE", "JUMP",
    "JGE" ,"ADD", "ADDABS", "SUB", "SUBABS", "MULT", "DIV", "LSH", "RSH", "STOREND"};
    for (int i = 0; i < 17; i++){
        if(strcmp(oldstr,instructions[i]) == 0) { // comparação com as possiveis instruções
           
            return 1;
        }
    }
    return 0;
}
//Função para classificar os tokens: retorna um inteiro diferente para cada TokenType
int defType(char* str){   
    if(str[strlen(str) - 1] == ':') {// Rotulo
        return 1;
    }
    
    if(str[0] == '.'){ // directive
        return 4;
    }
    if(isInstruction(str)){ // instruction
        return 5;
    }
    if(strlen(str) >= 2 && isdigit(str[0]) ){ // Hexadecimal
        if(str[1] == 'x' || str[1] == 'X')return 2;
    }
    if(isDecimal(str)){ // integger
        return 3;
    }
    return 6; // name
}


// Função principal : separa as linhas e  tokens e os classifica 
int processarEntrada(char* entrada, unsigned tamanho){     

    for (int i = 0; i < strlen(entrada); i++){ // Inicialmente, trocamos os '\t' por ' ' (espaços)
        if (entrada[i] == '\t'){
            entrada[i] = ' ';
        }   
    }
          

    char ** lines = malloc(tamanho*sizeof(char*));
    for (int i = 0; i < tamanho; i++) lines[i] = malloc(3*65*sizeof(char)); //aloco memoria para uma matriz que representa as linhas 
              
    int cont = 0; // contador do numero de linhas da entrada
    int t = 0; // contador que assume a posição de cada elemento da entrada 'entrada'
    
    for (int i = 0; i < tamanho; i++){ 
       
        int j = 0;
        char* line;
        line = malloc(200*sizeof(char));    // linha vazia alocada
        while (t < strlen(entrada) && entrada[t] != '\n'){ 
            // enquanto o caracterre estiver valido e 
            //pertencer àquela linha, ele é adicionado á linha alocada
            line[j] = entrada[t];
            j++;
            t++;
            if(entrada[t] == '\n' || entrada[t] == '\0') cont++;
        }
        
        for (int k = 0; k < 3*65; k++){  // Compio a linha alocada e já preenchida anteriormente para a matriz de linhas
            lines[i][k] = line[k];
        }
        
        if(j<1 && entrada[t] == '\n' ) cont++; // incrementar o contador se a linha for nula, pois tambem conta
        t++;
    } 


    // aloco memória para uma matriz de tokens, que será cada token de cada linha
    char ** tokens = malloc(tamanho*sizeof(char*));
    for (int i = 0; i < tamanho; i++) tokens[i] = malloc(65*sizeof(char));  

    int k = 0; // contador para o numero de linhas da entrada já definido (cont)
    t = 0; // contador para o numero de tokens/ linhas da matriz
    while(k < cont){
        char oldstr[strlen(lines[k])];  
        // copia da linha k, da matriz de linhas, que sera analizada e particionada 
        // usando o strtok()
        strcpy(oldstr,lines[k]);
        
        char * token = strtok(oldstr, " ");
    
        if (token != NULL && strchr(token, '#')){ // se achar um comentario, desconsidera a linha
            token = NULL;
        }
        
        while( token != NULL ) {
            for (int i = 0; i < 65; i++){ 
                tokens[t][i] = token[i]; // cópia do valor salvo em 'token' para a matriz de tokens
            }            
            if(strlen(token) > 0){
            
                switch (defType(token)){ // Classificação com o auxilio das funçoes definidas acima
                    case 1:
                        adicionarToken(DefRotulo,tokens[t],k+1);
                        break;
                    case 2:
                        adicionarToken(Hexadecimal,tokens[t],k+1);
                        break;
                    case 3:
                        adicionarToken(Decimal,tokens[t],k+1);
                        break;
                    case 4:
                        adicionarToken(Diretiva,tokens[t],k+1);
                        break;
                    case 5:
                        adicionarToken(Instrucao,tokens[t],k+1);
                        break;
                    case 6:
                        adicionarToken(Nome,tokens[t],k+1);
                        break;
                    return -1; // caso algum erro na classificação, retorna 1
                    
                }
            }
                       
            t++;     
            token = strtok(NULL, " ");
            if (token!= NULL && strchr(token, '#')){ // caso achar um '#' desconsidera tudo dali pra frente, pois é um comentario
                token = NULL;
            }
        }
        k++;
        
    }
    // retorna ZERO caso chague ao fim : sucesso
    return 0;
}

