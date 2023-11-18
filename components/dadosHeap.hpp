#pragma once
#include "../models/dadosEmprego.hpp"

class DadosHeap{
    friend class MinHeap;
    friend class OrdenacaoExterna;
    private:
        DadosEmprego valorDado;
        unsigned int pesoValor;
    public:
        DadosHeap();
        DadosHeap(DadosEmprego *d, int p = 0);
        bool operator<(const DadosHeap &d);
};
