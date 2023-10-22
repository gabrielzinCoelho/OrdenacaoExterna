#include <iostream>
#include <utility>
#include "./controllers/gerenciaCsv.cpp"
#include "./controllers/gerenciaDat.cpp"
#include "./models/dadosEmprego.hpp"
#include "./views/menuNavegacao.hpp"
#include "globals.cpp"

int main(){

    GerenciaCsv *gerenciaCsv = new GerenciaCsv("lms-jun22qtr.csv", taxaTransferencia);
    GerenciaDat *gerenciaDat = new GerenciaDat("output.dat", taxaTransferencia);

    MenuNavegacao menuPrincipal("MENU PRINCIPAL", 0, 6, new std::string[6]{
        "Importar CSV",
        "ExportarCSV",
        "Importar Binario",
        "Exportar Binario",
        "Visualizar dados",
        "Finalizar programa"
    });

    MenuNavegacao menuVisualizacao("VISUALIZAR DADOS", 10, 3, new std::string[3]{
        "Visualizar registros entre posições X e Y",
        "Visualizar todos os registros",
        "Menu anterior (principal)",
    });

    int escolhaMenu;
    MenuNavegacao *menuEscolhido{&menuPrincipal};

    do{
        system ("clear || cls");
        escolhaMenu = menuEscolhido->mostraMenu();

        switch (escolhaMenu)
        {
        case 1:
            std::cout << "Importando CSV...\n";
            confirmacaoUsuario();
            break;
        case 2:
            std::cout << "Exportando CSV...\n";
            confirmacaoUsuario();
            break;
        case 3:
            std::cout << "Importando Binario...\n";
            confirmacaoUsuario();
            break;
        case 4:
            std::cout << "Exportando Binario...\n";
            confirmacaoUsuario();
            break;
        case 5:
            menuEscolhido = &menuVisualizacao;
            break;
        case 11:
            std::cout << "Visualizando registros...\n";
            confirmacaoUsuario();
            break;
        case 12:
            std::cout << "Visualizando registros entre X e Y...\n";
            confirmacaoUsuario();
            break;
        case 13:
            menuEscolhido = &menuPrincipal;
            break;
        default:
            break;
        }
    }while(escolhaMenu != 6);
    

    // std::pair<DadosEmprego*, int> d;

    // do{
    //     d = gerenciaCsv->importarCsv("lms-jun22qtr.csv");

    //     if(d.second)
    //         gerenciaDat->exportarDat(d);
    //     delete[] d.first;
    // }while(d.second);

    // do{
    //     d = gerenciaDat->importarDat();
    //     if(d.second)
    //         gerenciaCsv->exportarCsv("output.csv", d);
    //     delete[] d.first;
    // }while(d.second);



    return 0;
}