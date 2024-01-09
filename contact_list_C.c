#include <stdio.h>
#include <string.h>
#include<stdbool.h>

// ----------------------- Struct e e outras definições ----------------------- //

typedef struct{
	char userName[51];
	char userPhone[16];
	char contactType[11];
} contact;

#define LIMIT 100
#define FILE_NAME "contact.ccf"

// ----------------------- Vetor ----------------------- //

contact contacts[LIMIT]; 
int counter = 0;

// ----------------------- Específica como as informações serão alocadas no arquivo ----------------------- //

void contactSave(contact ctt, FILE *file){
    fputs(ctt.userName, file);
    fputs(ctt.userPhone, file);
    fputs(ctt.contactType, file);
}

bool fileRewrite(int ignore){
    bool r = false;
    contact temp[LIMIT];
    int n = 0;

    remove(FILE_NAME);
    FILE *file = fopen(FILE_NAME, "a+");

    if(file != NULL){
        for(int index = 0; index < counter; index++){
            if(index != ignore){
                contactSave(contacts[index], file);
                temp[n] = contacts[index];
                n++;
            }
        }

        for(int index = 0; index < n; index++){
            contacts[index] = temp[index];
        }
        counter = n;
        r = true;
    } 
    return r;
}

// ----------------------- Carrega o arquivo com os contatos ----------------------- //

void contactLoad(){
    FILE *file = fopen(FILE_NAME, "r");

    if(file != NULL){
        counter = 0;

        while(!feof(file)){
            fgets(contacts[counter].userName, 50, file);
            if(strcmp(contacts[counter].userName, "") > 0){
                fgets(contacts[counter].userPhone, 15, file);
                fgets(contacts[counter].contactType, 11, file);
                counter++;
            }
        }
    }else{
        printf("Erro ao carregar os contatos.\n");
    }

    fclose(file);
}

// ----------------------- Verifica a existência ou cria um novo arquivo ----------------------- //

bool contactInsert(contact ctt){

    bool r = false;
    FILE *file = fopen(FILE_NAME, "a+");

    if(file != NULL){
        r = true;

        contactSave(ctt, file);
        contacts[counter] = ctt;
        counter++;
    }

    fclose(file);
    return r;
}

// ----------------------- Função criar contato ----------------------- //

void contactCreate(){

    if(counter < LIMIT){

    printf("\nNOVO CONTATO\n");

    contact ctt;

    printf("Informe o nome do contato: ");
    fgets(ctt.userName, 50, stdin);
    fflush(stdin);

    printf("Informe o numero do contato: ");
    fgets(ctt.userPhone, 15, stdin);
    fflush(stdin);

    printf("Informe o tipo do contato: ");
    fgets(ctt.contactType, 10, stdin);
    fflush(stdin);

    if(contactInsert(ctt)){
        printf("\nContato salvo.\n");
    }else{
        printf("\nErro ao salvar contato.\n");
    }
    }else{
        printf("\nMemória cheia.\n");
    }

}

// ----------------------- Função alterar contato ----------------------- //

void contactEdit(int index){

    contact ctt;

    printf("\nEDITAR CONTATO\n");

    printf("Informe o novo nome | Aperte enter para nao alterar o nome | Nome atual: %s", contacts[index].userName);
    fgets(ctt.userName, 50, stdin);
    fflush(stdin);

    if(strlen(ctt.userName) > 1){
        strcpy(contacts[index].userName, ctt.userName);
    }

    printf("Informe o novo numero | Aperte enter para nao alterar o numero | Numero atual: %s", contacts[index].userPhone);
    fgets(ctt.userPhone, 15, stdin);
    fflush(stdin);

    if(strlen(ctt.userPhone) > 1){
        strcpy(contacts[index].userPhone, ctt.userPhone);
    }

    printf("Informe o novo tipo de contato | Aperte enter para nao alterar o tipo de contato | Tipo de contato atual: %s", contacts[index].contactType);
    fgets(ctt.contactType, 10, stdin);
    fflush(stdin);

    if(strlen(ctt.contactType) > 1){
        strcpy(contacts[index].contactType, ctt.contactType);
    }

    if(fileRewrite(-1)){
        printf("\nContato alterado com sucesso.\n");
    }else{
        printf("\nErro ao atualizar contato.\n");
    }
}

bool confirm(char *message){
    char r;
    printf("%s S para sim, outro para NAO", message);
    scanf("%c", &r);

    if(r == 's' || r == 'S'){
        return true;
    }else{
        return false;
    }

}

void contactDelete(int index){
    if(confirm("\nDeseha excluir este contato?\n\n")){
        if(fileRewrite(index)){
            printf("\nContato excluido com sucesso\n\n");
        }
    }
}

void menuSelect(){
    int selected = -1;
    int option = 0;

    printf("\nInforme o codigo do contato para selecionar e iniciar a edicao | 0 para voltar ao menu: ");
    scanf("%i", &selected);
    fflush(stdin);

    if(selected > 0){
        if(selected <= LIMIT){
            selected--;
            printf("\nSelecionado: %s\n", contacts[selected].userName);
            printf("1 - Editar\n");
            printf("2 - Excluir\n");
            printf("0 - Sair\n");

            scanf("%i", &option);
            fflush(stdin);

            switch(option){
                case 1: contactEdit(selected);
                break;
                case 2: contactDelete(selected);
                break;
            }
        }else{
            printf("Contato inexistente.\n\n");
        }
    }
}


// ----------------------- Função para mostrar os contatos ----------------------- //

void contactShow(int index){
    printf("[%i]\n", index+1);
    printf("%s", contacts[index].userName);
    printf("%s", contacts[index].userPhone);
    printf("%s", contacts[index].contactType);
}

// ----------------------- Função listar contato ----------------------- //

void contactList(){
    printf("\nLISTA DE CONTATOS\n\n");

    for(int index = 0; index < counter; index++){
        contactShow(index);
        printf("--------------------------------\n");
    }
    if(counter > 0){
        menuSelect();
    }
}

// ----------------------- Funções de localizar ----------------------- //

bool searchMatch(char *a, char *b, int index, int limit){
    if(a[index] == b[index]){
        index++;
        if(index < limit){
            return searchMatch(a, b, index, limit);
        }else{
            return true;
        }
    }else{
        return false;
    }
}

void contactLocate(){
    char userName[51] = {""};
    bool found = false;

    printf("\nLOCALIZAR CONTATO\n\n");

    printf("Digite o nome para busca: ");
    fgets(userName, 50, stdin);
    fflush(stdin);

    int limit = strlen(userName);
    limit--;

    printf("\nRESULTADO\n\n");
    for(int index = 0; index < counter; index++){
        if(strlen(contacts[index].userName) >= limit){
            if(searchMatch(userName, contacts[index].userName, 0, limit)){
                contactShow(index);
                printf("-----------------------\n");
                found = true;
            }
        }
    }
    if(found){
        menuSelect();
    }
}

// ----------------------- Função menu ----------------------- //

void menu(){ 
    int option = 0;

    while(option != 9){
        printf("Escolha a operacao que deseja realizar:\n");
        printf("1 - Criar um novo contato\n");
        printf("2 - Listar todos os contatos cadastrados\n");
        printf("3 - Localizar um contato\n");
        printf("9 - Sair do menu\n");

        scanf("%i", &option);
        fflush(stdin);

        if(option != 9){
            switch(option){
                case 1: contactCreate();
                break;
                case 2: contactList();
                break;
                case 3: contactLocate();
                break;
                default: printf("\nOpcao invalida\n");
            }
        }else{
            printf("\nSaida efetuada\n");
        }
    }
}

// ----------------------- Main ----------------------- //

int main(){
    contactLoad();
    menu();
}
