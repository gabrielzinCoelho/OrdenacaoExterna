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
    std::cout << "tamanho heap:" << tamanhoHeap << "\n";

    DadosEmprego *d = new DadosEmprego[tamanhoHeap];
    arquivoPrincipal->posicionaInicio();
    arquivoPrincipal->lerRegistro(d, tamanhoHeap);
    heapRegistros = new MinHeap(d, tamanhoHeap);
    delete[] d;

    distribuicaoRegistros();

    std::cout << "main:" << arquivoPrincipal->getNumRegistros() << "\n";
    std::cout << "temp01: " << arquivoTemp_01->getNumRegistros() << "\n";
    std::cout << "temp02: " << arquivoTemp_02->getNumRegistros() << "\n";
    std::cout << "temp03: " << arquivoTemp_03->getNumRegistros() << "\n";
    std::cout << "temp04: " << arquivoTemp_04->getNumRegistros() << "\n";

    long long int diferenca = arquivoPrincipal->getNumRegistros() - arquivoTemp_01->getNumRegistros() - arquivoTemp_02->getNumRegistros();

    std::cout << "diferenca:  " << diferenca << "\n";

    if(arquivoTemp_01->getNumRegistros() && arquivoTemp_02->getNumRegistros())
        mergeSort(arquivoTemp_01, arquivoTemp_02, arquivoTemp_03, arquivoTemp_04);
    else{
        std::cout << "temp01: " << arquivoTemp_01->getNumRegistros() << "\n";
        std::cout << "temp02: " << arquivoTemp_02->getNumRegistros() << "\n";
        std::cout << "temp03: " << arquivoTemp_03->getNumRegistros() << "\n";
        std::cout << "temp04: " << arquivoTemp_04->getNumRegistros() << "\n";
        std::cout << "nem intercalou\n";
    }
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

        arquivoPrincipal->posicionaIndex();

        while(!heapRegistros->vazia()){

            dadoHeapRaiz = heapRegistros->espiaRaiz();

            if(dadoHeapRaiz.pesoValor > pesoSegmento){
                pesoSegmento = dadoHeapRaiz.pesoValor;
                arquivoTempAtual->setNumRegistros(arquivoTempAtual->getNumRegistros() + contadorRegistros);
                std::swap(arquivoTempAtual, arquivoTempSeguinte);
                contadorRegistros = 0;
            }

            // if(arquivoPrincipal->arquivo.read((char *)&dadoEmprego, sizeof(DadosEmprego))){
            if(!arquivoPrincipal->fimLeitura()){
                // ainda tem registros no arquivo principal (remove raiz inserindo novo valor lido)

                arquivoPrincipal->lerRegistro(&dadoEmprego);

                if(dadoEmprego < dadoHeapRaiz.valorDado)
                    dadoHeap = heapRegistros->removeInserindo(DadosHeap(&dadoEmprego, pesoSegmento + 1));

                else
                    dadoHeap = heapRegistros->removeInserindo(DadosHeap(&dadoEmprego, pesoSegmento));

            }
            else
                dadoHeap = heapRegistros->removeRaiz();

            arquivoTempAtual->escreverRegistro(&dadoHeap.valorDado);
            contadorRegistros++;
        }

        arquivoTempAtual->setNumRegistros(arquivoTempAtual->getNumRegistros() + contadorRegistros);

    }catch(std::runtime_error &e){
        std::cout << e.what() << "\n";
    }

}

void OrdenacaoExterna::intercalacaoRegistros(ArquivoBinario *fonteEntrada_01, ArquivoBinario *fonteEntrada_02, ArquivoBinario *fonteSaida){
	
    DadosEmprego *d1{new DadosEmprego}, *d1_anterior{new DadosEmprego}, *d2{new DadosEmprego}, *d2_anterior{new DadosEmprego};
    unsigned int contadorRegistros{0};
	bool fimBloco_01{false}, fimBloco_02{false};

    // primeira leitura de cada bloco
	if(!fonteEntrada_01->fimLeitura())
		fonteEntrada_01->lerRegistro(d1);
    else
        fimBloco_01 = true;

	if(!fonteEntrada_02->fimLeitura())
		fonteEntrada_02->lerRegistro(d2);
    else
        fimBloco_02 = true;

	
	while(!fimBloco_01 && !fimBloco_02){
		
		if(*d1 <= *d2){
			fonteSaida->escreverRegistro(d1);
            contadorRegistros++;

            if(!fonteEntrada_01->fimLeitura()){
                *d1_anterior = *d1;
                fonteEntrada_01->lerRegistro(d1);

                if(*d1 < *d1_anterior){
                    //novo bloco em arquivo 01
                    fonteEntrada_01->desfazerLeitura();
                    fimBloco_01 = true;
                }

            }
            else
                fimBloco_01 = true;
		}
        else{
			
			fonteSaida->escreverRegistro(d2);
            contadorRegistros++;

            if(!fonteEntrada_02->fimLeitura()){
                *d2_anterior = *d2;
                fonteEntrada_02->lerRegistro(d2);

                if(*d2 < *d2_anterior){
                    //novo bloco em arquivo 02
                    fonteEntrada_02->desfazerLeitura();
                    fimBloco_02 = true;
                }

            }
            else
                fimBloco_02 = true;
		}
		
		
	}
	
	while(!fimBloco_01){

        fonteSaida->escreverRegistro(d1);
        contadorRegistros++;

        if (!fonteEntrada_01->fimLeitura()){
            *d1_anterior = *d1;
            fonteEntrada_01->lerRegistro(d1);

            if (*d1 < *d1_anterior){
                // novo bloco em arquivo 01
                fonteEntrada_01->desfazerLeitura();
                fimBloco_01 = true;
            }
        }
        else
            fimBloco_01 = true;
    }
	
	while(!fimBloco_02){

        fonteSaida->escreverRegistro(d2);
        contadorRegistros++;

        if (!fonteEntrada_02->fimLeitura()){
            *d2_anterior = *d2;
            fonteEntrada_02->lerRegistro(d2);

            if (*d2 < *d2_anterior){
                // novo bloco em arquivo 02
                fonteEntrada_02->desfazerLeitura();
                fimBloco_02 = true;
            }
        }
        else
            fimBloco_02 = true;
    }

    fonteSaida->setNumRegistros(fonteSaida->getNumRegistros() + contadorRegistros);

	delete d1;
    delete d1_anterior;
    delete d2;
    delete d2_anterior;

}


void OrdenacaoExterna::mergeSort(ArquivoBinario *fonteEntrada_01, ArquivoBinario *fonteEntrada_02, ArquivoBinario *fonteSaida_01, ArquivoBinario *fonteSaida_02){

    fonteEntrada_01->posicionaInicio(); 
    fonteEntrada_02->posicionaInicio();
    fonteSaida_01->posicionaInicio();
    fonteSaida_02->posicionaInicio();

    while(!fonteEntrada_01->fimLeitura() || !fonteEntrada_02->fimLeitura()){

        intercalacaoRegistros(fonteEntrada_01, fonteEntrada_02, fonteSaida_01);
        std::swap(fonteSaida_01, fonteSaida_02);
    }

    fonteEntrada_01->setNumRegistros(0);
    fonteEntrada_02->setNumRegistros(0);

   if(fonteSaida_01->getNumRegistros() && fonteSaida_02->getNumRegistros())
        mergeSort(fonteSaida_01, fonteSaida_02, fonteEntrada_01, fonteEntrada_02);
    else{
        std::cout << "temp01: " << arquivoTemp_01->getNumRegistros() << "\n";
        std::cout << "temp02: " << arquivoTemp_02->getNumRegistros() << "\n";
        std::cout << "temp03: " << arquivoTemp_03->getNumRegistros() << "\n";
        std::cout << "temp04: " << arquivoTemp_04->getNumRegistros() << "\n";
    }
    
}