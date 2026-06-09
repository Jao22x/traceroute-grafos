#include "graph.cpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char*argv[]){

//ABRIR O ARQUIVO
    if (argc < 2){
        cerr << "O arquivo de log nao foi fornecido." << endl;
        return 1;
    }

    ifstream arq(argv[1]);

    if (!arq.is_open()){
        cerr << "Não foi possível abrir o arquivo para leitura." << endl;
        return 1;
    } 
   
//LER O ARQUIVO
    graph::digraph g;
    unordered_set<string> seen_arestas; //arestas existentes, nao aceita duplicadas
    int vert_unicos = 0;
    int link_unicos = 0;

    string linha;

    while (getline(arq, linha)) {
        stringstream ss(linha);
        string token;

        vector<string> colunas;

        while (getline(ss, token, ',')) {
            colunas.push_back(token);
        }

        if (colunas.size() >= 6) {
            string hop_from = colunas[4];
            string hop_to = colunas[5];

            if (hop_from == "hop_from") continue; //pula cabeçalho

            if (hop_to == "*" || hop_from.empty() || hop_to.empty()) {
                continue; //ignora se estiver vazio ou timeout
            }

            //CRIAR VÉRTICES
            if (g.find(hop_from) == nullptr){
                g.insert_nodo(hop_from);
                vert_unicos++;
            }
            if (g.find(hop_to) == nullptr){
                g.insert_nodo(hop_to);
                vert_unicos++;
            }

            //CRIAR LINKS
            string aresta = hop_from + "->" + hop_to;

            if (seen_arestas.find(aresta) == seen_arestas.end()){
                g.insert_link(hop_from, hop_to);
                seen_arestas.insert(aresta);
                link_unicos++;
            }
        }
    }

    cout << "--------------"
            <<"\nCarga de Dados inserida com sucesso!" 
                << "\nNumero de Vertices Unicos = " << vert_unicos << " | Grafo: " << g.size()
                    << "\nNumero de Links Unicos = " << link_unicos << "| Links grafo: " << seen_arestas.size()
                        << "\n--------------";
 
    //TESTE cout << hop_from << " -> " << hop_to << endl;    

    return 0;
}