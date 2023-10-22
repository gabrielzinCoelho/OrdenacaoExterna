#pragma once
#include <iostream>
#include <fstream>
#include <utility>
#include "../globals.cpp"
#include "../models/dadosEmprego.cpp"

class GerenciaCsv{
    private:
        std::fstream arquivoCsv;
        const int taxaTransferencia;
        unsigned long long int indexLeitura;
        const char separador{','};
        std::string tratamentoEscape(const std::string &);
        bool abrirCsv(const std::string &, std::ios_base::openmode);
        void fecharCsv();
    public:
        GerenciaCsv(int);
        GerenciaCsv(const std::string &, int);
        std::pair<DadosEmprego*, int> importarCsv(const std::string &);
        void definirParametrosLeitura(const std::string &);
        void exportarCsv(const std::string &, std::pair<DadosEmprego*, int>);
};

