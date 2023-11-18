#pragma once
#include "./dadosHeap.hpp"

DadosHeap::DadosHeap() : pesoValor(0){}

DadosHeap::DadosHeap(DadosEmprego *d, int p) : pesoValor(p), valorDado(*d) {}

bool DadosHeap::operator<(const DadosHeap &d){
    
    if(this->pesoValor < d.pesoValor)
        return true;
    if(this->pesoValor > d.pesoValor)
        return false;

    return this->valorDado < d.valorDado;

}