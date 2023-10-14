#pragma once
#include <fstream>
#include <iostream>
#include <utility>
#include "../globals.cpp"
#include "../models/dadosEmprego.hpp"

class GerenciaCsv{
    private:
        std::fstream arquivoCsv;
        int taxaTransferencia;
        unsigned long long int indexLeitura;
        const char separador{','};
    public:
        GerenciaCsv(int);
        GerenciaCsv(const std::string &, int);
        bool abrirCsv(const std::string &, std::ios_base::openmode);
        void fecharCsv();
        std::pair<DadosEmprego*, int> importarCsv(const std::string &);
        void definirParametrosLeitura(const std::string &);
        void exportarCsv(const std::string &, std::pair<DadosEmprego*, int>);
};

GerenciaCsv::GerenciaCsv(int taxaTransferencia = 1000){
    this->taxaTransferencia = taxaTransferencia;
}

GerenciaCsv::GerenciaCsv(const std::string &nomeArq, int taxaTransferencia = 1000){
    this->taxaTransferencia = taxaTransferencia;
    definirParametrosLeitura(nomeArq);
}

void GerenciaCsv::definirParametrosLeitura(const std::string &nomeArq){

    abrirCsv(nomeArq, std::ios::in);

    std::string cabecalhoCsv;
    std::getline(arquivoCsv, cabecalhoCsv);
    indexLeitura =  int(arquivoCsv.tellp()) - 2;
    
    fecharCsv();
}

bool GerenciaCsv::abrirCsv(const std::string &nomeArq, std::ios_base::openmode modoAbertura = std::ios::in){
    try{
        arquivoCsv.open(caminhoPastaArquivos + nomeArq, modoAbertura);
        if(!arquivoCsv.good())
            throw "Erro na abertura: " + nomeArq + "\n";
        return true;
    }
    catch (std::string err){
        std::cout << err;
        arquivoCsv.clear();
        return false;
    }
}

void GerenciaCsv::fecharCsv(){
    arquivoCsv.close();
}

std::pair<DadosEmprego*, int> GerenciaCsv::importarCsv(const std::string &nomeArq){

    abrirCsv(nomeArq, std::ios::in);

    arquivoCsv.seekp(indexLeitura, std::ios::beg);

    DadosEmprego *dadosEmprego = new DadosEmprego[taxaTransferencia];

    int indexCampoRegistro, indexSubstr, contadorRegistros{0};
    const int numCamposRegistro{13};
    std::string camposRegistro[numCamposRegistro], linhaRegistro;
    bool escapeCaractere;

    while(std::getline(arquivoCsv, linhaRegistro) && contadorRegistros < taxaTransferencia){
        indexSubstr = indexCampoRegistro = 0;
        escapeCaractere = false;
        // utiliza a variavel indexSubstr para indexar cada campo do registro no csv

        for(int i{0}; i < int(linhaRegistro.size()); i++){

            if(linhaRegistro[i] == '"'){
                escapeCaractere = !escapeCaractere;
            }
            else if(linhaRegistro[i] == separador && !escapeCaractere){

                if(linhaRegistro[indexSubstr] == '"')
                    camposRegistro[indexCampoRegistro] = linhaRegistro.substr(indexSubstr + 1, i - (indexSubstr + 2));
                else
                    camposRegistro[indexCampoRegistro] = linhaRegistro.substr(indexSubstr, i - indexSubstr);

                indexCampoRegistro++;
                indexSubstr = i + 1;

            }
        }
        dadosEmprego[contadorRegistros++] = DadosEmprego(camposRegistro);
    }

    indexLeitura = arquivoCsv.tellp();
    fecharCsv();

    std::cout << indexLeitura << " **\n";

    return std::make_pair(dadosEmprego, contadorRegistros);
}

void GerenciaCsv::exportarCsv(const std::string &nomeArq, std::pair<DadosEmprego*, int> dadosExportados){
    
    if(!abrirCsv(nomeArq, std::ios::in|std::ios::out)){
        std::cout << "Criando csv de saída...\n";
        abrirCsv(nomeArq, std::ios::out);
        fecharCsv();
        abrirCsv(nomeArq, std::ios::in|std::ios::out);
    }
    arquivoCsv.seekg(0, std::ios::end);

    for(int i{0}; i<dadosExportados.second; i++){

        arquivoCsv << dadosExportados.first[i].seriesId << separador
        << dadosExportados.first[i].periodo << separador
        << (dadosExportados.first[i].valor == -1 ? "" : std::to_string(dadosExportados.first[i].valor)) << separador
        << dadosExportados.first[i].status << separador
        << dadosExportados.first[i].unidadeMedidaValor << separador
        << dadosExportados.first[i].magnitudeValor << separador
        << dadosExportados.first[i].descricao << separador
        << dadosExportados.first[i].categoria << separador
        << dadosExportados.first[i].tituloSerie_1 << separador
        << dadosExportados.first[i].tituloSerie_2 << separador
        << dadosExportados.first[i].tituloSerie_3 << separador
        << dadosExportados.first[i].tituloSerie_4 << separador
        << dadosExportados.first[i].tituloSerie_5 << "\n";
    }
    fecharCsv();
}