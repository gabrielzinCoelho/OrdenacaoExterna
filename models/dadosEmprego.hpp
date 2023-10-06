class DadosEmprego{
    private:
        char seriesId[20], periodo[7], status[10], unidadeMedidaValor[10], descricao[50], categoria[100];
        char tituloSerie_1[100], tituloSerie_2[100], tituloSerie_3[100], tituloSerie_4[100], tituloSerie_5[100];
        float value;
        int magnitudeValor;
    public:
        DadosEmprego();
        ~DadosEmprego();
};

DadosEmprego::DadosEmprego(){

}

DadosEmprego::~DadosEmprego(){
    
}