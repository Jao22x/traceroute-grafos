/// biblioteca para grafos
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <cstdlib> // std::system
#include <unordered_set>
#include <algorithm> // std::count
#include <iostream>
#include <queue>

namespace graph
{

  class digraph
  {
  private:
    // nodos do grafo - VERTÍCES
    struct node
    {
      std::string value;         // valor
      std::vector<node *> links; // lista de ponteiros p/ os nodos vizinhos - ARESTAS
    };

    // tabela hash, que mapeia o rótulo do nó para o objeto node
    //<chave>, <valor>
    std::unordered_map<std::string, node> nodes;
    std::unordered_set<node *> visited; // Para o DFS

  public:
    // Insere um novo nó no grafo com o rótulo s
    void insert_nodo(const std::string &s)
    {
      node aux;
      aux.value = s;
      nodes[s] = aux;
    }

    // retorna a quantidade de nós (vertices) do grafo
    size_t size()
    {
      return nodes.size();
    }

    // busca um nó pelo seu rótulo e retorna o endereço do nodo
    node *find(const std::string &s)
    {
      auto it = nodes.find(s); // Este find é do unordered_map
      return it == nodes.end() ? nullptr : &it->second;
    }

    // INsere um aresta dirigida de 'from' para 'to'
    bool insert_link(const std::string &from, const std::string &to)
    {
      auto pfrom = find(from);
      if (pfrom == nullptr)
        return false; // nó de origem ñ existe
      auto pto = find(to);
      if (pto == nullptr)
        return false; // nó de destino ñ existe

      pfrom->links.push_back(pto); // Adiciona a aresta na lista de adjacencia
      return true;
    }

    // Numero de arestas que chegam a um vertice
    size_t indegree(const std::string &s)
    {
      auto p = find(s);
      if (p == nullptr)
      {
        return 0;
      }
      size_t contador = 0;
      for (auto nd : nodes)
      {
        for (auto v : nd.second.links)
        {
          if (s == v->value)
          {
            contador++;
          }
        }
      }
      return contador;
    }
    // Numero de arestas que saem de um vertice
    size_t outdegree(const std::string &s)
    {
      auto p = find(s);
      return p == nullptr ? 0 : p->links.size();
      // mesma coisa
      // if(p==nullptr){
      // return 0;
      //}else{
      // return p->links.size();
      //}
    }
    // Numero total de arestas conectadas a um vertice
    size_t degree(const std::string &s)
    {
      auto p = find(s);
      if (p == nullptr)
      {
        return 0;
      }
      return indegree(s) + outdegree(s);
    }

    void export2dot(const std::string &filename)
    {
      std::ofstream dot(filename); // cria o arquivo
      dot << "digraph{\n";

      for (auto nd : nodes)
      {                                    // Percorre todos os nodos
        dot << "\t\"" << nd.first << "\""; // escreve o rotulo do nodo entre aspas
        if (nd.second.links.size() > 0)
        {
          dot << " -> {";
          for (auto vizinho : nd.second.links)
          {
            dot << "\"" << vizinho->value << "\" ";
          }
          dot << "}"; // Fecha chaves da lista dos vizinho
        }
        dot << ";\n";
      }
      dot << "}\n"; // Fecha chaves do digraph
    }
    void draw()
    {
      export2dot("graphED2.dot");
      // std::system("dot -Tpng graphED2.dot -o graphED2.png");
      std::system("dot -Tx11 graphED2.dot");
    }

    void remove_link(const std::string &from, const std::string &to)
    {
      auto pfrom = find(from);
      if (!pfrom)
        return;
      auto pto = find(to);
      if (!pto)
        return;
      auto it = std::find(pfrom->links.begin(), pfrom->links.end(), pto);
      if (it == pfrom->links.end())
        return;
      pfrom->links.erase(it);
    }

    void remove_nodo(const std::string &key)
    {
      auto p = find(key);
      if (!p)
        return;

      for (auto &[k, nd] : nodes)
      { // é o equivalente a for (auto& entry : nodes) { auto& k  = entry.first;   // rótulo do nó auto& nd = entry.second;  // objeto node }
        auto it = std::find(nd.links.begin(), nd.links.end(), p);
        if (it != nd.links.end())
        { // Encontrou
          nd.links.erase(it);
        }
      }
      nodes.erase(key);
    }

    void recursive_DFS(node *p, int level = 0)
    {
      if (visited.count(p) > 0) // Já foi visitado
        return;
      // std::cout << p->value <<" ";
      std::cout << std::string(level, '\t') << p->value << std::endl;
      visited.insert(p);
      for (auto ln : p->links)
      {
        recursive_DFS(ln, level + 1);
      }
    }

    void DFS_from(const std::string &s)
    {
      auto p = find(s);
      if (!p)
        return;
      visited.clear();
      recursive_DFS(p);
    }

    void BFS_from(const std::string &s)
    {
      auto p = find(s);
      if (!p)
        return;

      visited.clear();
      std::queue<node *> q;
      q.push(p);
      visited.insert(p);

      while (!q.empty())
      {
        auto current = q.front();
        q.pop();
        std::cout << current->value << " ";

        for (auto &n : current->links)
        {
          if (visited.count(n) == 0)
          {
            q.push(n);
            visited.insert(n);
          }
        }
      }
      std::cout << std::endl;
    }

    std::vector<std::string> shortest_path(const std::string &from,
                                           const std::string &to)
    {
      std::vector<std::string> path;
      auto pfrom = find(from);
      if (!pfrom)
        return path;

      auto pto = find(to);
      if (!pto)
        return path;

      if (pfrom == pto)
      {
        path.push_back(from);
        return path;
      }

      std::unordered_map<node *, node *> source;
      std::queue<node *> q;
      q.push(pfrom);
      source[pfrom] = nullptr;
      bool found = false;

      while (!q.empty())
      {
        auto current = q.front();
        q.pop();
        if (current == pto)
        {
          found = true;
          break;
        }
        for (auto &n : current->links)
        {
          if (source.count(n) == 0)
          {
            q.push(n);
            source[n] = current;
          }
        }
      }
      if (found)
      {
        auto p = pto;
        while (p != nullptr)
        {
          path.push_back(p->value);
          p = source[p];
        }
        std::reverse(path.begin(), path.end());
      }
      return path;
    }

  }; /// fim da classe digraph

} // fim do namespace