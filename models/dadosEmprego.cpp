#pragma once
#include "dadosEmprego.hpp"

DadosEmprego::DadosEmprego(std::string *camposRegistro){

    magnitudeValor = std::stoi(camposRegistro[5]);

    std::strcpy(seriesId, camposRegistro[0].c_str());
    std::strcpy(periodo, camposRegistro[1].c_str());
    std::strcpy(valor, camposRegistro[2].c_str());
    std::strcpy(status, camposRegistro[3].c_str());
    std::strcpy(unidadeMedidaValor, camposRegistro[4].c_str());
    std::strcpy(descricao, camposRegistro[6].c_str());
    std::strcpy(categoria, camposRegistro[7].c_str());
    std::strcpy(tituloSerie_1, camposRegistro[8].c_str());
    std::strcpy(tituloSerie_2, camposRegistro[9].c_str());
    std::strcpy(tituloSerie_3, camposRegistro[10].c_str());
    std::strcpy(tituloSerie_4, camposRegistro[11].c_str());
    std::strcpy(tituloSerie_5, camposRegistro[12].c_str());
}

std::string DadosEmprego::getSeriesId(){
    return std::string(seriesId);
}