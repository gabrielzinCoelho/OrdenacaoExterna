#pragma once
#include "eventosUsuario.hpp"
#include "./ordenacaoExterna.cpp"

EventosUsuario::EventosUsuario(){
    gerenciaCsv = new GerenciaCsv(taxaTransferencia);
    gerenciaDat = nullptr;
}

EventosUsuario::~EventosUsuario(){
    delete gerenciaCsv;
    delete gerenciaDat;
}

void EventosUsuario::defineDat(){

    std::string nomeDat;
    std::cout << "Nome do arquivo dat: ";
    std::cin >> nomeDat;
    gerenciaDat = new GerenciaDat(nomeDat, taxaTransferencia);
}

void EventosUsuario::resetaDat(){
    if (gerenciaDat)
        delete gerenciaDat;
    gerenciaDat = nullptr;
}

void EventosUsuario::verificaDat(){
    if (!gerenciaCsv)
        throw "Nenhum arquivo dat definido.";
}

void EventosUsuario::conversaoBinario(){

    try{
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

void EventosUsuario::conversaoCsv(){

    try{
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

        //ordenar arquivo exportado
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

    }catch(std::string err){
        std::cout << "Erro na impressao dos registros.\n";
    }
    confirmacaoUsuario();
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

    }catch(std::string err){
        std::cout << "Erro na impressao do registro.\n";
    }
    confirmacaoUsuario();
}

void EventosUsuario::inserirRegistro(){

    try{

        std::cout << "Informe a posicao em que o registro será inserido: \n";

        long int x;

        std::cout << "X: ";
        std::cin >> x;

        std::string *camposRegistro = new std::string[numCamposRegistro];

        std::cout << "Preencha os campos do novo registro: \n\n";

        std::cin.ignore(1, '\n');

        for(int i{0}; i<numCamposRegistro; i++){
            std::cout << camposRegistroRotulo[i];
            std::getline(std::cin, camposRegistro[i]);
        }

        std::cout << "\nAdicionando registro...\n";


        gerenciaDat->inserirRegistro(x, camposRegistro);
        delete[] camposRegistro;

    }catch(std::string err){
        std::cout << err << "\n";
    }
    confirmacaoUsuario();
}

void EventosUsuario::trocarRegistros(){

    try{

        std::cout << "Trocando de posição os registros X e Y: \n";

        long int x, y;

        std::cout << "X: ";
        std::cin >> x;

        std::cout << "Y: ";
        std::cin >> y;

        gerenciaDat->trocarRegistros(x, y);
        std::cout << "\nTroca realizada com sucesso.\n";

    }catch(std::string err){
        std::cout << "Erro na troca dos registros.\n";
    }
    confirmacaoUsuario();
}

void EventosUsuario::editarRegistro(){

    try{

        std::cout << "Informe a posicao do registro: \n";

        long int x;

        std::cout << "X: ";
        std::cin >> x;

        std::cout << "\nAtualize os campos do novo registro: \n\n";

        std::string *camposRegistro = new std::string[numCamposRegistro];
        std::cin.ignore(1, '\n');
        for(int i{0}; i<numCamposRegistro; i++){
            std::cout << camposRegistroRotulo[i];
            std::getline(std::cin, camposRegistro[i]);
        }
        std::cout << "\n\n";

        gerenciaDat->editarRegistro(x, camposRegistro);

        std::cout << "\nEdicao realizada com sucesso.\n";
        delete[] camposRegistro;
    }catch(std::string err){
        std::cout << "Erro na edicao do registro.\n";
    }
    confirmacaoUsuario();
}

void EventosUsuario::deletarRegistro(){


    try{

        std::cout << "Informe a posicao do registro: \n";

        long int x;

        std::cout << "X: ";
        std::cin >> x;

        gerenciaDat->deletarRegistro(x);

        std::cout << "\nDelecao realizada com sucesso.\n";

    }catch(std::string err){
        std::cout << "Erro na edicao do registro.\n";
    }
    confirmacaoUsuario();

}

void EventosUsuario::ordenarArquivo(){

    std::string nomeDat;

    std::cout << "Nome do arquivo binario: ";
    std::cin >> nomeDat;

    OrdenacaoExterna *instanciaOrdenacao = new OrdenacaoExterna(nomeDat, 0.01); //10MB
    delete instanciaOrdenacao;

    std::cout << "\nOrdenacao realizada com sucesso.\n";

    confirmacaoUsuario();

}