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
    

//ifstream arq("input_1.log");

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

    cout << "==============="
            <<"\nCarga de Dados inserida com sucesso, roteamento inicializado!" 
                << "\nNumero de Vertices Unicos (IPs) : " << vert_unicos     
                    << "\nNumero de Arestas Unicas (Rotas) : = " << link_unicos
                        << endl;

    cout << "\n===== MENU ====="
            << "\n1. Exibir grafo completo"
                << "\n2. Encontrar menor caminho"
                    << "\n3. Calcular diametro do grafo"
                        << "\n4. Identificar roteadores criticos"
                            << "\n0. Sair"
                                << "\n==============" << endl;
 
    int op;
    while (cin >> op){
        if (op == 0){ cout << "Saindo..." << endl; break; }
            
        switch(op){
            //EXIBIR GRAFO
            case 1: {
                int metodoSaida;
                do {
                    cout << "Selecione o formato de saída do Grafo:" 
                            << "\n1. Tela" << "\n2. Imagem (PNG)" <<"\n3. Documento (PDF)" << "\n Ou '0' para sair" << endl;

                    cin >> metodoSaida;   
                    if (metodoSaida == 0) break;

                    g.export2dot("rotas.dot");            

                    switch(metodoSaida){
                        case 1: // TELA
                            cout << "\nAbrindo visualizacao com Graphviz (dot -Tx11)...\n";
                            std::system("dot -Tx11 rotas.dot");
                            break;

                        case 2: // PNG
                            cout << "\nExportando para PNG..\n" << endl;
                            system("dot -Tpng rotas.dot -o ROTAS_TELA.png");
                            system("start ROTAS_TELA.png");       
                            break;

                        case 3: // PDF
                            cout << "\nExportando para PDF...\n";
                            std::system("dot -Tpdf rotas.dot -o PDF_ROTAS.pdf");
                            cout << "Arquivo PDF_ROTAS.pdf gerado!\n";
                            break;

                        default:
                            cout <<"\nComando Inválido! Tente novamente.\n";
                    }
                } while (metodoSaida !=0);
            break;
            }

            case 2: //ENCONTRAR MENOR CAMINHO
            break;

            case 3: //DIAMETRO GRAFO
            break;

            case 4: //ENCONTRAR ROTEADORES CRITICOS
            break;
        }

    }
    return 0;
}