#pragma once
#include <iostream>
#include <fstream>
#include "./gerenciaCsv.hpp"
#include "./gerenciaDat.hpp"
#include "../globals.cpp"

class EventosUsuario{
    private:
        GerenciaCsv *gerenciaCsv;
    public:
        EventosUsuario();
        ~EventosUsuario();
        void conversaoBinario();
        void conversaoCsv();
};