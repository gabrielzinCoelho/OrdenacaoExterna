#include <iostream>
#include <utility>
#include "./controllers/gerenciaCsv.cpp"
#include "./controllers/gerenciaDat.cpp"
#include "./controllers/eventosUsuario.cpp"
#include "./models/dadosEmprego.cpp"
#include "./views/menuNavegacao.cpp"
#include "globals.cpp"

// Gabriel Coelho Costa
// Isac Gonçalves Cunha
// Mateus Milani Rodrigues
//10A

int main(){


    MenuNavegacao menuPrincipal("MENU PRINCIPAL", 0, 3, new std::string[3]{
        "Importar/Exportar arquivos",
        "Manipular registros",
        "Finalizar programa"
    });

    MenuNavegacao menuArquivos("ARQUIVOS", 10, 3, new std::string[3]{
        "Conversão CSV -> Binario",
        "Conversão Binario -> CSV",
        "Menu anterior (principal)",
    });

    MenuNavegacao menuRegistros("REGISTROS", 20, 6, new std::string[6]{
        "Visualizar registros",
        "Inserir registro",
        "Deletar registro",
        "Editar registro",
        "Inverter posicao entre registros",
        "Menu anterior (principal)",
    });

    MenuNavegacao menuVisualizacao("VISUALIZAR REGISTROS", 30, 4, new std::string[4]{
        "Visualizar registros entre posições X e Y",
        "Visualizar todos os registros",
        "Visualizar com detalhes",
        "Menu anterior (registros)",
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
            menuEscolhido = &menuArquivos;
            break;
        case 2:
            eventosUsuario->defineDat();
            menuEscolhido = &menuRegistros;
            break;
        case 11:
            eventosUsuario->conversaoBinario();
            break;
        case 12:
            eventosUsuario->conversaoCsv();
            break;
        case 13:
            menuEscolhido = &menuPrincipal;
            break;
        case 21:
            menuEscolhido = &menuVisualizacao;
            break;
        case 22:
            eventosUsuario->inserirRegistro();
            break;
        case 23:
            eventosUsuario->deletarRegistro();
            break;
        case 24:
            eventosUsuario->editarRegistro();
            break;
        case 25:
            eventosUsuario->trocarRegistros();
            break;
        case 26:
            eventosUsuario->resetaDat();
            menuEscolhido = &menuPrincipal;
            break;
        case 31:
            eventosUsuario->impressaoSecao();
            break;
        case 32:
            eventosUsuario->impressaoGeral();
            break;
        case 33:
            eventosUsuario->impressaoDetalhada();
            break;
        case 34:
            menuEscolhido = &menuRegistros;
        default:
            break;
        }
    }while(escolhaMenu != 3);

    delete[] camposRegistroRotulo;

    return 0;
}