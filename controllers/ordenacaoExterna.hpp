#pragma once
#include <iostream>
#include <fstream>
#include "../components/minHeap.hpp"
#include "../components/arquivoBinario.hpp"
#include "../globals.cpp"

class OrdenacaoExterna{
    private:
        MinHeap *heapRegistros;
        ArquivoBinario *arquivoPrincipal, *arquivoTemp_01, *arquivoTemp_02, *arquivoTemp_03, *arquivoTemp_04;
        void distribuicaoRegistros();
        void intercalacaoRegistros(ArquivoBinario *fonteEntrada_01, ArquivoBinario *fonteEntrada_02, ArquivoBinario *fonteSaida);
        void mergeSort(ArquivoBinario *fonteEntrada_01, ArquivoBinario *fonteEntrada_02, ArquivoBinario *fonteSaida_01, ArquivoBinario *fonteSaida_02);
    public:
        OrdenacaoExterna(const std::string &nomeArq, float gB = 4);
        ~OrdenacaoExterna();

};