#include <iostream>
#include <utility>
#include "./controllers/gerenciaCsv.hpp"
#include "./controllers/gerenciaDat.hpp"
#include "./models/dadosEmprego.hpp"

int main(){

    const int taxaTransferencia{1000};

    std::cout << "*** " << sizeof(DadosEmprego) << "\n";

    GerenciaCsv *gerenciaCsv = new GerenciaCsv("lms-jun22qtr.csv", taxaTransferencia);
    GerenciaDat *gerenciaDat = new GerenciaDat("output.dat", taxaTransferencia);

    std::pair<DadosEmprego*, int> d;

    do{
        d = gerenciaCsv->importarCsv("lms-jun22qtr.csv");
        // d = gerenciaCsv->importarCsv("input.csv");
        if(d.second)
            gerenciaDat->exportarDat(d);
        delete[] d.first;
    }while(d.second);

    do{
        d = gerenciaDat->importarDat();
        if(d.second)
            gerenciaCsv->exportarCsv("output.csv", d);
        delete[] d.first;
    }while(d.second);

    return 0;
}