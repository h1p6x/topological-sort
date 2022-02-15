#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <fstream>
#include <locale.h>

using namespace std;

// ����� ��� ������������� �����
class Graph
{
    int V;	// ���������� ������
    list<int>* adj; //  ��������� �� ������, ���������� ������ ���������
    void topologicalSortUtil(int v, int& k, bool visited[], stack<int>& Stack, stack<int> &Num); // �������, ������������ topologicalSort
public:
    Graph(int V); // �����������
    ~Graph(); // ����������
    void addEdge(int v, int w); // ������� ��� ���������� ����� � ����
    bool cyclic(int v, int& cycle_st, vector <int>& color); // �������� �� ������� ������ � �����
    void topologicalSort(int n); // ������� �������������� ���������� �����
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}

Graph::~Graph()
{
    delete[] adj;
    cout << "�������� ����������" << endl;
}

bool Graph::cyclic(int v, int& cycle_st, vector<int>& color)
{
    color[v] = 1;
    list<int>::iterator k;
    for (k = adj[v].begin(); k != adj[v].end(); ++k)
    {
        if (color[*k] == 0)
        { //���� � ������� �� ������� �� ����
            if (cyclic(*k, cycle_st, color))
            {
                return true;
            }
        }
        else if (color[*k] == 1)
        { //���� � ��������� ������� ����� �������, �� ������, ��� ������ ����
            cycle_st = *k; // � ������ �������� ����������
            return true;
        }
    }
    color[v] = 2; //���������, ��� � ������� ������ �� ���� ������� �� �����
    return false;
}

void Graph::addEdge(int v, int w)
{
    adj[v-1].push_back(w-1); // Add w to v�s list.
}

void Graph::topologicalSortUtil(int v, int& k, bool visited[], stack<int>& Stack, stack<int>& Num) // ����������� �������, ������������ topologicalSort
{
    visited[v] = true; // �������� ������� ���� ��� ����������

    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i) // ���������� �������� ������� ��� ���� ������� ������
    {
        if (!visited[*i])
        {
            topologicalSortUtil(*i, k, visited, Stack, Num);
        }
    }

    Num.push(k);
    k--;

    Stack.push(v+1); // ��������� ������� ������� � ���� � �����������
}

// ������� ��� ������ �������������� ����������.
// ���������� ���������� topologicalSortUtil()
void Graph::topologicalSort(int n)
{
    stack<int> Stack;
    stack<int> Num;
    int k = n;
    bool* visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false; // �������� ��� ������� ��� ������������

    // �������� ����������� ��������������� �������
    // ��� ������ �������������� ���������� ��� ������ �������
    for (int i = 0; i < V; i++)
    {
        if (visited[i] == false) //���� � ���� ���������� �������� ������� �� ��������������
        {
            topologicalSortUtil(i, k, visited, Stack, Num); // �� �������� ��� ��� �������� ������ � �������
        }
    }

    // ������� ���������� �����
    while (Stack.empty() == false)
    {
        cout << Stack.top() << " ";
        Stack.pop();
    }
    cout << endl;
    cout << "����� ������ ������: " << endl;
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
    Graph g(n); // ������� ����

    cout << "�������������� ����������������� ����: " << endl;
    for (int i = 0; i < n; i++)
    {
        f >> a >> b;
        cout << a << "  " << b << endl;
        g.addEdge(a, b); // ��������� � ���� �������
    }
    cout << endl;
    int cycle_st = -1;
    for (int i = 0; i < n; i++)
    {
        if (g.cyclic(i, cycle_st, color)) //�������� ����� �� ������������
            break;
    }
    if (cycle_st != -1)
    {
        cout << "******************************************************" << endl;
        cout << "���� ���������� ������������ �������������, �.�. � ��� ������������ ����!" << endl;
        cout << "******************************************************" << endl;
    }
    else
    {
        cout << "������������� ��������������� ����: " << endl;
        g.topologicalSort(n);
    }
    cout << endl;
    return 0;
}
