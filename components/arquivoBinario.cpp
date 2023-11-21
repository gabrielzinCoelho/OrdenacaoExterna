#pragma once
#include <stdexcept>
#include "./arquivoBinario.hpp"
#include "../controllers/ordenacaoExterna.hpp"

ArquivoBinario::ArquivoBinario(std::string nomeArq) : tamanhoCabecalho(sizeof(unsigned int)) {
    
    try{

        arquivo.open(caminhoPastaArquivos + nomeArq, std::ios::out);
        arquivo.close();
        arquivo.open(caminhoPastaArquivos + nomeArq, std::ios::in | std::ios::out);

        if (!arquivo.good())
            throw "Erro na abertura: " + nomeArq + "\n";

        numRegistros = 0;
        atualizarCabecalho();
        indexLeitura = tamanhoCabecalho;

    }catch(std::runtime_error &e){
        std::cout << e.what() << "\n";
        arquivo.clear();
    }

}

ArquivoBinario::ArquivoBinario(std::string nomeArq, std::ios_base::openmode modoAbertura) : tamanhoCabecalho(sizeof(unsigned int)) {
    
    try{

        arquivo.open(caminhoPastaArquivos + nomeArq, modoAbertura);

        if (!arquivo.good())
            throw "Erro na abertura: " + nomeArq + "\n";

        indexLeitura = tamanhoCabecalho;
        leituraCabecalho();

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
    arquivo.read((char *)&numRegistros, sizeof(int));

    //redefinirPonteiroArquivo();
}

void ArquivoBinario::atualizarCabecalho(){

    arquivo.seekp(0);
    arquivo.write((char *)&numRegistros, sizeof(unsigned int));
}

void ArquivoBinario::setNumRegistros(unsigned int numRegistros){
    this->numRegistros = numRegistros;
    indexLeitura = arquivo.tellp();
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

void ArquivoBinario::lerRegistro(DadosEmprego *d){
	arquivo.read((char *) d, sizeof(DadosEmprego));
	indexLeitura = arquivo.tellp();
}

void ArquivoBinario::desfazerLeitura(){
    indexLeitura-= sizeof(DadosEmprego);
    arquivo.seekp(indexLeitura);
}

bool ArquivoBinario::fimLeitura(){
	
	return (indexLeitura - tamanhoCabecalho)/sizeof(DadosEmprego) >= numRegistros;
	
}