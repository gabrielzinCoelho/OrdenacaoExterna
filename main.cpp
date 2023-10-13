#include <iostream>
#include <utility>
#include "./controllers/gerenciaCsv.hpp"
#include "./models/dadosEmprego.hpp"

int main(){

    GerenciaCsv gerenciaCsv("lms-jun22qtr.csv");

    std::pair<DadosEmprego*, int> d = gerenciaCsv.importarCsv();
    std::cout << d.second << " - " << d.first[999].getSeriesId() << "\n";

    return 0;
}