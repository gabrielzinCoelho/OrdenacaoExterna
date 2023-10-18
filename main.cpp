#include <iostream>
#include <utility>
#include "./controllers/gerenciaCsv.hpp"
#include "./models/dadosEmprego.hpp"

int main(){

    GerenciaCsv *gerenciaCsv = new GerenciaCsv("lms-jun22qtr.csv", 1000);

    std::pair<DadosEmprego*, int> d;

    do{
        d = gerenciaCsv->importarCsv("lms-jun22qtr.csv");
        // d = gerenciaCsv->importarCsv("input.csv");
        if(d.second)
            gerenciaCsv->exportarCsv("output.csv", d);
        delete[] d.first;

    }while(d.second);

    return 0;
}