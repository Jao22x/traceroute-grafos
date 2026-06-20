#include "graph.cpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_set>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

bool comparaRoteadores(const pair<string, size_t>& a, const pair<string, size_t>& b) {
    return a.second > b.second; //retorna true se o primeiro tiver mais entradas que o segundo
}

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
    unordered_set<string> seenArestas; //arestas existentes, nao aceita duplicadas
    vector<string> ips; //lista de todos os ips

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

                ips.push_back(hop_from);
            }
            if (g.find(hop_to) == nullptr){
                g.insert_nodo(hop_to);
                vert_unicos++;

                ips.push_back(hop_to);
            }

            //CRIAR LINKS
            string aresta = hop_from + "->" + hop_to;

            if (seenArestas.find(aresta) == seenArestas.end()){
                g.insert_link(hop_from, hop_to);
                seenArestas.insert(aresta);
                link_unicos++;
            }
        }
    }

    cout << "==============="
            <<"\nCarga de Dados inserida com sucesso, roteamento inicializado!" 
                << "\nNumero de Vertices Unicos (IPs) : " << vert_unicos     
                    << "\nNumero de Arestas Unicas (Rotas) : " << link_unicos
                        << endl;
 
    //MENU PRINCIPAL         
    int op;
    while (true){
        cout << "\n===== MENU ====="
            << "\n1. Exibir grafo completo"
            << "\n2. Encontrar menor caminho"
            << "\n3. Calcular diametro do grafo"
            << "\n4. Identificar roteadores criticos"
            << "\n0. Sair"
            << "\n==============" << endl;

        if (!(cin >> op) || op < 0 || op > 4){ //tratamento de erro de entrada
            cin.clear();
            cin.ignore(100, '\n');
            cerr << "Erro! Tente novamente" << endl;
            continue;
        }

        if (op == 0){ cout << "Saindo..." << endl; break; }
            
        switch(op){
            case 1: { //EXIBIR GRAFO -- SUBMENU
                int metodoSaida;
                do {
                    cout << "Selecione o formato:" 
                            << "\n1. Tela" << "\n2. Imagem (PNG)" <<"\n3. Documento (PDF)" << "\n Ou '0' para sair" << endl;

                    cin >> metodoSaida;   
                    if (metodoSaida == 0) break;

                    g.export2dot("rotas.dot");            

                    switch(metodoSaida){
                        case 1: // TELA
                            cout << "\nAbrindo visualizacao com Graphviz (dot -Tx11)...\n";
                            system("dot -Tx11 rotas.dot");
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

            case 2: { // ENCONTRAR MENOR CAMINHO
                string origem, destino;
                cout << "\nEncontrando menor caminho:\nIP de origem: "; cin >> origem;
                cout << "IP de destino: "; cin >> destino;
                cout << "==============" << endl;

                vector<string> rota = g.shortest_path(origem, destino);

                if(rota.empty()){  cout << "Não foi encontrado um caminho entre os IP's." << endl; }
                else if(rota.size() == 1){ cout << "A origem e o destino são o mesmo IP: " << rota.front() << endl; }
                else{
                    cout << "Caminho encontrado (" << rota.size() - 1 << " saltos):" << endl;
                    for (size_t i = 0; i < rota.size(); i++){
                        cout << rota[i];
                        if (i < rota.size() - 1){
                            cout << " -> ";
                        }
                    }

                    ofstream dot("rota_destacada.dot");
                    dot << "digraph {\n";
                    
                    int metodoSaida;
                    do {
                        cout << "\n\nSelecione o formato para a Saida Visual do caminho:" 
                             << "\n1. Tela" << "\n2. Imagem (PNG)" << "\n3. Documento (PDF)" << "\n Ou '0' para sair" << endl;

                        cin >> metodoSaida;   
                        if (metodoSaida == 0) break;

                        unordered_set<string> rotaNodos(rota.begin(), rota.end());
                        unordered_set<string> rotaLinks;
                        for (size_t i = 0; i + 1 < rota.size(); ++i) {
                            rotaLinks.insert(rota[i] + "->" + rota[i+1]);
                        }


                        // PINTAR VERTICES
                        for (auto& ip : ips) {
                            dot << "\t\"" << ip << "\"";
                            if (rotaNodos.find(ip) != rotaNodos.end()) {
                                dot << " [color=red, style=filled, fillcolor=yellow, penwidth=3]";
                            }
                            dot << ";\n";
                        }

                        // PINTAR LINKS
                        for (auto& aresta : seenArestas) {
                            size_t pos = aresta.find("->");
                            string from = aresta.substr(0, pos);
                            string to = aresta.substr(pos + 2);

                            dot << "\t\"" << from << "\" -> \"" << to << "\"";
                            
                            if (rotaLinks.find(aresta) != rotaLinks.end()) {
                                dot << " [color=red, penwidth=3]";
                            }
                            dot << ";\n";
                        }
                        dot << "}\n";
                        dot.close();

                        switch(metodoSaida){
                            case 1: // TELA
                                cout << "\nAbrindo visualizacao do caminho com Graphviz...\n";
                                system("dot -Tx11 rota_destacada.dot");
                                break;

                            case 2: // PNG
                                cout << "\nExportando rota para PNG..\n" << endl;
                                system("dot -Tpng rota_destacada.dot -o CAMINHO_TELA.png");
                                system("start CAMINHO_TELA.png");   
                                break;

                            case 3: // PDF
                                cout << "\nExportando rota para PDF...\n";
                                system("dot -Tpdf rota_destacada.dot -o PDF_CAMINHO.pdf");
                                cout << "Arquivo PDF_CAMINHO.pdf gerado!\n";
                                break;

                            default:
                                cout <<"\nComando Inválido! Tente novamente.\n";
                        }
                    } while (metodoSaida != 0);
                }   
                break;
            }
            
            case 3: //DIAMETRO GRAFO
                cout << "Exibindo diametro do grafo de redes: \n"
                     << g.size() << " Roteadores." << endl;
                break;

            case 4:  //ENCONTRAR ROTEADORES CRITICOS
                cout << "\n=================" << endl;
                cout << "Exibindo TOP 5 roteadores criticos (Com maior numero de entradas): " << endl;
                cout << "=================" << endl;
                
                vector< pair<string, size_t> > rotCriticos; //pair = cada roteador <ip, entradas>
                
                for (auto ip : ips) {
                    size_t entradas = g.indegree(ip);
                    rotCriticos.push_back({ip, entradas});
                }

                sort(rotCriticos.begin(), rotCriticos.end(), comparaRoteadores); //ordena os rot com mais entradas
                
                for (int i = 0; i < 5; i++) {
                        cout << i + 1 << " Lugar -> Roteador: " << rotCriticos[i].first 
                             << " | Entradas: " << rotCriticos[i].second << endl;
                }
                break;
        }
    }
    return 0;
}