#pragma once
#include <iostream>
#include <cmath>
#include <algorithm>
#include <limits>

class MenuNavegacao{
    private:
        std::string *opcoesMenu;
        const std::string tituloMenu;
        const int numOpcoes, pesoMenu, larguraMenu{50};
        const char caractereEspacamento{'*'};
        void mostraTitulo();
        void mostraOpcao(int indexOpcao, int valorOpcao);
    public:
        MenuNavegacao(std::string titulo, int pesoMenu, int numOpcoes, std::string *opcoes);
        ~MenuNavegacao();
        int mostraMenu();
};