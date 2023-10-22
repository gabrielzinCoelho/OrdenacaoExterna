#pragma once
#include <iostream>

const std::string caminhoPastaArquivos{"./baseDados/"};
const int numCamposRegistro{13};
const int taxaTransferencia{1000};

void confirmacaoUsuario(){
    std::cout << "Pressione qualquer tecla para prosseguir...\n";

    try{
        char c;
        std::cin >> c;
    }catch(std::string err){}
}