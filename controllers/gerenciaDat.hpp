#pragma once
#include <iostream>
#include <fstream>
#include <utility>
#include "../models/dadosEmprego.cpp"
#include "../globals.cpp"

class GerenciaDat{
    private:
        std::fstream arquivoDat;
        const std::string nomeArq;
        const int taxaTransferencia, tamanhoCabecalho;
        unsigned long long int indexLeitura;
        unsigned int numRegistros;
        bool abrirDat(std::ios_base::openmode);
        void fecharDat();
        void atualizarCabecalho();
        void leituraCabecalho();
    public:
        GerenciaDat(const std::string&, int taxaTransferencia);
        std::pair<DadosEmprego*, int> importarDat();
        void exportarDat(std::pair<DadosEmprego*, int>);
        void definirParametrosLeitura(int);
        unsigned int getNumRegistros();
        void imprimirRegistros(bool imprimirTudo, long int posInical = -1, long int posFinal = -1, bool impressaoDetalhada = false);
        void inserirRegistro(std::string *camposRegistro);
};