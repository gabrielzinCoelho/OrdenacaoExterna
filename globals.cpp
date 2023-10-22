#pragma once
#include <iostream>

const std::string caminhoPastaArquivos{"./baseDados/"};
const int numCamposRegistro{13};
const int taxaTransferencia{1000};

std::string *camposRegistroRotulo = new std::string[numCamposRegistro]{
    "Series reference: ",
    "Period: ",
    "Data value: ",
    "Status: ",
    "Units: ",
    "Magntude: ",
    "Subject: ",
    "Group: ",
    "Series title (1): ",
    "Series title (2): ",
    "Series title (3): ",
    "Series title (4): ",
    "Series title (5): "
};

void confirmacaoUsuario(){
    std::cout << "Pressione qualquer tecla para prosseguir...\n";

    try{
        char c;
        std::cin >> c;
    }catch(std::string err){}
}

bool ehNumero(std::string s){

    for(int i{0}; i<int(s.size()); i++)
        if(!std::isdigit(s[i]))
            return false;
    
    return true;

}
