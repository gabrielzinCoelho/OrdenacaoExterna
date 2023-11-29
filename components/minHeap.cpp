#pragma once
#include <stdexcept>
#include "./minHeap.hpp"
#include "./dadosHeap.cpp"

MinHeap::MinHeap(DadosEmprego *arr, int tamanho) : capacidade(tamanho), tamanho(tamanho), heap(new DadosHeap[tamanho]) {
    
    for(int i{0}; i<tamanho; i++)
        std::memcpy(heap + i, arr + i, sizeof(DadosEmprego));
    
    arruma();
}

MinHeap::~MinHeap(){
    delete[] heap;
}

void MinHeap::arruma(){

    for(int i{tamanho/2 - 1}; i>=0; i--) // metade para cima (apenas nós não-folhas) até a raiz
        corrigeDescendo(i);
}

int MinHeap::pai(int i){
    return (i - 1)/2;
}

int MinHeap::esquerdo(int i){
    return i*2 + 1;
}

int MinHeap::direito(int i){
    return i*2 + 2;
}

void MinHeap::corrigeDescendo(int i){

    int esq{esquerdo(i)}, dir{direito(i)};
    int indexMenor = i;

    if(esq < tamanho && heap[esq] < heap[indexMenor]) // esq é indice valido e é menor q o pai
        indexMenor = esq;

    if(dir < tamanho && heap[dir] < heap[indexMenor]) // dir é indice valido e menor que esq ou pai
        indexMenor = dir;
    
    if(indexMenor != i){ // menor nao é pai
        std::swap(heap[i], heap[indexMenor]); // troca pai com menor filho
        corrigeDescendo(indexMenor);
    }
}

void MinHeap::corrigeSubindo(int i){

    if(i == 0)
        return;

    int indexPai{pai(i)};

    if(heap[i] < heap[indexPai]){
        std::swap(heap[i], heap[indexPai]);
        corrigeSubindo(indexPai);
    }

}

DadosHeap MinHeap::removeRaiz(){

    if(vazia()) throw std::runtime_error("Heap vazia.\n");

    DadosHeap temp = heap[0];

    std::swap(heap[0], heap[tamanho - 1]);
    tamanho--;

    corrigeDescendo(0);
    return temp;
}

DadosHeap MinHeap::removeInserindo(const DadosHeap &d){

    if(!tamanho) throw std::runtime_error("Heap vazia.\n");

    DadosHeap temp = heap[0];
    heap[0] = d;

    corrigeDescendo(0);
    return temp;
}


DadosHeap MinHeap::espiaRaiz(){
    if(!tamanho) throw std::runtime_error("Heap vazia.\n");
    return heap[0];
}

void MinHeap::insere(const DadosHeap &d){

    if(tamanho == capacidade) throw std::runtime_error("Heap cheia.\n");

    heap[tamanho] = d;
    corrigeSubindo(tamanho);

    tamanho++;
}

bool MinHeap::vazia(){
    return tamanho <= 0;
}