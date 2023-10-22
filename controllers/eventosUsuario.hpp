#pragma once
#include <iostream>
#include <fstream>
#include "./gerenciaCsv.hpp"
#include "./gerenciaDat.hpp"
#include "../globals.cpp"

class EventosUsuario{
    private:
        GerenciaCsv *gerenciaCsv;
        GerenciaDat *gerenciaDat;
        void verificaDat();
    public:
        EventosUsuario();
        ~EventosUsuario();
        void defineDat();
        void resetaDat();
        void conversaoBinario();
        void conversaoCsv();
        void impressaoSecao();
        void impressaoGeral();
        void impressaoDetalhada();
        void inserirRegistro();
};