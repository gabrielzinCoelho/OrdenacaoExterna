#include <iostream>
#include <utility>
#include "./controllers/gerenciaCsv.cpp"
#include "./controllers/gerenciaDat.cpp"
#include "./controllers/eventosUsuario.cpp"
#include "./models/dadosEmprego.cpp"
#include "./views/menuNavegacao.cpp"
#include "globals.cpp"

int main(){


    MenuNavegacao menuPrincipal("MENU PRINCIPAL", 0, 4, new std::string[4]{
        "Conversão CSV -> Binario",
        "Conversão Binario -> CSV",
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

    EventosUsuario *eventosUsuario = new EventosUsuario();

    do{
        system ("clear || cls");
        escolhaMenu = menuEscolhido->mostraMenu();

        switch (escolhaMenu)
        {
        case 1:
            eventosUsuario->conversaoBinario();
            break;
        case 2:
            eventosUsuario->conversaoCsv();
            break;
        case 3:
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
    }while(escolhaMenu != 4);

    return 0;
}