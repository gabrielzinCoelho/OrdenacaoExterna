#pragma once
#include <stdexcept>
#include "./ordenacaoExterna.hpp"
#include "../components/minHeap.cpp"
#include "../components/arquivoBinario.cpp"

OrdenacaoExterna::OrdenacaoExterna(const std::string &nomeArq, float gB){

    const unsigned int gBParaByte = 1000000000; // 1 x 10^9
    unsigned int tamanhoMaxHeap = gB * gBParaByte/sizeof(DadosHeap), tamanhoHeap;

    arquivoPrincipal = new ArquivoBinario(nomeArq, std::ios::in);

    // arquivos temporários para ordenação externa

    arquivoTemp_01 = new ArquivoBinario("temp01.dat");
    arquivoTemp_02 = new ArquivoBinario("temp02.dat");
    arquivoTemp_03 = new ArquivoBinario("temp03.dat");
    arquivoTemp_04 = new ArquivoBinario("temp04.dat");

    // calcula o tamanho da heap: se há registros suficientes, o tamanho será igual ao definido pelo parâmetro gB recebido no construtor da classe
    tamanhoHeap = arquivoPrincipal->getNumRegistros() <= tamanhoMaxHeap ? arquivoPrincipal->getNumRegistros() : tamanhoMaxHeap;

    // lê os primeiros registros do arquivo, o suficiente para povoar o min heap de acordo com o tamanho definido

    DadosEmprego *d = new DadosEmprego[tamanhoHeap];
    arquivoPrincipal->posicionaInicio();
    arquivoPrincipal->lerRegistro(d, tamanhoHeap);
    heapRegistros = new MinHeap(d, tamanhoHeap);
    delete[] d;

    // dividir o arquivo em blocos ordenados utilizando uma espécie de heap sort
    distribuicaoRegistros();

    //se um dos arquivos já se encontra sem registros, a fase de intercalação dos blocos não é necessária
    if(arquivoTemp_01->getNumRegistros() && arquivoTemp_02->getNumRegistros())
        mergeSort(arquivoTemp_01, arquivoTemp_02, arquivoTemp_03, arquivoTemp_04);
   
}

OrdenacaoExterna::~OrdenacaoExterna(){

    // remove o arquivo de entrada (que foi ordenado) e o substitui pelo arquivo resultante da ordenação
    arquivoPrincipal->close();
    remove((caminhoPastaArquivos + arquivoPrincipal->getNome()).c_str());

    ArquivoBinario* listaArquivos[4] = {
        arquivoTemp_01,
        arquivoTemp_02,
        arquivoTemp_03,
        arquivoTemp_04
    };

    // remove os arquivos temporários, à exceção do arquivo que contem numero de registros diferente de 0 (produto da ordenação)
    // arquivo ordenado é renomeado para substituir o arquivo de entrada
    for(int i{0}; i<4; i++){

        listaArquivos[i]->close();

        if(!listaArquivos[i]->getNumRegistros())
            remove((caminhoPastaArquivos + listaArquivos[i]->getNome()).c_str());
        else
            rename((caminhoPastaArquivos + listaArquivos[i]->getNome()).c_str(), (caminhoPastaArquivos + arquivoPrincipal->getNome()).c_str());
    }

    delete arquivoPrincipal;
    delete arquivoTemp_01;
    delete arquivoTemp_02;
    delete arquivoTemp_03;
    delete arquivoTemp_04;

}

void OrdenacaoExterna::distribuicaoRegistros(){

    try{

        int pesoSegmento{0};
        ArquivoBinario *arquivoTempAtual{arquivoTemp_01}, *arquivoTempSeguinte{arquivoTemp_02};

        DadosHeap dadoHeap, dadoHeapRaiz;
        DadosEmprego dadoEmprego;
        unsigned int contadorRegistros{0};

        // garante a posição correta do ponteiro de leitura no arquivo
        arquivoPrincipal->posicionaIndex();

        // enquanto a heap possui registros o heapSort continua em execução
        while(!heapRegistros->vazia()){


            // espia a raiz para determinar o valor do campo pesoSegmento do registro que sera inserido
            // e se o bloco atual deve ser encerrado
            dadoHeapRaiz = heapRegistros->espiaRaiz();

            // caso o atributo pesoSegmento seja inferior ao pesoSegmento da raiz, o bloco atual deve ser encerrado
            // (significa que os valores cuja inserção foram anteriormente adiadas chegaram ao topo da arvore)

            if(dadoHeapRaiz.pesoValor > pesoSegmento){
                pesoSegmento = dadoHeapRaiz.pesoValor; // pesoSegmento é atualizado
                arquivoTempAtual->setNumRegistros(arquivoTempAtual->getNumRegistros() + contadorRegistros);
                std::swap(arquivoTempAtual, arquivoTempSeguinte); // novo bloco é criado no outro arquivo
                contadorRegistros = 0;
            }


            if(!arquivoPrincipal->fimLeitura()){
                // ainda tem registros no arquivo principal (remove raiz inserindo novo valor lido)

                arquivoPrincipal->lerRegistro(&dadoEmprego);

                // calcula pesoSegmento do novo dado inserido
                // (se o novo valor for quebrar a ordem crescente, seu peso é acrescido)
                if(dadoEmprego < dadoHeapRaiz.valorDado)
                    dadoHeap = heapRegistros->removeInserindo(DadosHeap(&dadoEmprego, pesoSegmento + 1));

                else
                    dadoHeap = heapRegistros->removeInserindo(DadosHeap(&dadoEmprego, pesoSegmento));

            }
            else
                dadoHeap = heapRegistros->removeRaiz();

            // escreve o registro no arquivo atual
            arquivoTempAtual->escreverRegistro(&dadoHeap.valorDado);
            contadorRegistros++;
        }

        // atualiza o numero de registros do arquivo atual
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

    
    // enquanto nenhum dos blocos chegou ao fim, o menor dos registros entre os dois blocos é lido

	while(!fimBloco_01 && !fimBloco_02){
		
        // bloco 1 possui o menor registro

		if(*d1 <= *d2){

            // leitura do registro do bloco 1

			fonteSaida->escreverRegistro(d1);
            contadorRegistros++;

            // os blocos de cada arquivo possuem uma divisão lógica (segmentos de tamanho variável)

            if(!fonteEntrada_01->fimLeitura()){
                *d1_anterior = *d1;
                fonteEntrada_01->lerRegistro(d1);

                // compara o novo registro lido com o anterior, se for menor o bloco chega ao fim
                // e a leitura adicional é desfeita (ponteiro arquivo volta 1 casa)

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
			
            // bloco 2 possui o menor registro

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

    // atualiza o numero de registros
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

    // enquanto algum dos arquivos ainda possui blocos a serem intercalados
    while(!fonteEntrada_01->fimLeitura() || !fonteEntrada_02->fimLeitura()){

        // a cada intercalação o arquivo que recebera o novo bloco é atualizado
        intercalacaoRegistros(fonteEntrada_01, fonteEntrada_02, fonteSaida_01);
        std::swap(fonteSaida_01, fonteSaida_02); 
    }

    // atualiza o numero de registros dos arquivos de entrada
    fonteEntrada_01->setNumRegistros(0);
    fonteEntrada_02->setNumRegistros(0);

    // até que apenas um dos arquivos concentre todos os registros (ordenados) o metodo é chamado recursivamente
    // arquivos de entrada se tornam arquivos de saida e vice versa
    if(fonteSaida_01->getNumRegistros() && fonteSaida_02->getNumRegistros())
        mergeSort(fonteSaida_01, fonteSaida_02, fonteEntrada_01, fonteEntrada_02);
    
    
}