#pragma once
#include <iostream>
#include <fstream>
#include "../components/minHeap.hpp"
#include "../components/arquivoBinario.hpp"
#include "../globals.cpp"

class OrdenacaoExterna{
    private:
        unsigned int gigasRamDisponivel;
        MinHeap *heapRegistros;
        ArquivoBinario *arquivoPrincipal, *arquivoTemp_01, *arquivoTemp_02, *arquivoTemp_03, *arquivoTemp_04;
        void distribuicaoRegistros();
        void intercalacaoRegistros();
    public:
        OrdenacaoExterna(const std::string &nomeArq, float gB = 4);
        ~OrdenacaoExterna();

};