#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ORIGIN_FILE "cep.dat"
#define DESTINY_FILE "indice.dat"

typedef struct _Endereco Endereco;

struct _Endereco
{
    char logradouro [72];
    char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

typedef struct _Registro Registro;

struct _Registro
{
    char cep[8];
    int posicao;
};

int compare (const void *e1, const void *e2) 
{
    return strncmp(((Registro*)e1)->cep, ((Registro*)e2)->cep,8);
}

int main (int argc, char **argv)
{
    FILE *file;
    long endFILE, totalAdress;
    int index = 0;

    file = fopen(ORIGIN_FILE, "rb");
    if(file == NULL) 
    {
        printf("Error. Couldn't open file");
        return -1;
    }

    fseek(file, 0, SEEK_END); //set pointer to the end of the file

    endFILE = ftell(file); //return the number of bytes conteined in the file
    totalAdress = endFILE/sizeof(Endereco);

    Endereco *adress = (Endereco*) malloc(sizeof(Endereco));
    Registro *registro = (Registro*) malloc(sizeof(Registro)* totalAdress);

    if(registro == NULL) {
        printf("Error: Not possible to allocate memory to registers");
        fclose(file);
        return -1;
    }

    rewind(file); //return to the beggining of file

    while (fread(adress,sizeof(Endereco), 1, file) > 0)
    {
        strncpy(registro[index].cep, adress->cep, 8);
        registro[index].posicao = index;
        index++;
        
        //printf("%.8s\n", adress->cep);
        if (index >= totalAdress) {break;}
        
    }
    fclose(file);

    qsort(registro, totalAdress, sizeof(Registro),compare);
    printf("The registers have been ordenated\n");

    file = fopen(DESTINY_FILE, "wb");
    if(file == NULL) 
    {
        printf("Error. Couldn't open file");
        return -1;
    }

    if (fwrite(registro, sizeof(Registro), totalAdress, file) != totalAdress) {
        printf("Error writing to file\n");
        fclose(file);
        return -1;
    }
    
    fclose(file);
    printf("Write = OK");
    
    free(registro);
}