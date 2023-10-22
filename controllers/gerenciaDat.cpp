#pragma once
#include "gerenciaDat.hpp"

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

    if(!abrirDat(std::ios::in))
        throw nomeArq + " não foi inicializado corretamente.";
        
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

unsigned int GerenciaDat::getNumRegistros(){
    return numRegistros;
}

void GerenciaDat::imprimirRegistros(bool imprimirTudo, long int posInicial, long int posFinal, bool impressaoDetalhada){

    try{

        if(!abrirDat(std::ios::in))
            throw std::string(nomeArq + " não foi inicializado corretamente.");

        if(!numRegistros)
            throw std::string("Não há nada para visualizar...");
        
        if(imprimirTudo){
            posInicial = 0;
            posFinal = numRegistros - 1;
        }

        if (posInicial < 0 || posInicial >= numRegistros || posFinal < 0 || posFinal >= numRegistros)
            throw std::string("Posicoes de registro invalidas.");

        long ordemImpressao{(posInicial <= posFinal) ? 1 : -1};
        long int contador{posInicial};
        DadosEmprego *registro = new DadosEmprego();

        std::cout << "\n\n";

        while ((contador * ordemImpressao) <= (posFinal * ordemImpressao)){

            arquivoDat.seekp(tamanhoCabecalho + contador * sizeof(DadosEmprego), std::ios::beg);
            arquivoDat.read((char *)registro, sizeof(DadosEmprego));
            
            if(impressaoDetalhada)
                registro->impressaoDetalhada();
            else
                registro->impressaoResumida();

            contador += ordemImpressao;
        }

        delete registro;
        fecharDat();

    }catch(std::string err){
        fecharDat();
        std::cout << err << "\n";
    }

}

void GerenciaDat::inserirRegistro(std::string *camposRegistro){

    abrirDat(std::ios::in|std::ios::out);

    DadosEmprego *registro = new DadosEmprego(camposRegistro);

    arquivoDat.seekp(0, std::ios::end);
    arquivoDat.write((char *) registro, (sizeof(DadosEmprego)));

    numRegistros ++;
    atualizarCabecalho();

    std::cout << "Registro criado com sucesso.\n";

    fecharDat();
}


