#pragma once
#include "gerenciaDat.hpp"

void GerenciaDat::definirParametrosLeitura(int indexLeitura = -1)
{
    if (indexLeitura < tamanhoCabecalho)
        this->indexLeitura = tamanhoCabecalho;
    else
        this->indexLeitura = indexLeitura;
}

GerenciaDat::GerenciaDat(const std::string &nomeArq, int taxaTransferencia = 1000)
    : nomeArq(nomeArq), taxaTransferencia(taxaTransferencia), tamanhoCabecalho(sizeof(unsigned int))
{

    if (!abrirDat(std::ios::in | std::ios::out))
    {
        std::cout << "Criando dat de saída...\n";
        numRegistros = 0;
        abrirDat(std::ios::out);
        atualizarCabecalho();
    }
    else
    {
        leituraCabecalho();
    }
    definirParametrosLeitura();
    fecharDat();
}

bool GerenciaDat::abrirDat(std::ios_base::openmode modoAbertura = std::ios::in)
{
    try
    {
        arquivoDat.open(caminhoPastaArquivos + nomeArq, modoAbertura);

        if (!arquivoDat.good())
            throw "Erro na abertura: " + nomeArq + "\n";

        return true;
    }
    catch (std::string err)
    {
        std::cout << err;
        arquivoDat.clear();
        return false;
    }
}

void GerenciaDat::fecharDat()
{
    arquivoDat.close();
}
void GerenciaDat::atualizarCabecalho()
{

    if (!arquivoDat.good())
        abrirDat(std::ios::in | std::ios::out);

    arquivoDat.seekp(0);
    arquivoDat.write((char *)&numRegistros, sizeof(int));
}

void GerenciaDat::leituraCabecalho()
{
    if (!arquivoDat.good())
        abrirDat(std::ios::in | std::ios::out);
    arquivoDat.seekp(0);
    arquivoDat.read((char *)&numRegistros, sizeof(int));
}

std::pair<DadosEmprego *, int> GerenciaDat::importarDat()
{

    if (!abrirDat(std::ios::in))
        throw nomeArq + " não foi inicializado corretamente.";

    int dadosRestantes = numRegistros - ((indexLeitura - tamanhoCabecalho) / sizeof(DadosEmprego));
    int tamanhoLeitura = dadosRestantes > taxaTransferencia ? taxaTransferencia : dadosRestantes;

    if (!tamanhoLeitura)
        return std::make_pair(nullptr, 0);
    
    arquivoDat.seekp(indexLeitura, std::ios::beg);

    DadosEmprego *dadosEmprego = new DadosEmprego[tamanhoLeitura];
    arquivoDat.read((char *)dadosEmprego, sizeof(DadosEmprego) * tamanhoLeitura);

    // indexLeitura = arquivoDat.tellp();
    indexLeitura += sizeof(DadosEmprego) * tamanhoLeitura;

    fecharDat();

    return std::make_pair(dadosEmprego, tamanhoLeitura);
}

void GerenciaDat::exportarDat(std::pair<DadosEmprego *, int> dadosExportados)
{

    abrirDat(std::ios::in | std::ios::out);

    arquivoDat.seekp(0, std::ios::end);
    arquivoDat.write((char *)dadosExportados.first, (sizeof(DadosEmprego) * dadosExportados.second));

    numRegistros += dadosExportados.second;
    atualizarCabecalho();

    fecharDat();
}

unsigned int GerenciaDat::getNumRegistros()
{
    return numRegistros;
}

void GerenciaDat::imprimirRegistros(bool imprimirTudo, long int posInicial, long int posFinal, bool impressaoDetalhada)
{

    try
    {

        if (!abrirDat(std::ios::in))
            throw std::string(nomeArq + " não foi inicializado corretamente.");

        if (!numRegistros)
            throw std::string("Não há nada para visualizar...");

        if (imprimirTudo)
        {
            posInicial = 0;
            posFinal = numRegistros - 1;
        }

        if (posInicial < 0 || posInicial >= numRegistros || posFinal < 0 || posFinal >= numRegistros)
            throw std::string("Posicoes de registro invalidas.");

        long ordemImpressao{(posInicial <= posFinal) ? 1 : -1};
        long int contador{posInicial};
        DadosEmprego *registro = new DadosEmprego();

        std::cout << "\n\n";

        while ((contador * ordemImpressao) <= (posFinal * ordemImpressao))
        {

            arquivoDat.seekp(tamanhoCabecalho + contador * sizeof(DadosEmprego), std::ios::beg);
            arquivoDat.read((char *)registro, sizeof(DadosEmprego));

            if (impressaoDetalhada)
                registro->impressaoDetalhada();
            else
                registro->impressaoResumida();

            contador += ordemImpressao;
        }

        delete registro;
        fecharDat();
    }
    catch (std::string err)
    {
        fecharDat();
        std::cout << err << "\n";
    }
}

void GerenciaDat::inserirRegistro(long int pos, std::string *camposRegistro)
{

    if (pos < 0 || pos > numRegistros) // pos == numRegistors (final do arquivo)
        throw std::string("Posicao de registro invalida.");

    abrirDat(std::ios::in | std::ios::out);
    DadosEmprego *registro = new DadosEmprego(camposRegistro);
    DadosEmprego *aux = new DadosEmprego();

    long int contador{numRegistros - 1};

    while (contador >= pos)
    {

        arquivoDat.seekp(tamanhoCabecalho + contador * sizeof(DadosEmprego), std::ios::beg);
        arquivoDat.read((char *)aux, sizeof(DadosEmprego));
        arquivoDat.write((char *)aux, (sizeof(DadosEmprego)));

        contador--;
    }

    arquivoDat.seekp(tamanhoCabecalho + pos * sizeof(DadosEmprego), std::ios::beg);
    arquivoDat.write((char *)registro, (sizeof(DadosEmprego)));

    numRegistros++;
    atualizarCabecalho();

    delete registro;
    delete aux;

    std::cout << "Registro criado com sucesso.\n";

    fecharDat();
}

void GerenciaDat::trocarRegistros(long int posA, long int posB){

    try
    {

        abrirDat(std::ios::in | std::ios::out);

        if (posA < 0 || posA >= numRegistros || posB < 0 || posB >= numRegistros)
            throw std::string("Posicoes de registro invalidas.");

        DadosEmprego *registroA{new DadosEmprego()}, *registroB{new DadosEmprego()};

        arquivoDat.seekp(tamanhoCabecalho + posA * sizeof(DadosEmprego), std::ios::beg);
        arquivoDat.read((char *)registroA, sizeof(DadosEmprego));

        arquivoDat.seekp(tamanhoCabecalho + posB * sizeof(DadosEmprego), std::ios::beg);
        arquivoDat.read((char *)registroB, sizeof(DadosEmprego));

        arquivoDat.seekp(tamanhoCabecalho + posB * sizeof(DadosEmprego), std::ios::beg);
        arquivoDat.write((char *)registroA, (sizeof(DadosEmprego)));

        arquivoDat.seekp(tamanhoCabecalho + posA * sizeof(DadosEmprego), std::ios::beg);
        arquivoDat.write((char *)registroB, (sizeof(DadosEmprego)));

        delete registroA;
        delete registroB;
        fecharDat();
    }
    catch (std::string err)
    {
        fecharDat();
        std::cout << err << "\n";
    }
}

void GerenciaDat::editarRegistro(long int pos, std::string *camposRegistro)
{

    if (pos < 0 || pos >= numRegistros)
        throw std::string("Posicao de registro invalida.");

    abrirDat(std::ios::in | std::ios::out);

    DadosEmprego *registro = new DadosEmprego(camposRegistro);

    arquivoDat.seekp(tamanhoCabecalho + pos * sizeof(DadosEmprego), std::ios::beg);
    arquivoDat.write((char *)registro, (sizeof(DadosEmprego)));

    delete registro;
    fecharDat();
}

void GerenciaDat::deletarRegistro(long int pos){

    if (pos < 0 || pos >= numRegistros)
        throw std::string("Posicao de registro invalida.");

    abrirDat(std::ios::in | std::ios::out);

    DadosEmprego *registro = new DadosEmprego();
    long int contador{pos + 1};

    while (contador <= numRegistros - 1)
    {

        arquivoDat.seekp(tamanhoCabecalho + contador * sizeof(DadosEmprego), std::ios::beg);
        arquivoDat.read((char *)registro, sizeof(DadosEmprego));
        arquivoDat.seekp(-2 * sizeof(DadosEmprego), std::ios::cur);
        arquivoDat.write((char *)registro, (sizeof(DadosEmprego)));

        contador++;
    }

    numRegistros--;
    atualizarCabecalho();

    delete registro;
    fecharDat();
}