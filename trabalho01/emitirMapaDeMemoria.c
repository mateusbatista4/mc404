#include "montador.h"
#include "token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "processarEntrada.c"
#include "token.c"
#include <stdint.h>

/*
    ---- Voce deve implementar essa função para a Parte 2! ----
    Utilize os tokens da estrutura de tokens para montar seu código!
    Retorna:
        *  1 caso haja erro na montagem; (imprima o erro em stderr)
        *  0 caso não haja erro.
 */
typedef struct RotuloEndereco{
    char* rotulo;
    int end;
    int dirEsq; // dir 1, esq 0
} RotuloEndereco;

typedef struct Sets{
    char* palavra;
    char* value;
} Sets;


typedef struct PosicaoAtual{
    int end ;
    int dirEsq ;
} PosicaoAtual;
int hexadecimalToDecimal(char hexVal[]) 
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
void avancar(PosicaoAtual* pos){
    if(pos->dirEsq){
        pos->end++;  
    }   
    pos->dirEsq = !pos->dirEsq;
}



char* instructions[17] = {"LD","LDINV","LDABS","LDMQ","LDMQMX","STORE","JUMP","JGE","ADD","ADDABS","SUB","SUBABS","MULT","DIV","LSH","RSH","STOREND"};
int codigo[17] = {1,2,3,10,9,33,0,0,5,7,6,8,11,12,20,21,0};
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
    int numeroRotulos = 0;
    int numeroSets = 0;
    
    for (int i = 0; i < getNumberOfTokens(); i++){ 
        
        if(recuperaToken(i)->tipo == DefRotulo) numeroRotulos++;
        if(recuperaToken(i)->tipo == Diretiva && strcmp(toUpper(recuperaToken(i)->palavra),".SET") == 0 ) numeroSets++;

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
                fprintf( stderr, "ERRO: Rótulo ou sı́mbolo usado mas não definido: %s\n",recuperaToken(i)->palavra);
                return 1;
            }
        }
    }
    
    // AAA DD DDD DD DDD  length: 13
    int numberOfLines = recuperaToken(getNumberOfTokens()-1)->linha;
    
    
    
    // começar atribuindo endereço aos rótulos 
    int* enderecos = malloc(1024*sizeof(int));
    int* instrucaoEsquerda = malloc(1024*sizeof(int));
    int* paramEsquerda = malloc(1024*sizeof(int));
    int* instrucaoDireita = malloc(1024*sizeof(int));
    int* paramDireita = malloc(1024*sizeof(int));
    int* used = malloc(1034*sizeof(int));

    for (int i = 0; i < 1024; i++){
        enderecos[i] = i;
    }
    

    RotuloEndereco rotuloEnd[numeroRotulos];
    Sets setValues[numeroSets];

    //printf("no rot: %d  no .set: %d\n",numeroRotulos,numeroSets);

    int tk= 0; // Index list of Tokens
    int set = 0; // index List of Sets
    PosicaoAtual pos; pos.end = 0; pos.dirEsq = 0;

    for (int i = 0; i < getNumberOfTokens(); i++){
        Token* token = recuperaToken(i);

        if(token->tipo == DefRotulo){
            RotuloEndereco rot ;
            rot.dirEsq = pos.dirEsq;
            rot.end = pos.end;
            rot.rotulo = token->palavra;
            rotuloEnd[tk] = rot;
            tk++;
            char novo[strlen(recuperaToken(i)->palavra)];
            strcpy(novo,recuperaToken(i)->palavra);
            novo[strlen(recuperaToken(i)->palavra)-1] = '\0';
            for (int k = i+1 ; k < getNumberOfTokens(); k++){
                
                if(strcmp(recuperaToken(k)->palavra,novo) == 0 && recuperaToken(k)->tipo == Nome){
                    sprintf(recuperaToken(k)->palavra, "%d", pos.end);
                   
                }
            }
            
        }
        if(token->tipo == Diretiva){
            if(strcmp(toUpper(token->palavra),directives[0]) == 0 ){ // ".SET"
                // Sets s ;
                // s.palavra = recuperaToken(i+1)->palavra;
                // s.value =  recuperaToken(i+2)->palavra;
                // setValues[set] = s;
                // set++;
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
                // printf(".org at %d\n",pos.end);            
            }
            else if (strcmp(toUpper(token->palavra),directives[2]) == 0 ) {// ".ALIGN"
                int next =  atoi(recuperaToken(i+1)->palavra);
                for (int k = pos.end + 1; k < 1024; k++){
                    if( k % next == 0) {
                        pos.end = k;  
                        pos.dirEsq = 0;                      
                        break;
                    }                    
                }
                
            }
            else if(strcmp(toUpper(token->palavra),directives[3]) == 0 ){ // ".WFILL"
                int size =  atoi(recuperaToken(i+1)->palavra);
                int value =  atoi(recuperaToken(i+2)->palavra);
                int k;
                for (k = pos.end; k < pos.end + size; k++){
                    paramDireita[k] = value;
                    used[k] = 1;
                }
                pos.end = k;
                pos.dirEsq = 0;
            }
            else if(strcmp(toUpper(token->palavra),directives[4]) == 0 ){ // ".WORD"
               int value =  hexadecimalToDecimal(recuperaToken(i+1)->palavra);
               paramDireita[pos.end] = value;
               used[pos.end] = 1;
               avancar(&pos);
            }
        }
        if(token->tipo == Instrucao){
            int end;
            //imprimeListaTokens();
            char p[60];
            strcpy(p,token->palavra);
            for (int k = 0; k < 17; k++){
                if(strcmp(toUpper(token->palavra),instructions[k]) == 0 ){ 
                    end = k;
                    break;
                }
            }
            if(codigo[end] != 0){
                if(!pos.dirEsq){
                    instrucaoEsquerda[pos.end] = codigo[end];
                    if(parameters[end]){
                        //printf("param : %d\n",atoi(recuperaToken(i+1)->palavra));
                       paramEsquerda[pos.end] = atoi(recuperaToken(i+1)->palavra); 
                    }
                } else {
                    instrucaoDireita[pos.end] = codigo[end];
                    if(parameters[end]){
                        //printf("param : %d\n",atoi(recuperaToken(i+1)->palavra));
                        paramDireita[pos.end] = atoi(recuperaToken(i+1)->palavra); 
                    }
                }
                used[pos.end] =1;   


            } else {// 6,7,16 //
                if(recuperaToken(i+1)->tipo == Hexadecimal ){ // esquerda
                    if(strcmp(toUpper(token->palavra),instructions[6]) == 0 ) { // JUMP - 13
                        instrucaoEsquerda[pos.end] = 13;
                        paramEsquerda[pos.end] = hexadecimalToDecimal(recuperaToken(i+1)->palavra);
                    }
                    else if(strcmp(toUpper(token->palavra),instructions[7]) == 0 ){ // JUMP+ 15
                        instrucaoEsquerda[pos.end] = 15;
                        paramEsquerda[pos.end] = hexadecimalToDecimal(recuperaToken(i+1)->palavra);
                    }else if(strcmp(toUpper(token->palavra),instructions[16]) == 0 ){ // STOREND 18
                        instrucaoEsquerda[pos.end] = 18;
                        paramEsquerda[pos.end] = hexadecimalToDecimal(recuperaToken(i+1)->palavra);
                    }
                } 
                
                else if (recuperaToken(i+1)->tipo == Decimal ){
                    if(strcmp(toUpper(token->palavra),instructions[6]) == 0 ) { // JUMP - 13
                        instrucaoEsquerda[pos.end] = 13;
                        paramEsquerda[pos.end] =  atoi(recuperaToken(i+1)->palavra);
                    }
                    else if(strcmp(toUpper(token->palavra),instructions[7]) == 0 ){ // JUMP+ 15
                        instrucaoEsquerda[pos.end] = 15;
                        paramEsquerda[pos.end] =  atoi(recuperaToken(i+1)->palavra);
                    }else if(strcmp(toUpper(token->palavra),instructions[16]) == 0 ){ // STOREND 18
                        instrucaoEsquerda[pos.end] = 18;
                        paramEsquerda[pos.end] = atoi(recuperaToken(i+1)->palavra);
                    }
                } 
                
                else if(recuperaToken(i+1)->tipo == Nome){
                    printf("instr: %s   param: %s",token->palavra,recuperaToken(i+1)->palavra);
                }
                
                
            }
            
            //printf("%03X %02X %03X %02X %03X\n",enderecos[pos.end],instrucaoEsquerda[pos.end],paramEsquerda[pos.end],instrucaoDireita[pos.end],paramDireita[pos.end]);

            


            avancar(&pos);
        }
   
              
    }
    //  for (int i = 0; i < 1024; i++){
    //      if(used[i])
    //     printf("%03X %02X %03X %02X %03X\n",enderecos[i],instrucaoEsquerda[i],paramEsquerda[i],instrucaoDireita[i],paramDireita[i]);
    // }
    // for (int i = 0; i < numeroRotulos; i++){
    //     printf("rot: %s end: %03X dirEsq: %d\n",rotuloEnd[i].rotulo,rotuloEnd[i].end,rotuloEnd[i].dirEsq);
    // }
    
    
    //imprimeListaTokens();

    // for (int i = 0; i < numeroRotulos; i++){
    //     printf("end: %d, dirEsq %d, rotulo: %s\n",rotuloEnd[i].end,rotuloEnd[i].dirEsq,rotuloEnd[i].rotulo);
    // }
    // for (int i = 0; i < numeroSets; i++){
    //     printf("palavra %s, value %s\n",setValues[i].palavra,setValues[i].value);
    // }
    
    
        

    
        
    /* printf("Voce deve implementar essa função para a Parte 2!");*/
    return 0;
}
int main(){
    char*  string = ".SET TREZENTOS 300\n\n.org 0X300\n um: .word 1\n\n.align 16\nvetor: .wfill 15 TREZENTOS\n\n.org 0\nld vetor\naltend:\nadd um\nstore vetor\nadd 10\nstorend altend\njge 0\nrsh\nlsh\njump 400";    
    processarEntrada(string,strlen(string));
    //imprimeListaTokens();
    printf("\n");
    // if(!emitirMapaDeMemoria()) {
    //     printf("Certo\n");
    // }else{
    //     printf("Errado\n");
    // }  

}