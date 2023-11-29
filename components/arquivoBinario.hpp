#pragma once
#include <iostream>
#include <fstream>
#include "../globals.cpp"

class ArquivoBinario{
    friend class OrdenacaoExterna;
    private:
        const std::string nomeArq;
        std::fstream arquivo;
        unsigned long long int indexLeitura;
        unsigned int numRegistros;
        const int tamanhoCabecalho;
    public:
        ArquivoBinario(std::string nomeArq);
        ArquivoBinario(std::string nomeArq, std::ios_base::openmode modoAbertura);
        ~ArquivoBinario();
        bool abrirDat(const std::string &nomeArq, std::fstream *arqPtr, std::ios_base::openmode modoAbertura = std::ios::in);
        void leituraCabecalho();
        void atualizarCabecalho();
        void setNumRegistros(unsigned int numRegistros);
        unsigned int getNumRegistros();
        void posicionaInicio();
        void posicionaIndex();
        void lerRegistro(DadosEmprego *d, int numRegistros = 1);
        void escreverRegistro(DadosEmprego *d);
        void desfazerLeitura();
        bool fimLeitura();
        std::string getNome();
        void close();
};