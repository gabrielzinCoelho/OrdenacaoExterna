#pragma once
#include <iostream>
#include <fstream>
#include "../components/minHeap.hpp"
#include "../globals.cpp"

class OrdenacaoExterna{
    private:
        unsigned int gigasRamDisponivel, numRegistros;
        MinHeap *heapRegistros;
        std::fstream arquivoPrincipal, arquivoTemp_01, arquivoTemp_02, arquivoTemp_03, arquivoTemp_04;
        bool abrirDat(const std::string &nomeArq, std::fstream *arqPtr, std::ios_base::openmode modoAbertura = std::ios::in);
        void leituraCabecalho();
        void atualizarCabecalho(std::fstream *arqPtr, unsigned int numRegistros);
        void distribuicaoRegistros();
    public:
        OrdenacaoExterna(const std::string &nomeArq, float gB = 4);
        ~OrdenacaoExterna();

};