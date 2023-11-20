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

    std::cout << "temp01: " << arquivoTemp_01->getNumRegistros() << "\n";
    std::cout << "temp02: " << arquivoTemp_02->getNumRegistros() << "\n";
    std::cout << "temp03: " << arquivoTemp_03->getNumRegistros() << "\n";
    std::cout << "temp04: " << arquivoTemp_04->getNumRegistros() << "\n";

    intercalacaoRegistros(arquivoTemp_01, arquivoTemp_02, arquivoTemp_03, arquivoTemp_04);
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


    }catch(std::runtime_error &e){
        std::cout << e.what() << "\n";
    }

}

void OrdenacaoExterna::intercalacaoRegistros(ArquivoBinario *fonteEntrada_01, ArquivoBinario *fonteEntrada_02, ArquivoBinario *fonteSaida_01, ArquivoBinario *fonteSaida_02){

    fonteEntrada_01->posicionaInicio();
    fonteEntrada_02->posicionaInicio();
    fonteSaida_01->posicionaInicio();
    fonteSaida_02->posicionaInicio();

    DadosEmprego *d1{new DadosEmprego}, *d1_anterior{new DadosEmprego}, *d2{new DadosEmprego}, *d2_anterior{new DadosEmprego};

    d1->apagar();
    d2->apagar();
    d1_anterior->apagar();
    d2_anterior->apagar();

    unsigned int contadorRegistros_01{0}, contadorRegistros_02{0}, contadorRegistrosIntercalados{0};
    bool fimBloco_01{false}, fimBloco_02{false}, lerBloco_01, lerBloco_02;

    while(contadorRegistros_01 < fonteEntrada_01->getNumRegistros() && contadorRegistros_02 < fonteEntrada_02->getNumRegistros()){

        lerBloco_01 = lerBloco_02 = false;

        if(d1->ehNulo() && d2->ehNulo()){ // primeira leitura de cada bloco  

            fonteEntrada_01->arquivo.read((char *) d1, sizeof(DadosEmprego));
            fonteEntrada_02->arquivo.read((char *) d2, sizeof(DadosEmprego));

        }
        
        if(!d1_anterior->ehNulo() && !d2_anterior->ehNulo()){
             if(*d1 < *d1_anterior){
                //novo bloco em arquivo 01
                fimBloco_01 = true;
            }else if(*d2 < *d2_anterior){
                //novo bloco em arquivo 02
                fimBloco_02 = true;
            }
        }

        if(!fimBloco_01 && !fimBloco_02){
            if(*d1 < *d2)
                lerBloco_01 = true;
            else
                lerBloco_02 = true;

        }else if(!fimBloco_01 && fimBloco_02)
            lerBloco_01 = true;
        else if(fimBloco_01 && !fimBloco_02)
            lerBloco_02 = true;

        if(lerBloco_01){
            fonteSaida_01->arquivo.write((char *) d1, sizeof(DadosEmprego));
            contadorRegistros_01++;
            contadorRegistrosIntercalados++;
            *d1_anterior = *d1;
            fonteEntrada_01->arquivo.read((char *) d1, sizeof(DadosEmprego));
        }else if(lerBloco_02){
            fonteSaida_01->arquivo.write((char *) d2, sizeof(DadosEmprego));
            contadorRegistros_02++;
            contadorRegistrosIntercalados++;
            *d2_anterior = *d2;
            fonteEntrada_02->arquivo.read((char *) d2, sizeof(DadosEmprego));
        }else{
            // fim bloco 01 e bloco 02
            fimBloco_01 = fimBloco_02 = false;
            d1_anterior->apagar();
            d2_anterior->apagar();
            fonteSaida_01->setNumRegistros(fonteSaida_01->getNumRegistros() + contadorRegistrosIntercalados);
            std::swap(fonteSaida_01, fonteSaida_02);
            contadorRegistrosIntercalados = 0;
        }

    }

    while(contadorRegistros_01 < fonteEntrada_01->getNumRegistros()){

        if(d1->ehNulo()) // nao entrou no loop anterior e nn leu o primeiro elemento do bloco
            fonteEntrada_01->arquivo.read((char *) d1, sizeof(DadosEmprego));
        else{
            fonteEntrada_01->arquivo.seekp(-1 * (sizeof(DadosEmprego)), std::ios::end);
            fonteEntrada_01->arquivo.read((char *) d1_anterior, sizeof(DadosEmprego));
        }   

        fonteSaida_01->arquivo.write((char *) d1, sizeof(DadosEmprego));
        contadorRegistros_01++;
        contadorRegistrosIntercalados++;
        *d1_anterior = *d1;
        fonteEntrada_01->arquivo.read((char *) d1, sizeof(DadosEmprego));

        if(*d1 < *d1_anterior){
            //novo bloco em arquivo 01
            fonteSaida_01->setNumRegistros(fonteSaida_01->getNumRegistros() + contadorRegistrosIntercalados);
            std::swap(fonteSaida_01, fonteSaida_02);
            contadorRegistrosIntercalados = 0;
        }
    }

    while(contadorRegistros_02 < fonteEntrada_02->getNumRegistros()){

        if(d2->ehNulo()) // nao entrou no loop anterior e nn leu o primeiro elemento do bloco
            fonteEntrada_02->arquivo.read((char *) d2, sizeof(DadosEmprego));
        else{
            fonteEntrada_02->arquivo.seekp(-1 * (sizeof(DadosEmprego)), std::ios::end);
            fonteEntrada_02->arquivo.read((char *) d2_anterior, sizeof(DadosEmprego));
        }   

        fonteSaida_01->arquivo.write((char *) d2, sizeof(DadosEmprego));
        contadorRegistros_02++;
        *d2_anterior = *d2;
        fonteEntrada_02->arquivo.read((char *) d2, sizeof(DadosEmprego));

        if(*d2 < *d2_anterior){
            //novo bloco em arquivo 02
            fonteSaida_01->setNumRegistros(fonteSaida_01->getNumRegistros() + contadorRegistrosIntercalados);
            std::swap(fonteSaida_01, fonteSaida_02);
            contadorRegistrosIntercalados = 0;
        }
    }

    fonteEntrada_01->setNumRegistros(0);
    fonteEntrada_02->setNumRegistros(0);

    delete d1;
    delete d1_anterior;
    delete d2;
    delete d2_anterior;

    if(fonteSaida_01->getNumRegistros() && fonteSaida_02->getNumRegistros())
        intercalacaoRegistros(fonteSaida_01, fonteSaida_02, fonteEntrada_01, fonteEntrada_02);
    else{
        std::cout << "temp01: " << arquivoTemp_01->getNumRegistros() << "\n";
        std::cout << "temp02: " << arquivoTemp_02->getNumRegistros() << "\n";
        std::cout << "temp03: " << arquivoTemp_03->getNumRegistros() << "\n";
        std::cout << "temp04: " << arquivoTemp_04->getNumRegistros() << "\n";
    }

}