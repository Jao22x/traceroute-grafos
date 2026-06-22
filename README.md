# Analisador de Rotas de Rede (Traceroute)

Este projeto em C++ realiza a leitura de arquivos de log contendo saltos de rotas de rede (traceroute) e constrói um **Grafo Direcionado** para analisar a topologia da rede. O sistema permite visualizar o grafo, encontrar o menor caminho entre dois IPs e identificar os roteadores mais críticos.

## Desenvolvido por:
-João Vitor Dalla Lana
-Maria Eduarda Schell

## Estrutura do Projeto

```text
├── output/
│   ├── input_1.log
│   ├── input_2.log
│   ├── input_3.log
│   └── traceroute.exe (Executável Windows gerado anteriormente)
├── graph.cpp        (Implementação da classe do grafo)
└── traceroute.cpp   (Arquivo principal/Main)
````

## Pré-requisitos

Para compilar e visualizar os grafos corretamente em um ambiente Linux, você precisará do compilador GCC (g++) e do pacote Graphviz (responsável por renderizar os arquivos .dot em imagens e PDFs).

Abra o terminal e instale as dependências caso não as tenha:

```bash
sudo apt update
sudo apt install g++ graphviz
```

##  Como Compilar e Executar

### 1. Compilação:

Como o arquivo `traceroute.cpp` já faz a inclusão direta do `graph.cpp` via `#include`, basta compilar o arquivo principal. No terminal, na raiz do projeto, execute:

```bash
g++ traceroute.cpp -o traceroute -O3
```

### 2. Execução:

Passe o caminho de um dos arquivos de log como argumento na execução. Seguindo a estrutura do projeto:

```bash
./traceroute output/input_1.log
```

## Justificativas Técnicas e Escolhas de Implementação

Durante o desenvolvimento do analisador no `traceroute.cpp`, decisões arquiteturais baseadas em Estruturas de Dados Avançadas foram tomadas para garantir que o sistema escale adequadamente ao processar arquivos de log massivos:

### Desempenho Rápido com std::unordered_set (Tabela Hash)

A estrutura `unordered_set` foi utilizada estrategicamente em dois momentos críticos do `traceroute.cpp`:

* Na leitura de dados (seen_arestas): Para garantir que não inseriríamos arestas duplicadas no grafo.
* No Destaque Visual (Geração do .dot): Para separar os nós e links que pertencem ao "menor caminho".

Por que não usar um std::vector? Um vetor exige uma busca linear (complexidade O(N)). Se o grafo tiver 10.000 arestas, verificar se uma aresta específica pertence à rota percorreria toda a lista repetidas vezes, causando gargalos imensos de performance. O unordered_set, por trabalhar com espalhamento (Hashing), responde à pergunta "Esse IP está na lista?" de forma quase instantânea (complexidade média O(1)), mantendo a renderização visual extremamente rápida, independente do tamanho do grafo.

### Associação de Dados e Ordenação com std::pair

Para identificar os roteadores mais críticos (Top 5 com maior Indegree), precisávamos ordenar os vértices com base na sua quantidade de conexões de entrada. A dificuldade aqui é que não podemos ordenar apenas o número de conexões e "perder" a qual IP aquele número pertence.

A solução: Utilizamos o `std::pair<string, size_t>` para encapsular o endereço de IP e sua respectiva contagem de conexões em uma única unidade indissociável. Ao alocarmos esses pares em um `std::vector`, pudemos utilizar a função de ordenação nativa do C++ (std::sort) aplicando uma função comparadora customizada (comparaRoteadores). Isso permitiu ordenar os nós do maior para o menor grau de maneira limpa, eficiente e sem a necessidade de criar structs ou arrays paralelos complexos.

### Representação do Grafo (Lista de Adjacência no Hash Map)

A classe principal do grafo mapeia os rótulos dos nós para a estrutura do vértice. Essa escolha em conjunto com uma lista de ponteiros para as arestas é o modelo ideal para grafos esparsos (como são as topologias de rede reais), economizando muito mais memória se comparado ao uso de uma Matriz de Adjacência Clássica.

### Busca do Menor Caminho (BFS adaptada)

Como os links/saltos de rede não possuem "pesos" distintos (cada salto conta como 1 "hop"), o caminho mais curto é assegurado de forma matemática utilizando a Busca em Largura (BFS). Não foi necessário o uso do Algoritmo de Dijkstra, o que poupou overhead computacional desnecessário.

