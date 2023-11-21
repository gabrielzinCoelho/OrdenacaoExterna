#pragma once
#include <iostream>
#include <fstream>
#include "../globals.cpp"

class ArquivoBinario{
    friend class OrdenacaoExterna;
    private:
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
        void lerRegistro(DadosEmprego *d);
        void desfazerLeitura();
        bool fimLeitura();
};