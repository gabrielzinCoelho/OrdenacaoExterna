#pragma once
#include "gerenciaCsv.hpp"

GerenciaCsv::GerenciaCsv(int taxaTransferencia = 1000) : taxaTransferencia(taxaTransferencia) {}

GerenciaCsv::GerenciaCsv(const std::string &nomeArq, int taxaTransferencia = 1000) : taxaTransferencia(taxaTransferencia){
    definirParametrosLeitura(nomeArq);
}

void GerenciaCsv::definirParametrosLeitura(const std::string &nomeArq){

    if (!abrirCsv(nomeArq, std::ios::in))
        throw nomeArq + " não foi inicializado corretamente.";

    std::string cabecalhoCsv;
    std::getline(arquivoCsv, cabecalhoCsv);
    indexLeitura = int(arquivoCsv.tellp());

    fecharCsv();
}

bool GerenciaCsv::abrirCsv(const std::string &nomeArq, std::ios_base::openmode modoAbertura = std::ios::in){
    try{
        arquivoCsv.open(caminhoPastaArquivos + nomeArq, modoAbertura);
        if(!arquivoCsv.good())
            throw "Erro na abertura: " + nomeArq + "\n";
        return true;
    }
    catch (std::string err){
        std::cout << err;
        arquivoCsv.clear();
        return false;
    }
}

void GerenciaCsv::fecharCsv(){
    arquivoCsv.close();
}

std::pair<DadosEmprego*, int> GerenciaCsv::importarCsv(const std::string &nomeArq){

    abrirCsv(nomeArq, std::ios::in);

    arquivoCsv.seekp(indexLeitura, std::ios::beg);

    DadosEmprego *dadosEmprego = new DadosEmprego[taxaTransferencia];

    int indexCampoRegistro, indexSubstr, contadorRegistros{0};
    std::string camposRegistro[numCamposRegistro], linhaRegistro;
    bool escapeCaractere;

    try{
        while(!arquivoCsv.eof() && contadorRegistros < taxaTransferencia){

            // pegar cada linha do csv e salvar em uma string
            if(!std::getline(arquivoCsv, linhaRegistro))
                throw std::string("Getline Exception.");

            indexSubstr = indexCampoRegistro = 0;
            escapeCaractere = false;
            // utiliza a variavel indexSubstr para indexar cada campo do registro no csv


            //percorre cada caractere da linha procurando delimitadores
            for(int i{0}; i < int(linhaRegistro.size()); i++){
                
                // se encontrou aspas, inverte o booleano para controle das virgulas
                if(linhaRegistro[i] == '"'){
                    escapeCaractere = !escapeCaractere;
                }

                // caractere de fim de linha para pegar o ultimo campo
                else if((linhaRegistro[i] == separador && !escapeCaractere) || linhaRegistro[i] == '\n' || linhaRegistro[i] == '\r'){

                    // se o texto tiver aspas remover o primeiro e ultimo caractere da linha
                    if(linhaRegistro[indexSubstr] == '"')
                        camposRegistro[indexCampoRegistro] = linhaRegistro.substr(indexSubstr + 1, i - (indexSubstr + 2));
                    else
                        camposRegistro[indexCampoRegistro] = linhaRegistro.substr(indexSubstr, i - indexSubstr);

                    indexCampoRegistro++;
                    indexSubstr = i + 1;

                }
            }
            dadosEmprego[contadorRegistros++] = DadosEmprego(camposRegistro);
        }

    }catch(std::string err){
        std::cout << err << "\n";
    }

    indexLeitura = arquivoCsv.tellp();
    fecharCsv();

    return std::make_pair(dadosEmprego, contadorRegistros);
}

void GerenciaCsv::exportarCsv(const std::string &nomeArq, std::pair<DadosEmprego*, int> dadosExportados){
    
    if(!abrirCsv(nomeArq, std::ios::in|std::ios::out)){
        std::cout << "Criando csv de saída...\n";
        abrirCsv(nomeArq, std::ios::out);
        fecharCsv();
        abrirCsv(nomeArq, std::ios::in|std::ios::out);
    }
    arquivoCsv.seekg(0, std::ios::end);

    for(int i{0}; i<dadosExportados.second; i++){

        arquivoCsv << tratamentoEscape(dadosExportados.first[i].seriesId) << separador
        << tratamentoEscape(dadosExportados.first[i].periodo) << separador
        << tratamentoEscape(dadosExportados.first[i].valor) << separador
        << tratamentoEscape(dadosExportados.first[i].status) << separador
        << tratamentoEscape(dadosExportados.first[i].unidadeMedidaValor) << separador
        << dadosExportados.first[i].magnitudeValor << separador
        << tratamentoEscape(dadosExportados.first[i].descricao) << separador
        << tratamentoEscape(dadosExportados.first[i].categoria) << separador
        << tratamentoEscape(dadosExportados.first[i].tituloSerie_1) << separador
        << tratamentoEscape(dadosExportados.first[i].tituloSerie_2) << separador
        << tratamentoEscape(dadosExportados.first[i].tituloSerie_3) << separador
        << tratamentoEscape(dadosExportados.first[i].tituloSerie_4) << separador
        << tratamentoEscape(dadosExportados.first[i].tituloSerie_5) << "\n";
    }
    fecharCsv();
}

std::string GerenciaCsv::tratamentoEscape(const std::string &str){

    for(int i{0}; i<int(str.size()); i++)
        if(str[i] == separador)
            return '"' + str + '"'; 
    return str;
}