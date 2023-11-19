#pragma once
#include <stdexcept>
#include "./ordenacaoExterna.hpp"
#include "../components/minHeap.cpp"
#include "../components/arquivoBinario.cpp"

OrdenacaoExterna::OrdenacaoExterna(const std::string &nomeArq, float gB){

    const unsigned int gBParaByte = 1000000000; // 1 x 10^9
    unsigned int tamanhoMaxHeap = gB * gBParaByte/sizeof(DadosHeap), tamanhoHeap;

    arquivoPrincipal = new ArquivoBinario(nomeArq, std::ios::in);
    arquivoTemp_01 = new ArquivoBinario("temp01.dat");
    arquivoTemp_02 = new ArquivoBinario("temp02.dat");
    arquivoTemp_03 = new ArquivoBinario("temp03.dat");
    arquivoTemp_04 = new ArquivoBinario("temp04.dat");


    tamanhoHeap = arquivoPrincipal->getNumRegistros() <= tamanhoMaxHeap ? arquivoPrincipal->getNumRegistros() : tamanhoMaxHeap;

    DadosEmprego *d = new DadosEmprego[tamanhoHeap];
    arquivoPrincipal->arquivo.read((char *)d, sizeof(DadosEmprego) * tamanhoHeap);

    heapRegistros = new MinHeap(d, tamanhoHeap);
    delete[] d;

    distribuicaoRegistros();

}

OrdenacaoExterna::~OrdenacaoExterna(){
    
    delete arquivoPrincipal;
    delete arquivoTemp_01;
    delete arquivoTemp_02;
    delete arquivoTemp_03;
    delete arquivoTemp_04;

    // deletar temporarios

}

void OrdenacaoExterna::distribuicaoRegistros(){

    try{

        int pesoSegmento{0};
        ArquivoBinario *arquivoTempAtual{arquivoTemp_01}, *arquivoTempSeguinte{arquivoTemp_02};

        DadosHeap dadoHeap, dadoHeapRaiz;
        DadosEmprego dadoEmprego;
        unsigned int contadorRegistros{0};

        while(!heapRegistros->vazia()){

            if(arquivoPrincipal->arquivo.read((char *)&dadoEmprego, sizeof(DadosEmprego))){
                // ainda tem registros no arquivo principal (remove raiz inserindo novo valor lido)

                dadoHeapRaiz = heapRegistros->espiaRaiz();

                if(dadoHeapRaiz.pesoValor > pesoSegmento){
                    pesoSegmento = dadoHeapRaiz.pesoValor;
                    std::swap(arquivoTempAtual, arquivoTempSeguinte);
                    arquivoTempAtual->setNumRegistros(arquivoTempAtual->getNumRegistros() + contadorRegistros);
                    contadorRegistros = 0;
                }

                if(dadoEmprego < dadoHeapRaiz.valorDado){
                    dadoHeap = heapRegistros->removeInserindo(DadosHeap(&dadoEmprego, pesoSegmento + 1));

                }
                else
                    dadoHeap = heapRegistros->removeInserindo(DadosHeap(&dadoEmprego, pesoSegmento));

            }
            else
                dadoHeap = heapRegistros->removeRaiz();
            
            arquivoTempAtual->arquivo.write((char *)&dadoHeap.valorDado, sizeof(DadosEmprego));
            contadorRegistros++;
        }

        std::cout << "arquivo 1: " << arquivoTemp_01->getNumRegistros() << "\n";
        std::cout << "arquivo 2: " << arquivoTemp_02->getNumRegistros() << "\n";

    }catch(std::runtime_error &e){
        std::cout << e.what() << "\n";
    }

}

void OrdenacaoExterna::intercalacaoRegistros(){

    // std::pair<std::fstream*, std::fstream*> fontesEntrada = std::make_pair(&arquivoTemp_01, &arquivoTemp_02);
    // std::pair<std::fstream*, std::fstream*> fontesSaida = std::make_pair(&arquivoTemp_03, &arquivoTemp_04);

    // while()

}