#pragma once
#include <iostream>
#include <cstring>

class DadosEmprego{
    friend class GerenciaCsv;
    private:
        char seriesId[20], periodo[10], status[20], unidadeMedidaValor[20], descricao[200], categoria[200], valor[20];
        char tituloSerie_1[200], tituloSerie_2[200], tituloSerie_3[200], tituloSerie_4[200], tituloSerie_5[200];
        int magnitudeValor;
    public:
        DadosEmprego();
        DadosEmprego(std::string *camposRegistro);
        std::string getSeriesId();
        std::string getPeriodo();
        void impressaoResumida();
        void impressaoDetalhada();
        bool operator<(const DadosEmprego &d);
        bool operator>(const DadosEmprego &d);
        bool operator==(const DadosEmprego &d);
        void apagar();
        bool ehNulo();
};