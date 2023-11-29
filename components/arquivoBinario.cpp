#pragma once
#include <stdexcept>
#include <cmath>
#include "./arquivoBinario.hpp"
#include "../controllers/ordenacaoExterna.hpp"

ArquivoBinario::ArquivoBinario(std::string nomeArq) : nomeArq(nomeArq), tamanhoCabecalho(sizeof(unsigned int)) {
    
    try{

        arquivo.open(caminhoPastaArquivos + nomeArq, std::ios::out);
        arquivo.close();
        arquivo.open(caminhoPastaArquivos + nomeArq, std::ios::in | std::ios::out);

        if (!arquivo.good())
            throw "Erro na abertura: " + nomeArq + "\n";

        numRegistros = 0;
        atualizarCabecalho();
        posicionaInicio();

    }catch(std::runtime_error &e){
        std::cout << e.what() << "\n";
        arquivo.clear();
    }

}

ArquivoBinario::ArquivoBinario(std::string nomeArq, std::ios_base::openmode modoAbertura) : nomeArq(nomeArq), tamanhoCabecalho(sizeof(unsigned int)) {
    
    try{

        arquivo.open(caminhoPastaArquivos + nomeArq, modoAbertura);

        if (!arquivo.good())
            throw "Erro na abertura: " + nomeArq + "\n";

        leituraCabecalho();
        posicionaInicio();

    }catch(std::runtime_error &e){
        std::cout << e.what() << "\n";
        arquivo.clear();
    }

}

ArquivoBinario::~ArquivoBinario(){
    arquivo.close();
}

void ArquivoBinario::leituraCabecalho(){

    arquivo.seekp(0);
    arquivo.read((char *)&numRegistros, sizeof(unsigned int));

    //redefinirPonteiroArquivo();
}

void ArquivoBinario::atualizarCabecalho(){

    arquivo.seekp(0);
    arquivo.write((char *)&numRegistros, sizeof(unsigned int));
}

void ArquivoBinario::setNumRegistros(unsigned int numRegistros){
    this->numRegistros = numRegistros;
    atualizarCabecalho();
    arquivo.seekp(indexLeitura);
}

unsigned int ArquivoBinario::getNumRegistros(){
    return numRegistros;
}

void ArquivoBinario::posicionaInicio(){

    indexLeitura = tamanhoCabecalho;
    arquivo.seekp(indexLeitura);

}

void ArquivoBinario::posicionaIndex(){

    arquivo.seekp(indexLeitura);

}

void ArquivoBinario::lerRegistro(DadosEmprego *d, int numRegistros){

	arquivo.read((char *) d, sizeof(DadosEmprego) * numRegistros);
	indexLeitura += sizeof(DadosEmprego) * numRegistros;
}

void ArquivoBinario::escreverRegistro(DadosEmprego *d){

	arquivo.write((char *) d, sizeof(DadosEmprego));
	indexLeitura += sizeof(DadosEmprego);
}

void ArquivoBinario::desfazerLeitura(){
    indexLeitura-= sizeof(DadosEmprego);
    arquivo.seekp(indexLeitura);
}

bool ArquivoBinario::fimLeitura(){
	
    // unsigned int registrosLidos = std::ceil((indexLeitura - tamanhoCabecalho) / float(sizeof(DadosEmprego)));
    unsigned int registrosLidos = (indexLeitura - tamanhoCabecalho) / sizeof(DadosEmprego);
	return registrosLidos >= numRegistros;
	
}

std::string ArquivoBinario::getNome(){
    return nomeArq;
}

void ArquivoBinario::close(){
    arquivo.close();
}