#include "graph.cpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

int main(int argc, char*argv[]){

//ABRIR O ARQUIVO
    if (argc < 2){
        cerr << "O arquivo de log não foi fornecido." << endl;
        return 1;
    }

    ifstream arq(argv[1]);

    if (!arq.is_open()){
        cerr << "Não foi possível abrir o arquivo para leitura." << endl;
        return 1;
    } 
   

//LER O ARQUIVO
    graph::digraph g;

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
                continue; //ignora se estiver vazio
            }

            //TESTE
            cout << hop_from << " -> " << hop_to << endl;    
            
            //CRIAR GRAFO

            if (g.find(hop_from) == nullptr){
                g.insert_nodo(hop_from);
            }
            if (g.find(hop_to) == nullptr){
                g.insert_nodo(hop_to);
            }
            g.insert_link(hop_from, hop_to);
        }
    }

    return 0;
}