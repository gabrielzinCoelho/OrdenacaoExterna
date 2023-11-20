#pragma once
#include <iostream>
#include <cstring>

class DadosEmprego{
    friend class GerenciaCsv;
    private:
        char seriesId[20], periodo[8], status[10], unidadeMedidaValor[10], descricao[50], categoria[100], valor[10];
        char tituloSerie_1[100], tituloSerie_2[100], tituloSerie_3[100], tituloSerie_4[100], tituloSerie_5[100];
        int magnitudeValor;
    public:
        DadosEmprego();
        DadosEmprego(std::string *camposRegistro);
        std::string getSeriesId();
        void impressaoResumida();
        void impressaoDetalhada();
        bool operator<(const DadosEmprego &d);
        bool operator>(const DadosEmprego &d);
        bool operator==(const DadosEmprego &d);
        void apagar();
        bool ehNulo();
};