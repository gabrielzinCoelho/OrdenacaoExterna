#pragma once
#include "eventosUsuario.hpp"

EventosUsuario::EventosUsuario(){
    gerenciaCsv = new GerenciaCsv(taxaTransferencia);
}

EventosUsuario::~EventosUsuario(){
    delete gerenciaCsv;
}

void EventosUsuario::conversaoBinario(){

    std::string nomeCsv, nomeDat;

    std::cout << "Nome do arquivo csv de entrada: ";
    std::cin >> nomeCsv;
    gerenciaCsv->definirParametrosLeitura(nomeCsv);
    
    std::cout << "Nome do arquivo dat de saida: ";
    std::cin >> nomeDat;
    GerenciaDat *gerenciaDat = new GerenciaDat(nomeDat, taxaTransferencia);

    std::pair<DadosEmprego*, int> d;

    do{
        d = gerenciaCsv->importarCsv(nomeCsv);

        if(d.second)
            gerenciaDat->exportarDat(d);
        delete[] d.first;
    }while(d.second);

    delete gerenciaDat;

    std::cout << nomeDat << " exportado com sucesso.\n";
    confirmacaoUsuario();

}

void EventosUsuario::conversaoCsv(){

    std::string nomeCsv, nomeDat;

    std::cout << "Nome do arquivo dat de entrada: ";
    std::cin >> nomeDat;
    GerenciaDat *gerenciaDat = new GerenciaDat(nomeDat, taxaTransferencia);

    std::cout << "Nome do arquivo csv de saida: ";
    std::cin >> nomeCsv;

    std::pair<DadosEmprego*, int> d;

    do{
        d = gerenciaDat->importarDat();
        if(d.second)
            gerenciaCsv->exportarCsv(nomeCsv, d);
        delete[] d.first;
    }while(d.second);
    

    delete gerenciaDat;

    std::cout << nomeCsv << " exportado com sucesso.\n";
    confirmacaoUsuario();

}