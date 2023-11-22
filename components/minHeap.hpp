#pragma once
#include <iostream>
#include "./dadosHeap.hpp"
#include "../models/dadosEmprego.hpp"
#include <cstring>
// #include <cstdlib>

class MinHeap{
    
    private:
        DadosHeap *heap;
        int capacidade, tamanho;
        int pai(int i);
        int direito(int i);
        int esquerdo(int i);
        void arruma();
        void corrigeDescendo(int i);
        void corrigeSubindo(int i);
    public:
        MinHeap(DadosEmprego *arr, int tamanhoArr);
        ~MinHeap();
        DadosHeap espiaRaiz();
        DadosHeap removeRaiz();
        DadosHeap removeInserindo(const DadosHeap &d);
        void insere(const DadosHeap &d);
        bool vazia();
};