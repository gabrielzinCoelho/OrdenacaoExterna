#pragma once
#include "menuNavegacao.hpp"

MenuNavegacao::MenuNavegacao(std::string titulo, int pesoMenu, int numOpcoes, std::string *opcoes)
    : numOpcoes(numOpcoes), pesoMenu(pesoMenu), tituloMenu(titulo) {

    opcoesMenu = opcoes;

}

MenuNavegacao::~MenuNavegacao(){
    delete[] opcoesMenu;
}

void MenuNavegacao::mostraTitulo(){

    if(int(tituloMenu.size()) >= larguraMenu){
        std::cout << tituloMenu.substr(0, larguraMenu);
    }else{

        int numEspacamentoAntes = std::ceil(float(larguraMenu)/2);

        for(int i{0}; i<numEspacamentoAntes - 1; i++)
            std::cout << caractereEspacamento;
        
        std::cout << " " << tituloMenu << " ";

        for(int i{0}; i<(larguraMenu - numEspacamentoAntes) - 1; i++)
            std::cout << caractereEspacamento;
    }

    std::cout << "\n\nEscolha uma opcao:\n\n";
}

void MenuNavegacao::mostraOpcao(int indexOpcao, int valorOpcao){

    std::cout << "[" << valorOpcao << "] " << opcoesMenu[indexOpcao] << "\n"; 

}

int MenuNavegacao::mostraMenu(){

    int opcao;

    mostraTitulo();
    
    for(int i{0}; i<numOpcoes; i++)
        mostraOpcao(i, i + 1);
    
    std::cout << "\n===> ";
    while (!(std::cin >> opcao) || opcao<=0 || opcao > numOpcoes){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "===> ";
    }

    return opcao + pesoMenu;
}