#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <fstream>
#include <locale.h>

using namespace std;

// Класс для представления графа
class Graph
{
    int V;	// Количество вершин
    list<int>* adj; //  Указатель на массив, содержащий список смежности
    void topologicalSortUtil(int v, int& k, bool visited[], stack<int>& Stack, stack<int> &Num); // Функция, используемая topologicalSort
public:
    Graph(int V); // Конструктор
    ~Graph(); // Деструктор
    void addEdge(int v, int w); // Функция для добавления ребра в граф
    bool cyclic(int v, int& cycle_st, vector <int>& color); // Проверка на наличие циклов в графе
    void topologicalSort(int n); // Выводит топологическую сортировку графа
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}

Graph::~Graph()
{
    delete[] adj;
    cout << "Вызвался деструктор" << endl;
}

bool Graph::cyclic(int v, int& cycle_st, vector<int>& color)
{
    color[v] = 1;
    list<int>::iterator k;
    for (k = adj[v].begin(); k != adj[v].end(); ++k)
    {
        if (color[*k] == 0)
        { //если в вершину не входили ни разу
            if (cyclic(*k, cycle_st, color))
            {
                return true;
            }
        }
        else if (color[*k] == 1)
        { //если в указанную вершину ранее входили, то значит, что найден цикл
            cycle_st = *k; // и меняем значение индикатора
            return true;
        }
    }
    color[v] = 2; //указываем, что в вершину больше ни разу входить не будем
    return false;
}

void Graph::addEdge(int v, int w)
{
    adj[v-1].push_back(w-1); // Add w to v’s list.
}

void Graph::topologicalSortUtil(int v, int& k, bool visited[], stack<int>& Stack, stack<int>& Num) // Рекурсивная функция, используемая topologicalSort
{
    visited[v] = true; // Помечаем текущий узел как посещенный

    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i) // Рекурсивно вызываем функцию для всех смежных вершин
    {
        if (!visited[*i])
        {
            topologicalSortUtil(*i, k, visited, Stack, Num);
        }
    }

    Num.push(k);
    k--;

    Stack.push(v+1); // Добавляем текущую вершину в стек с результатом
}

// Функция для поиска топологической сортировки.
// Рекурсивно использует topologicalSortUtil()
void Graph::topologicalSort(int n)
{
    stack<int> Stack;
    stack<int> Num;
    int k = n;
    bool* visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false; // Помечаем все вершины как непосещенные

    // Вызываем рекурсивную вспомогательную функцию
    // для поиска топологической сортировки для каждой вершины
    for (int i = 0; i < V; i++)
    {
        if (visited[i] == false) //если в ходе предыдущий операций вершина не использовалась
        {
            topologicalSortUtil(i, k, visited, Stack, Num); // То вызываем для нее алгоритм поиска в глубину
        }
    }

    // Выводим содержимое стека
    while (Stack.empty() == false)
    {
        cout << Stack.top() << " ";
        Stack.pop();
    }
    cout << endl;
    cout << "Новые номера вершин: " << endl;
    while (Num.empty() == false)
    {
        cout << Num.top() << " ";
        Num.pop();
    }
    delete[] visited;
}

int main()
{
    setlocale(0, "");
    int n, a, b;
    ifstream f("input.txt", ios::in);
    f >> n;
    vector <int> color(n, 0);
    Graph g(n); // Создаем граф

    cout << "Первоначальный неотсортированный граф: " << endl;
    for (int i = 0; i < n; i++)
    {
        f >> a >> b;
        cout << a << "  " << b << endl;
        g.addEdge(a, b); // Добавляем в граф вершины
    }
    cout << endl;
    int cycle_st = -1;
    for (int i = 0; i < n; i++)
    {
        if (g.cyclic(i, cycle_st, color)) //проверка графа на ацикличность
            break;
    }
    if (cycle_st != -1)
    {
        cout << "******************************************************" << endl;
        cout << "Граф невозможно топлогически отсортировать, т.к. в нем присутствует цикл!" << endl;
        cout << "******************************************************" << endl;
    }
    else
    {
        cout << "Топологически отсортированный граф: " << endl;
        g.topologicalSort(n);
    }
    cout << endl;
    return 0;
}
