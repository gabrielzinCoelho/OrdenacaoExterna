#pragma once
#include "eventosUsuario.hpp"

EventosUsuario::EventosUsuario()
{
    gerenciaCsv = new GerenciaCsv(taxaTransferencia);
    gerenciaDat = nullptr;
}

EventosUsuario::~EventosUsuario()
{
    delete gerenciaCsv;
    delete gerenciaDat;
}

void EventosUsuario::defineDat()
{

    std::string nomeDat;
    std::cout << "Nome do arquivo dat: ";
    std::cin >> nomeDat;
    gerenciaDat = new GerenciaDat(nomeDat, taxaTransferencia);
}

void EventosUsuario::resetaDat()
{
    if (gerenciaDat)
        delete gerenciaDat;
    gerenciaDat = nullptr;
}

void EventosUsuario::verificaDat()
{
    if (!gerenciaCsv)
        throw "Nenhum arquivo dat definido.";
}

void EventosUsuario::conversaoBinario()
{

    try
    {
        std::string nomeCsv, nomeDat;

        std::cout << "Nome do arquivo csv de entrada: ";
        std::cin >> nomeCsv;
        gerenciaCsv->definirParametrosLeitura(nomeCsv);

        std::cout << "Nome do arquivo dat de saida: ";
        std::cin >> nomeDat;
        GerenciaDat *gerenciaDat = new GerenciaDat(nomeDat, taxaTransferencia);

        std::pair<DadosEmprego *, int> d;

        do
        {
            d = gerenciaCsv->importarCsv(nomeCsv);

            if (d.second)
                gerenciaDat->exportarDat(d);
            delete[] d.first;
        } while (d.second);

        delete gerenciaDat;
        std::cout << nomeDat << " exportado com sucesso.\n";
    }
    catch (std::string err)
    {
        std::cout << "Falha na importacao: " << err << "\n";
    }

    confirmacaoUsuario();
}

void EventosUsuario::conversaoCsv()
{

    try
    {
        std::string nomeCsv, nomeDat;

        std::cout << "Nome do arquivo dat de entrada: ";
        std::cin >> nomeDat;
        GerenciaDat *gerenciaDat = new GerenciaDat(nomeDat, taxaTransferencia);

        if (!gerenciaDat->getNumRegistros())
            throw nomeDat + " não foi inicializado corretamente.";

        std::cout << "Nome do arquivo csv de saida: ";
        std::cin >> nomeCsv;

        std::pair<DadosEmprego *, int> d;

        do
        {
            d = gerenciaDat->importarDat();
            if (d.second)
                gerenciaCsv->exportarCsv(nomeCsv, d);
            delete[] d.first;
        } while (d.second);

        delete gerenciaDat;

        std::cout << nomeCsv << " exportado com sucesso.\n";
    }
    catch (std::string err){
        std::cout << "Falha na importacao: " << err << "\n";
    }
    confirmacaoUsuario();
}

void EventosUsuario::impressaoSecao(){

    try{

        std::cout << "Visualizando registros entre X e Y: \n";
        std::cout << "( X > Y para impressoes em ordem decrescente )\n\n";

        long int x, y;

        std::cout << "X: ";
        std::cin >> x;

        std::cout << "Y: ";
        std::cin >> y;

        gerenciaDat->imprimirRegistros(false, x, y);
        std::cout << "\n";
        confirmacaoUsuario();

    }catch(std::string err){
        std::cout << "Erro na impressao dos registros.\n";
    }
}

void EventosUsuario::impressaoGeral(){

    gerenciaDat->imprimirRegistros(true);
    std::cout << "\n";
    confirmacaoUsuario();

}

void EventosUsuario::impressaoDetalhada(){

    try{

        std::cout << "Informe a posicao do registro: \n";

        long int x;

        std::cout << "X: ";
        std::cin >> x;

        gerenciaDat->imprimirRegistros(false, x, x, true);
        std::cout << "\n";
        confirmacaoUsuario();

    }catch(std::string err){
        std::cout << "Erro na impressao do registro.\n";
    }

}

void EventosUsuario::inserirRegistro(){

    std::string *camposRegistro = new std::string[numCamposRegistro];

    std::cout << "Preencha os campos do novo registro: \n\n";

    std::cin.ignore(1, '\n');

    for(int i{0}; i<numCamposRegistro; i++){
        std::cout << camposRegistroRotulo[i];
        std::getline(std::cin, camposRegistro[i]);
    }

    std::cout << "\n\n";

    gerenciaDat->inserirRegistro(camposRegistro);
    confirmacaoUsuario();

    delete[] camposRegistro;

}