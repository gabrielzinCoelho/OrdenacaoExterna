#pragma once
#include "dadosEmprego.hpp"

DadosEmprego::DadosEmprego(){}

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

void DadosEmprego::impressaoResumida(){

    std::cout << seriesId << ": " << periodo << " * " << valor << " * " << descricao << "\n";
}

void DadosEmprego::impressaoDetalhada(){

    std::cout << "Series reference: " << seriesId << "\n"
        << "Period: " << periodo << "\n"
        << "Data value: " << valor << "\n"
        << "Status: " << status << "\n"
        << "Units: " << unidadeMedidaValor << "\n"
        << "Magntude: " << magnitudeValor << "\n"
        << "Subject: " << descricao << "\n"
        << "Group: " << categoria << "\n"
        << "Series title (1): " << tituloSerie_1 << "\n"
        << "Series title (2): " << tituloSerie_2 << "\n"
        << "Series title (3): " << tituloSerie_3 << "\n"
        << "Series title (4): " << tituloSerie_4 << "\n"
        << "Series title (5): " << tituloSerie_5 << "\n";

}

bool DadosEmprego::operator<(const DadosEmprego &d){

    int comparacaoSeriesId = std::strcmp(this->seriesId, d.seriesId);

    if(comparacaoSeriesId < 0)
        return true;
    
    if(comparacaoSeriesId > 0)
        return false;
    
    if(std::strcmp(this->periodo, d.periodo) <= 0)
        return true;
    
    return false;
    
}

bool DadosEmprego::operator>(const DadosEmprego &d){

    int comparacaoSeriesId = std::strcmp(this->seriesId, d.seriesId);

    if(comparacaoSeriesId > 0)
        return true;
    
    if(comparacaoSeriesId < 0)
        return false;
    
    if(std::strcmp(this->periodo, d.periodo) >= 0)
        return true;
    
    return false;

}

bool DadosEmprego::operator==(const DadosEmprego &d){

    return !(std::strcmp(this->seriesId, d.seriesId)) && !(std::strcmp(this->periodo, d.periodo));

}

void DadosEmprego::apagar(){
    seriesId[0] = '-';
    seriesId[1] = '1';
    seriesId[2] = '\0';
}

bool DadosEmprego::ehNulo(){
    return seriesId[0] == '-' && seriesId[1] == '1' && seriesId[2] == '\0';
}