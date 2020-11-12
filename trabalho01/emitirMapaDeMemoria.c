#include "montador.h"
#include "token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>

typedef struct RotuloEndereco{ // Struct utilizado para fazer a associação rótulo - endereço
    char rotulo[64];
    int end;
    int dirEsq; // dir 1, esq 0
} RotuloEndereco;



typedef struct PosicaoAtual{ // uso esse struct para armazenar as informações de onde estou no Mapa de memoria
    int end ;
    int dirEsq ;
} PosicaoAtual;

int retornaValor(char* rot, RotuloEndereco* list, int t){ // retorna o endereço a que um rótulo "rot" se refere
    for (int i = 0; i < t; i++){
        
        if(strcmp(rot,list[i].rotulo) == 0) 
            return list[i].end;
    }
    printf("n cahou bad\n");
    return 0;
}
int retornaDirEsq(char* rot, RotuloEndereco* list, int t){ // retorna 0 se o rotulo "rot" estiver na esquerda, e 1 caso for direita

    for (int i = 0; i < t; i++){
        
        if(strcmp(rot,list[i].rotulo) == 0) 
            return list[i].dirEsq;
    }
    printf("n cahou bad\n");
    return 0;
}

int hexadecimalToDecimal(char hexVal[])  // Hexadecimal String para decimal
{    
    int len = strlen(hexVal); 
      
    // Initializing base value to 1, i.e 16^0 
    int base = 1; 
      
    int dec_val = 0; 
      
    // Extracting characters as digits from last character 
    for (int i=len-1; i>=0; i--) 
    {    
        // if character lies in '0'-'9', converting  
        // it to integral 0-9 by subtracting 48 from 
        // ASCII value. 
        if (hexVal[i]>='0' && hexVal[i]<='9') 
        { 
            dec_val += (hexVal[i] - 48)*base; 
                  
            // incrementing base by power 
            base = base * 16; 
        } 
  
        // if character lies in 'A'-'F' , converting  
        // it to integral 10 - 15 by subtracting 55  
        // from ASCII value 
        else if (hexVal[i]>='A' && hexVal[i]<='F') 
        { 
            dec_val += (hexVal[i] - 55)*base; 
          
            // incrementing base by power 
            base = base*16; 
        } 
    } 
      
    return dec_val; 
} 
void avancar(PosicaoAtual* pos, int word){ // avançar no mapa de memoria, considerando direita e esquerda
    if(word) {
        pos->end ++;
    } else {
        if(pos->dirEsq){
            pos->end++;  
        }   
        pos->dirEsq = !pos->dirEsq;
    }
    
    
}

/// Dicionário de valores

char* instructions[17] = {"LD","LDINV","LDABS","LDMQ","LDMQMX","STORE","JUMP","JGE","ADD","ADDABS","SUB","SUBABS","MULT","DIV","LSH","RSH","STOREND"};
int codigo[17] = {1,2,3,10,9,33,0,0,5,7,6,8,11,12,20,21,0};
int parameters[17] = {1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,1};

char* directives[5] = {".SET",".ORG",".ALIGN",".WFILL",".WORD"};
int direParam[5] = {2,1,1,2,1};

//String to UPPPEr
char* toUpper(char* str){
    int size = strlen(str);
    char* new = malloc(size*sizeof(char));
    for (int i = 0; i < size; i++){
        new[i] = toupper(str[i]);
    }
    return new;
}


int emitirMapaDeMemoria(){
    int achado;
    int numeroRotulos = 0;
    int numeroPontoOrg = 0;
    
    for (int i = 0; i < getNumberOfTokens(); i++){  // Este "for" é utilizado para achar nomes não declarados
        
        if(recuperaToken(i)->tipo == DefRotulo) numeroRotulos++;
        if(recuperaToken(i)->tipo == Diretiva && strcmp(toUpper(recuperaToken(i)->palavra),".ORG") == 0 ) numeroPontoOrg++;

        if(recuperaToken(i)->tipo == Nome){
            achado = 0;
            
            char novo[strlen(recuperaToken(i)->palavra) + 2];
            strcpy(novo,recuperaToken(i)->palavra);
            novo[strlen(recuperaToken(i)->palavra)] = ':';
            novo[strlen(recuperaToken(i)->palavra)+1] = '\0';
            for (int j = 0; j < i; j++){
                if(strcmp(recuperaToken(j)->palavra,novo) == 0 && recuperaToken(j)->tipo == DefRotulo){
                    achado = 1;
                    break;
                }
            }
            novo[strlen(recuperaToken(i)->palavra)] = '\0';
            for (int j = 0; j < i; j++){
                if(!strcmp(toUpper(recuperaToken(j)->palavra),".SET") && !strcmp(recuperaToken(j+1)->palavra,recuperaToken(i)->palavra)){
                    achado = 1;
                    break;                    
                }
            }
            

            if(!achado) {
                fprintf(stderr, "%s %s\n","ERRO: Rótulo ou símbolo usado mas não definido:",recuperaToken(i)->palavra);
                return 1;
            }
        }
    }
    
   
    
    
    // Mapa de memória
    int* enderecos = malloc(1024*sizeof(int));
    int* instrucaoEsquerda = malloc(1024*sizeof(int));
    int* paramEsquerda = malloc(1024*sizeof(int));
    int* instrucaoDireita = malloc(1024*sizeof(int));
    int* paramDireita = malloc(1024*sizeof(int));
    int* used = malloc(1034*sizeof(int)); // pra saber se usei o endereço

    for (int i = 0; i < 1024; i++){
        enderecos[i] = i;
    }
    

    RotuloEndereco rotuloEnd[numeroRotulos]; // lista de informações sobre os rótulos
    
    
    int* ends = malloc(1024*sizeof(int)); // uso isso para printar ceto quando tenho .org mudando eo endereço 
    for (int i = 0; i < 1024; i++) ends[i] = -1;
    


    int tk= 0; // Index list of Tokens
    int atual = 0; // 
    PosicaoAtual pos; pos.end = 0; pos.dirEsq = 0;

    for (int i = 0; i < getNumberOfTokens(); i++){
        Token* token = recuperaToken(i);
       
        if(token->tipo == DefRotulo){ // Tokens -> add na lista
            RotuloEndereco rot ;
            rot.dirEsq = pos.dirEsq;
            rot.end = pos.end;

            char novo[strlen(recuperaToken(i)->palavra)];
            strcpy(novo,recuperaToken(i)->palavra);
            novo[strlen(recuperaToken(i)->palavra)-1] = '\0';
            
            strcpy(rot.rotulo,novo);
            rotuloEnd[tk] = rot;
            tk++;
            
        }
        if(token->tipo == Diretiva){
            if(strcmp(toUpper(token->palavra),directives[0]) == 0 ){ // ".SET" -> ja mudo todos os nomes iguais dali pra frente
                for (int k = i+2 ; k < getNumberOfTokens(); k++){
                    if(strcmp(recuperaToken(k)->palavra,recuperaToken(i+1)->palavra) == 0 && recuperaToken(k)->tipo == Nome){
                        recuperaToken(k)->tipo = recuperaToken(i+2)->tipo;
                        recuperaToken(k)->palavra = recuperaToken(i+2)->palavra;
                    }
                }
                
            }
            else if (strcmp(toUpper(token->palavra),directives[1]) == 0 ) {// ".ORG"
                
                pos.end = hexadecimalToDecimal(recuperaToken(i+1)->palavra);
                pos.dirEsq = 0;
                         
            }
            else if (strcmp(toUpper(token->palavra),directives[2]) == 0 ) {// ".ALIGN"
                int next =  atoi(recuperaToken(i+1)->palavra);
                int cont = 0;
                for (int k = pos.end + 1; k < 1024; k++){
                    cont++;
                    if( k % next == 0) {
                        pos.end = k;  
                        pos.dirEsq = 0;                      
                        break;
                    }                    
                }

                
                
                
            }
            else if(strcmp(toUpper(token->palavra),directives[3]) == 0 ){ // ".WFILL"
                int size =  atoi(recuperaToken(i+1)->palavra);
                int value ;
                if(recuperaToken(i+2)->tipo == Hexadecimal) value =  hexadecimalToDecimal(recuperaToken(i+2)->palavra);
               else if(recuperaToken(i+2)->tipo == Decimal) value = atoi(recuperaToken(i+2)->palavra);
                int k;
                for (k = pos.end; k < pos.end + size; k++){
                    paramDireita[k] = value;
                    ends[atual] = k;
                    atual++;
                    used[k] = 1;
                }
                pos.end = k;
            
                pos.dirEsq = 0;
            }
            else if(strcmp(toUpper(token->palavra),directives[4]) == 0 ){ // ".WORD"
               int value;
               if(recuperaToken(i+1)->tipo == Hexadecimal) value =  hexadecimalToDecimal(recuperaToken(i+1)->palavra);
               else if(recuperaToken(i+1)->tipo == Decimal) value = atoi(recuperaToken(i+1)->palavra);
               paramDireita[pos.end] = value;
               used[pos.end] = 1;
               ends[atual] = pos.end;
               atual++;
              
               avancar(&pos,1);
               
            }
        }
        if(token->tipo == Instrucao){ // Instruçoes
            int end; // index daa instrução no dicionário declarado anteriomente
            ends[atual] = pos.end;
            atual++;
            char p[60];
            strcpy(p,token->palavra);
            for (int k = 0; k < 17; k++){
                if(strcmp(toUpper(token->palavra),instructions[k]) == 0 ){ 
                    end = k;
                    break;
                }
            }
            used[pos.end] = 1; 

            if(codigo[end] != 0){
                if(!pos.dirEsq){
                    instrucaoEsquerda[pos.end] = codigo[end];
                    
                    if(parameters[end]){
                       if(recuperaToken(i+1)->tipo == Decimal ) 
                            paramEsquerda[pos.end] = atoi(recuperaToken(i+1)->palavra); 
                        else if(recuperaToken(i+1)->tipo == Nome){
                            paramEsquerda[pos.end] = retornaValor(recuperaToken(i+1)->palavra,rotuloEnd,numeroRotulos);
                        }
                        else if (recuperaToken(i+1)->tipo == Hexadecimal) 
                            paramEsquerda[pos.end] = hexadecimalToDecimal(recuperaToken(i+1)->palavra);                         
                    }
                } else {
                    instrucaoDireita[pos.end] = codigo[end];
                    
                    if(parameters[end]){
                        if(recuperaToken(i+1)->tipo == Decimal ) 
                            paramDireita[pos.end] = atoi(recuperaToken(i+1)->palavra); 
                        else if(recuperaToken(i+1)->tipo == Nome){
                            paramDireita[pos.end] = retornaValor(recuperaToken(i+1)->palavra,rotuloEnd,numeroRotulos);
                        }
                        else if (recuperaToken(i+1)->tipo == Hexadecimal) 
                            paramDireita[pos.end] = hexadecimalToDecimal(recuperaToken(i+1)->palavra);                         
                    }                    
                }   

            } else {//  caso em que deovo verificar se é direita ou esquerda, mudando o código da instrução
                if(recuperaToken(i+1)->tipo == Hexadecimal ){ // esquerda
                    if(strcmp(toUpper(token->palavra),instructions[6]) == 0 ) { // JUMP - 13
                        if(!pos.dirEsq){
                            instrucaoEsquerda[pos.end] = 13;
                            paramEsquerda[pos.end] = hexadecimalToDecimal(recuperaToken(i+1)->palavra);
                        } else{
                            instrucaoDireita[pos.end] = 13;
                            paramDireita[pos.end] = hexadecimalToDecimal(recuperaToken(i+1)->palavra);
                        }
                        
                    }
                    else if(strcmp(toUpper(token->palavra),instructions[7]) == 0 ){ // JUMP+ 15
                        if(!pos.dirEsq){
                            instrucaoEsquerda[pos.end] = 15;
                            paramEsquerda[pos.end] = hexadecimalToDecimal(recuperaToken(i+1)->palavra);
                        } else{
                            instrucaoDireita[pos.end] = 15;
                            paramDireita[pos.end] = hexadecimalToDecimal(recuperaToken(i+1)->palavra);
                        }
                        
                        
                    }else if(strcmp(toUpper(token->palavra),instructions[16]) == 0 ){ // STOREND 18
                        if(!pos.dirEsq){
                            instrucaoEsquerda[pos.end] = 18;
                            paramEsquerda[pos.end] = hexadecimalToDecimal(recuperaToken(i+1)->palavra);
                        } else{
                            instrucaoDireita[pos.end] = 18;
                            paramDireita[pos.end] = hexadecimalToDecimal(recuperaToken(i+1)->palavra);
                        }
                        
                    }
                } 
                
                else if (recuperaToken(i+1)->tipo == Decimal ){ // esquerda
                    if(strcmp(toUpper(token->palavra),instructions[6]) == 0 ) { // JUMP - 13
                        if(!pos.dirEsq){
                            instrucaoEsquerda[pos.end] = 13;
                            paramEsquerda[pos.end] =  atoi(recuperaToken(i+1)->palavra);
                        } else{
                            instrucaoDireita[pos.end] = 13;
                            paramDireita[pos.end] =  atoi(recuperaToken(i+1)->palavra);
                        }
                        
                    }
                    else if(strcmp(toUpper(token->palavra),instructions[7]) == 0 ){ // JUMP+ 15
                        if(!pos.dirEsq){
                            instrucaoEsquerda[pos.end] = 15;
                            paramEsquerda[pos.end] =  atoi(recuperaToken(i+1)->palavra);
                        } else{
                            instrucaoDireita[pos.end] = 15;
                            paramDireita[pos.end] =  atoi(recuperaToken(i+1)->palavra);
                        }
                        
                    }else if(strcmp(toUpper(token->palavra),instructions[16]) == 0 ){ // STOREND 18
                        if(!pos.dirEsq){
                            instrucaoEsquerda[pos.end] = 18;
                            paramEsquerda[pos.end] =  atoi(recuperaToken(i+1)->palavra);
                        } else{
                            instrucaoDireita[pos.end] = 18;
                            paramDireita[pos.end] =  atoi(recuperaToken(i+1)->palavra);
                        }
                        
                    }
                } 
                
                else if(recuperaToken(i+1)->tipo == Nome){// Direita ou esquerda
                    
                    int dirEs = retornaDirEsq(recuperaToken(i+1)->palavra,rotuloEnd,numeroRotulos); // descubro se é esquerda ou direira
                    
                    if(!dirEs){ // esquerda
                       if(strcmp(toUpper(token->palavra),instructions[6]) == 0 ) { // JUMP - 13
                            if(!pos.dirEsq){
                                instrucaoEsquerda[pos.end] = 13;
                                paramEsquerda[pos.end] = retornaValor(recuperaToken(i+1)->palavra,rotuloEnd,numeroRotulos);
                                
                            } else{
                                instrucaoDireita[pos.end] = 13;
                                paramDireita[pos.end] = retornaValor(recuperaToken(i+1)->palavra,rotuloEnd,numeroRotulos);
                            }  
                            
                        }
                        else if(strcmp(toUpper(token->palavra),instructions[7]) == 0 ){ // JUMP+ 15
                            if(!pos.dirEsq){
                                instrucaoEsquerda[pos.end] = 15;
                                paramEsquerda[pos.end] = retornaValor(recuperaToken(i+1)->palavra,rotuloEnd,numeroRotulos);
                            } else{
                                instrucaoDireita[pos.end] = 15;
                                 paramDireita[pos.end] = retornaValor(recuperaToken(i+1)->palavra,rotuloEnd,numeroRotulos);
                            }
                            
                        }else if(strcmp(toUpper(token->palavra),instructions[16]) == 0 ){ // STOREND 18
                            if(!pos.dirEsq){
                                instrucaoEsquerda[pos.end] = 18;
                                paramEsquerda[pos.end] = retornaValor(recuperaToken(i+1)->palavra,rotuloEnd,numeroRotulos);
                            } else{
                                instrucaoDireita[pos.end] = 18;
                                 paramDireita[pos.end] = retornaValor(recuperaToken(i+1)->palavra,rotuloEnd,numeroRotulos);
                            }
                            
                        } 
                    
                    } else { // direita
                        if(strcmp(toUpper(token->palavra),instructions[6]) == 0 ) { // JUMP - 14
                            if(!pos.dirEsq){
                                instrucaoEsquerda[pos.end] = 14;
                                paramEsquerda[pos.end] = retornaValor(recuperaToken(i+1)->palavra,rotuloEnd,numeroRotulos);
                            } else{
                                instrucaoDireita[pos.end] = 14;
                                 paramDireita[pos.end] = retornaValor(recuperaToken(i+1)->palavra,rotuloEnd,numeroRotulos);
                            }
                            
                        }
                        else if(strcmp(toUpper(token->palavra),instructions[7]) == 0 ){ // JUMP+ 16
                            if(!pos.dirEsq){
                                instrucaoEsquerda[pos.end] = 16;
                                paramEsquerda[pos.end] = retornaValor(recuperaToken(i+1)->palavra,rotuloEnd,numeroRotulos);
                            } else{
                                instrucaoDireita[pos.end] = 16;
                                 paramDireita[pos.end] = retornaValor(recuperaToken(i+1)->palavra,rotuloEnd,numeroRotulos);
                            }
                            
                        }else if(strcmp(toUpper(token->palavra),instructions[16]) == 0 ){ // STOREND 19
                            if(!pos.dirEsq){
                                instrucaoEsquerda[pos.end] = 19;
                                paramEsquerda[pos.end] = retornaValor(recuperaToken(i+1)->palavra,rotuloEnd,numeroRotulos);
                            } else{
                                instrucaoDireita[pos.end] = 19;
                                 paramDireita[pos.end] = retornaValor(recuperaToken(i+1)->palavra,rotuloEnd,numeroRotulos);
                            }
                            
                        }
                    }
                    
                }   
            }      

            avancar(&pos,0); // avanço depois de cada Instrução
            
        }
   
              
    }
    
    for (int i = 0; i < 1024; i++){  // paranauê pra printar certo, por causa do .org
        if(ends[i]>=0){
            for (int j = i + 1; j < 1024; j++){
                if(ends[i] == ends[j]){
                    ends[j] = -1;
                    
                }
            }
            
        }
    }
   
    // imprime o mapa de memória
    for (int j = 0; j < 1024; j++){
        int i = ends[j];
        if(i < 0) continue;
        if(used[i] )
        printf("%03X %02X %03X %02X %03X\n",enderecos[i],instrucaoEsquerda[i],paramEsquerda[i],instrucaoDireita[i],paramDireita[i]);
    }

    return 0;
}


