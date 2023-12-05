# Tratamento de Arquivos e Ordenação em Memória Secundária

Projeto desenvolvido para a disciplina de Estrutura de Dados, ofertada pela UFLA.

## Sobre o projeto

A finalidade deste trabalho é implementar um sistema que converta um arquivo CSV em binário e,  posteriormente, trabalhe com ordenação em memória secundária.

A base de informações utilizada se refere a dados estatísticos relacionados à força de trabalho, coletados em pesquisas como a "Household Labour Force Survey - HLF", "Labour Cost Index - LCI", "Quarterly Employment Survey - QEM", entre outras. A tabela contém uma série de observações e estatísticas relacionadas à força de trabalho e dados de emprego.


## Saídas no terminal

``` 

***************** MENU PRINCIPAL ******************

Escolha uma opcao:

[1] Importar/Exportar arquivos
[2] Manipular registros
[3] Ordenacao Externa dos Registros
[4] Finalizar programa

===>
```

``` 

******************** ARQUIVOS *********************

Escolha uma opcao:

[1] Conversão CSV -> Binario
[2] Conversão Binario -> CSV
[3] Menu anterior (principal)

===>
``` 
``` 

******************** REGISTROS ********************

Escolha uma opcao:

[1] Visualizar registros
[2] Inserir registro
[3] Deletar registro
[4] Editar registro
[5] Inverter posicao entre registros
[6] Menu anterior (principal)

===>
``` 

```

************** VISUALIZAR REGISTROS ***************

Escolha uma opcao:

[1] Visualizar registros entre posições X e Y
[2] Visualizar todos os registros
[3] Visualizar com detalhes
[4] Menu anterior (registros)

===>
``` 

## Detalhes do projeto

* Basta compilar o arquivo main.cpp
* Todos os arquivos manipulados pelo programa devem ficar na pasta ``` "./baseDados/" ```
* O programa consegue gerenciar vários registros csv e binário em uma mesma execução
* Qualquer tipo de importação/exportação de arquivos concatena os dados no arquivo de destino, nunca os sobreescreve

## Detalhes da implementação

Durante a fase de distribuição dos registros, a estrutura de dados **minheap** se encaixou muito bem como uma maneira de otimizar o processo de ordenação externa em um processo muito similar ao **heap sort**. Para isso, foi atribuído aos dados do heap um valor do tipo inteiro denominado **pesoSegmento**. Dessa maneira, caso um valor que viesse a ser inserido na estrutura fosse menor que a raiz atual, tal valor é acrescido de um peso maior, visando que o mesmo **afunde no heap**, evitando que a ordenação do bloco atual seja interrompida por tal valor e um novo bloco venha a ser criado. Sendo assim, valores com pesos superiores têm sua inserção adiada nos arquivos temporários, resultado em **blocos os maiores possíveis** e, consequentemente, **redução de etapas na fase de intercalação dos registros**.

Além disso, um detalhe importante a ser mencionado é a criação do método **removeInserindo** na estrutura heap. Tradicionalmente, remoções nessa estrutura são feitas trocando o último registro pela raiz e, após isso, o método **corrigeDescendo** é chamado a partir da nova raiz. Já no caso de inserções, todo valor é inserido na última folha disponível e, após isso, o método **corrigeSubindo** é invocado a partir dessa folha. Dito isso, tendo em vista que grande parte das remoções no heap são acompanhadas da inserção do próximo registro disponível no arquivo, o método **removeInserindo** otimiza esse processo substituindo a antiga raiz pelo novo valor lido do arquivo e apenas se preocupa em chamar o método “corrigeDescendo” a partir da nova raiz. Dessa maneira, alterações desnecessárias na estrutura são evitadas, considerando esse contexto em específico da atuação do algoritmo. Como resultado, foi observado a obtenção de tempos de execução inferiores após tal melhoria no programa.

A fase de distribuição dos registros tem como resultado a criação de blocos ordenados distribuídos em dois arquivos temporários. Na fase subsequente, o **merge sort externo** é responsável por **intercalar os registros** de tais blocos, criando blocos ainda maiores (também ordenados) e os distribui em outros dois arquivos temporários. Sendo assim, ao todo, **quatro arquivos temporários** são criados, dois deles utilizados como **arquivos de entrada** e dois como **arquivos de saída**, sendo que tais papéis **se invertem** a cada execução do merge sort.

No final, apenas um único grande bloco é criado e a ordenação encontra-se completa.
