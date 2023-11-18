#pragma once
#include <stdexcept>
#include "./ordenacaoExterna.hpp"
#include "../components/minHeap.cpp"

OrdenacaoExterna::OrdenacaoExterna(const std::string &nomeArq, float gB){

    const unsigned int gBParaByte = 1000000000; // 1 x 10^9
    unsigned int tamanhoMaxHeap = gB * gBParaByte/sizeof(DadosHeap), tamanhoHeap;

    abrirDat(nomeArq, &arquivoPrincipal);

    abrirDat("temp01.dat", &arquivoTemp_01, std::ios::out);
    arquivoTemp_01.close();
    abrirDat("temp01.dat", &arquivoTemp_01, std::ios::in | std::ios::out);

    abrirDat("temp02.dat", &arquivoTemp_02, std::ios::out);
    arquivoTemp_02.close();
    abrirDat("temp02.dat", &arquivoTemp_02, std::ios::in | std::ios::out);

    // abrirDat("temp03.dat", &arquivoTemp_03, std::ios::in | std::ios::out);
    // abrirDat("temp04.dat", &arquivoTemp_04, std::ios::in | std::ios::out);

    leituraCabecalho();
    atualizarCabecalho(&arquivoTemp_01, 0);
    atualizarCabecalho(&arquivoTemp_02, 0);

    tamanhoHeap = numRegistros <= tamanhoMaxHeap ? numRegistros : tamanhoMaxHeap;

    DadosEmprego *d = new DadosEmprego[tamanhoHeap];
    arquivoPrincipal.read((char *)d, sizeof(DadosEmprego) * tamanhoHeap);

    heapRegistros = new MinHeap(d, tamanhoHeap);
    delete[] d;

    distribuicaoRegistros();

}

OrdenacaoExterna::~OrdenacaoExterna(){
    
    arquivoPrincipal.close();
    arquivoTemp_01.close();
    arquivoTemp_02.close();
    arquivoTemp_03.close();
    arquivoTemp_04.close();
    // remover arquivos temporarios do disco
}

bool OrdenacaoExterna::abrirDat(const std::string &nomeArq, std::fstream *arqPtr, std::ios_base::openmode modoAbertura){
    try
    {
        
        arqPtr->open(caminhoPastaArquivos + nomeArq, modoAbertura);

        if (!arqPtr->good())
            throw "Erro na abertura: " + nomeArq + "\n";

        return true;
    }
    catch (std::string err)
    {
        std::cout << err;
        arqPtr->clear();
        return false;
    }
}

void OrdenacaoExterna::leituraCabecalho()
{
    arquivoPrincipal.seekp(0);
    arquivoPrincipal.read((char *)&numRegistros, sizeof(int));
}

void OrdenacaoExterna::atualizarCabecalho(std::fstream *arqPtr, unsigned int numRegistros){

    arqPtr->seekp(0);
    arqPtr->write((char *)&numRegistros, sizeof(unsigned int));
}


void OrdenacaoExterna::distribuicaoRegistros(){

    int pesoSegmento{0};
    std::fstream *arquivoTempAtual{&arquivoTemp_01}, *arquivoTempSeguinte{&arquivoTemp_02};

    try{

        DadosHeap dadoHeap, dadoHeapRaiz;
        DadosEmprego dadoEmprego;

        while(!heapRegistros->vazia()){

            if(arquivoPrincipal.read((char *)&dadoEmprego, sizeof(DadosEmprego))){
                // ainda tem registros no arquivo principal (remove raiz inserindo novo valor lido)

                dadoHeapRaiz = heapRegistros->espiaRaiz();

                if(dadoHeapRaiz.pesoValor > pesoSegmento){
                    pesoSegmento = dadoHeapRaiz.pesoValor;
                    std::swap(arquivoTempAtual, arquivoTempSeguinte);
                }

                if(dadoEmprego < dadoHeapRaiz.valorDado){
                    dadoHeap = heapRegistros->removeInserindo(DadosHeap(&dadoEmprego, pesoSegmento + 1));

                }
                else
                    dadoHeap = heapRegistros->removeInserindo(DadosHeap(&dadoEmprego, pesoSegmento));

            }
            else
                dadoHeap = heapRegistros->removeRaiz();

            arquivoTempAtual->write((char *)&dadoHeap.valorDado, sizeof(DadosEmprego));

        }

        

    }catch(std::runtime_error &e){
        std::cout << e.what() << "\n";
    }

}
