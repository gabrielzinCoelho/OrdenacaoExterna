#pragma once
#include <fstream>
#include <iostream>
#include <utility>
#include "../globals.cpp"
#include "../models/dadosEmprego.hpp"

class GerenciaCsv{
private:
    std::fstream arquivoCsv;
    std::string nomeArq;
    int indexLeitura, taxaTransferencia;

public:
    GerenciaCsv(const std::string &nomeArq, int taxaTransferencia);
    void abrirCsv(std::ios_base::openmode modoAbertura);
    void fecharCsv();
    std::pair<DadosEmprego*, int> importarCsv();
};

GerenciaCsv::GerenciaCsv(const std::string &nomeArq, int taxaTransferencia = 1000){
    this->nomeArq = nomeArq;
    this->taxaTransferencia = taxaTransferencia;
    indexLeitura =  147;
    abrirCsv(std::ios::in);
    fecharCsv();
}

void GerenciaCsv::abrirCsv(std::ios_base::openmode modoAbertura = std::ios::in)
{
    try{
        arquivoCsv.open(caminhoPastaArquivos + nomeArq, modoAbertura);
        if(!arquivoCsv.good())
            throw "Erro na abertura: " + nomeArq + "\n";
    }
    catch (std::string err){
        std::cout << err;
        arquivoCsv.clear();
    }
}

void GerenciaCsv::fecharCsv(){
    arquivoCsv.close();
}

std::pair<DadosEmprego*, int> GerenciaCsv::importarCsv(){

    abrirCsv(std::ios::in);
    arquivoCsv.seekp(indexLeitura);

    DadosEmprego *dadosEmprego = new DadosEmprego[taxaTransferencia];

    int indexCampoRegistro, indexSubstr, contadorRegistros{0};
    const int numCamposRegistro{13};
    std::string camposRegistro[numCamposRegistro];

    const char separador{','};

    std::string linhaRegistro;

    while(std::getline(arquivoCsv, linhaRegistro) && contadorRegistros < taxaTransferencia){
        indexSubstr = indexCampoRegistro = 0;
        // utiliza a variavel indexSubstr para indexar cada campo do registro no csv

        for(int i{0}; i < int(linhaRegistro.size()); i++){
            if(linhaRegistro[i] == separador){

                if(linhaRegistro[indexSubstr] == '"')
                    camposRegistro[indexCampoRegistro] = linhaRegistro.substr(indexSubstr + 1, i - (indexSubstr + 2));
                else
                    camposRegistro[indexCampoRegistro] = linhaRegistro.substr(indexSubstr, i - indexSubstr);

                indexCampoRegistro++;
                indexSubstr = i + 1;

                // std::cout << camposRegistro[indexCampoRegistro - 1] << "\n";
            }
        }
        dadosEmprego[contadorRegistros++] = DadosEmprego(camposRegistro);
    }

    indexLeitura = arquivoCsv.tellp();
    fecharCsv();

    return std::make_pair(dadosEmprego, contadorRegistros);
}