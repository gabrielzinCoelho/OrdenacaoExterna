#pragma once
#include <iostream>
#include <fstream>
#include <utility>
#include "../models/dadosEmprego.hpp"
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
};

void GerenciaDat::definirParametrosLeitura(int indexLeitura = -1){
    if(indexLeitura < tamanhoCabecalho)
        this->indexLeitura = tamanhoCabecalho;
    else
        this->indexLeitura = indexLeitura;
}

GerenciaDat::GerenciaDat(const std::string& nomeArq, int taxaTransferencia = 1000) 
    : nomeArq(nomeArq), taxaTransferencia(taxaTransferencia), tamanhoCabecalho(sizeof(unsigned int)){
    
    if(!abrirDat(std::ios::in|std::ios::out)){
        std::cout << "Criando dat de saída...\n";
        numRegistros = 0;
        abrirDat(std::ios::out);
        atualizarCabecalho();
    }else{
        leituraCabecalho();
    }
    definirParametrosLeitura();
    fecharDat();
}

bool GerenciaDat::abrirDat(std::ios_base::openmode modoAbertura = std::ios::in){
    try{
        arquivoDat.open(caminhoPastaArquivos + nomeArq, modoAbertura);

        if(!arquivoDat.good())
            throw "Erro na abertura: " + nomeArq + "\n";
    
        return true;
    }catch(std::string err){
        std::cout << err;
        arquivoDat.clear();
        return false;
    }
}

void GerenciaDat::fecharDat(){
    arquivoDat.close();
}
void GerenciaDat::atualizarCabecalho(){
    
    if(!arquivoDat.good())
        abrirDat(std::ios::in|std::ios::out);
    
    arquivoDat.seekp(0);
    arquivoDat.write((char *) &numRegistros, sizeof(int));

}

void GerenciaDat::leituraCabecalho(){
    if(!arquivoDat.good())
        abrirDat(std::ios::in|std::ios::out);
    arquivoDat.seekp(0);
    arquivoDat.read((char *) &numRegistros, sizeof(int));
}

std::pair<DadosEmprego*, int> GerenciaDat::importarDat(){

    abrirDat(std::ios::in);
    arquivoDat.seekp(indexLeitura, std::ios::beg);

    int dadosRestantes = numRegistros - ((indexLeitura - tamanhoCabecalho)/sizeof(DadosEmprego));
    int tamanhoLeitura = dadosRestantes > taxaTransferencia ? taxaTransferencia : dadosRestantes;

    if(!tamanhoLeitura)
        return std::make_pair(nullptr, 0);

    DadosEmprego *dadosEmprego = new DadosEmprego[tamanhoLeitura];
    arquivoDat.read((char *) dadosEmprego, sizeof(DadosEmprego)*tamanhoLeitura);

    indexLeitura = arquivoDat.tellp();
    fecharDat();

    return std::make_pair(dadosEmprego, tamanhoLeitura);
}

void GerenciaDat::exportarDat(std::pair<DadosEmprego*, int> dadosExportados){

    abrirDat(std::ios::in|std::ios::out);

    arquivoDat.seekp(0, std::ios::end);
    arquivoDat.write((char *) dadosExportados.first, (sizeof(DadosEmprego)*dadosExportados.second));

    numRegistros += dadosExportados.second;
    atualizarCabecalho();

    fecharDat();
}