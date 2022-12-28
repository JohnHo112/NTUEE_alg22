#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

using namespace std;

void help_message()
{
	cout << "Usage: mps <input_file_name> <output_file_name>" << endl;
}

class Node
{
public:
	Node(int a, int b = 0)
	{
		parent = a;
		rank = b;
	}
	int parent, rank;
};

class Disjoint_Forest
{
public:
	Disjoint_Forest(int V)
	{
		nodes.reserve(V);
	}

	void makeSet(int x)
	{
		Node node(x);
		nodes.push_back(node);
	}

	int findSet(int x)
	{
		while (x != nodes[x].parent)
		{
			x = nodes[x].parent;
		}
		return x;
	}

	void link(int x, int y)
	{
		if (nodes[x].rank > nodes[y].rank)
		{
			nodes[y].parent = x;
		}
		else
		{
			nodes[x].parent = y;
			if (nodes[x].rank == nodes[y].rank)
			{
				nodes[y].rank = nodes[y].rank + 1;
			}
		}
	}

	void uni(int x, int y)
	{
		int xr = findSet(x);
		int yr = findSet(y);
		link(xr, yr);
	}

	vector<Node> nodes;
};

class Edge
{
public:
	Edge(int s, int t, int w)
	{
		src = s;
		dest = t;
		weight = w;
	}

	bool operator==(const Edge &e)
	{
		return (this->src == e.src) && (this->dest == e.dest) && (this->weight == e.weight);
	}

	int src, dest, weight;
};

class Graph
{
public:
	Graph(int x, int y)
	{
		V = x;
		E = y;
		adjList.reserve(V);
		edges.reserve(E);
		vertricsColor.reserve(V);
	}

	void addEdge(Edge e)
	{
		edges.push_back(e);
		// adjList[e.src].push_back(e);
	}

	void printAjd()
	{
		for (int i = 0; i < V; i++)
		{
			printf("%d: ", i);
			for (int j = 0; j < adjList[i].size(); j++)
			{
				printf("%d, %d-> ", adjList[i][j].dest, adjList[i][j].weight);
			}
			printf("\n");
		}
		printf("\n");
	}

	void printEdges()
	{
		printf("print edges: \n");
		for (int i = 0; i < E; i++)
		{
			printf("%d, %d, %d\n", edges[i].src, edges[i].dest, edges[i].weight);
		}
		printf("\n");
	}

	void MergeSort(vector<Edge> &data)
	{
		MergeSortSubVector(data, 0, data.size() - 1);
	}

	void MergeSortSubVector(vector<Edge> &data, int low, int high)
	{
		if (high > low)
		{
			int mid = (high + low) / 2;
			MergeSortSubVector(data, low, mid);
			MergeSortSubVector(data, mid + 1, high);
			Merge(data, low, mid, mid + 1, high);
		}
	}

	void Merge(vector<Edge> &data, int low, int middle1, int middle2, int high)
	{
		vector<Edge> left, right;
		for (int i = low; i <= middle1; i++)
		{
			left.push_back(data[i]);
		}
		for (int i = middle2; i <= high; i++)
		{
			right.push_back(data[i]);
		}
		Edge e1(2147483647, 2147483647, 2147483647);
		Edge e2(2147483647, 2147483647, 2147483647);
		left.push_back(e1);
		right.push_back(e2);
		int i = 0, j = 0;
		for (int k = low; k <= high; k++)
		{
			if (left[i].weight <= right[j].weight)
			{
				data[k] = left[i];
				i++;
			}
			else
			{
				data[k] = right[j];
				j++;
			}
		}
	}

	void QuickSort(vector<Edge> &data)
	{
		QuickSortSubVector(data, 0, data.size() - 1);
	}

	void QuickSortSubVector(vector<Edge> &data, int low, int high)
	{
		if (high > low)
		{
			int q = Partition(data, low, high);
			QuickSortSubVector(data, low, q - 1);
			QuickSortSubVector(data, q + 1, high);
		}
	}

	int Partition(vector<Edge> &data, int low, int high)
	{
		srand(time(NULL));
		int min = low, max = high;
		int xIndex = rand() % (max - min + 1) + min;
		int x = data[xIndex].weight;
		swap(data[xIndex], data[high]);
		int i = low - 1;
		for (int j = low; j < high; j++)
		{
			if (data[j].weight <= x)
			{
				i++;
				swap(data[i], data[j]);
			}
		}
		swap(data[i + 1], data[high]);
		return i + 1;
	}

	void MST_Kruskal(int &cost, vector<Edge> &ans, char &type)
	{
		Disjoint_Forest d(V);
		for (int i = 0; i < V; i++)
		{
			d.makeSet(i);
		}

		if (type == 'u')
		{
			MergeSort(edges);
			// printf("merge sort\n");
		}
		else if (type == 'd')
		{
			QuickSort(edges);
			// printf("quick sort\n");
		}

		reverse(edges.begin(), edges.end());

		for (Edge e : edges)
		{
			if (d.findSet(e.src) != d.findSet(e.dest))
			{
				d.uni(e.src, e.dest);
				adjList[e.src].push_back(e);
			}
			else
			{
				ans.push_back(e);
				cost += e.weight;
			}
		}
	}

	void cycleBreak(int &cost, vector<Edge> &ans, char &type)
	{
		MST_Kruskal(cost, ans, type);
		auto it = ans.begin();
		while (it != ans.end())
		{
			if (it->weight < 0)
			{
				++it;
				continue;
			}
			adjList[it->src].push_back(*it);

			if (checkCycle())
			{
				adjList[it->src].pop_back();
				++it;
			}
			else
			{
				cost -= it->weight;
				it = ans.erase(it);
			}
		}
	}

	bool DFSVisit(int u)
	{
		vertricsColor[u] = 'g';
		bool c = false;
		for (int i = 0; i < adjList[u].size(); i++)
		{
			int v = adjList[u][i].dest;
			if (vertricsColor[v] == 'w')
				c = DFSVisit(v);
			if (vertricsColor[v] == 'g' || c)
				return true;
		}
		vertricsColor[u] = 2;
		return false;
	}

	bool checkCycle()
	{
		bool c = false;
		for (int i = 0; i < V; i++)
		{
			vertricsColor[i] = 'w';
		}
		for (int i = 0; i < V; i++)
		{
			if (vertricsColor[i] == 'w')
				c = DFSVisit(i);
			if (c)
				return true;
		}
		return false;
	}

	int V, E;
	vector<vector<Edge>> adjList;
	vector<Edge> edges;
	vector<char> vertricsColor;
};

void printMST(vector<Edge> edges)
{
	printf("print edges: \n");
	for (int i = 0; i < edges.size(); i++)
	{
		printf("%d, %d, %d\n", edges[i].src, edges[i].dest, edges[i].weight);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	// show the input arg
	if (argc != 3)
	{
		help_message();
		return 0;
	}

	// read input
	fstream fin(argv[1]);
	fstream fout;
	fout.open(argv[2], ios::out);

	char type;
	int V, E;

	fin >> type;
	fin >> V;
	fin >> E;

	Graph g(V, E);
	Graph g2(V, E);
	Graph g3(V, E);

	int a, b, c;
	while (fin >> a >> b >> c)
	{
		g.addEdge(Edge(a, b, c));
		g2.addEdge(Edge(a, b, c));
		g3.addEdge(Edge(a, b, c));
	}
	vector<Edge> ans;
	int cost = 0;

	vector<Edge> second;
	int second_cost = 0;

	vector<Edge> thrid;
	int thrid_cost = 0;

	if (type == 'u')
	{
		g.MST_Kruskal(cost, ans, type);
	}
	else if (type == 'd')
	{
		const clock_t begin_time = clock();

		g.cycleBreak(cost, ans, type);

		// printf("diff time: %f\n", float(clock() - begin_time) / CLOCKS_PER_SEC);
		if ((float(clock() - begin_time) / CLOCKS_PER_SEC) <= 15)
		{
			sleep(1);
			g2.cycleBreak(second_cost, second, type);
			// printf("1: %d\n 2: %d\n", second_cost, cost);
			if (cost >= second_cost)
			{
				cost = second_cost;
				ans = second;
			}
		}

		// printf("diff time: %f\n", float(clock() - begin_time) / CLOCKS_PER_SEC);
		if ((float(clock() - begin_time) / CLOCKS_PER_SEC) <= 30)
		{
			sleep(1);
			g2.cycleBreak(thrid_cost, thrid, type);
			// printf("1: %d\n 2: %d\n", second_cost, cost);
			if (cost >= thrid_cost)
			{
				cost = thrid_cost;
				ans = thrid;
			}
		}

		// printf("1: %d\n 2: %d\n 3: %d\n", cost, second_cost, thrid_cost);
	}

	// printf("cost: %d\n", cost);
	// printMST(ans);

	// write output
	fout << cost << endl;
	for (Edge e : ans)
	{
		fout << e.src << " " << e.dest << " " << e.weight << endl;
	}

	fin.close();
	fout.close();

	return 0;
}