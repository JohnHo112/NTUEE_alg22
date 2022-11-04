#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

void help_message()
{
	cout << "Usage: mps <input_file_name> <output_file_name>" << endl;
}

int findAnotherEndpoint(int j, vector<int> nodePairs)
{
	return nodePairs[j];
}

vector<vector<int>> mps(int n, vector<int> nodePairs)
{
	vector<vector<int>> m(n, vector<int>(n, 0));
	for (int j = 0; j < n; j++)
	{
		int k = findAnotherEndpoint(j, nodePairs);
		for (int i = 0; i <= j - 1; i++)
		{
			if (i <= k && k <= j - 1 && m[i][k - 1] + 1 + m[k + 1][j - 1] > m[i][j - 1])
			{
				m[i][j] = m[i][k - 1] + 1 + m[k + 1][j - 1];
			}
			else
			{
				m[i][j] = m[i][j - 1];
			}
		}
	}
	return m;
}

void findChord(int i, int j, vector<vector<int>> &m, vector<int> &nodePairs, vector<int> &kArray)
{
	int k = 0;
	k = findAnotherEndpoint(j, nodePairs);
	if (j > i)
	{
		if (k > j || k < i)
		{
			findChord(i, j - 1, m, nodePairs, kArray);
		}
		else if (k > i && k < j)
		{
			if (m[i][k - 1] + m[k][j] > m[i][j - 1])
			{
				findChord(i, k - 1, m, nodePairs, kArray);
				findChord(k, j, m, nodePairs, kArray);
			}
			else
			{
				findChord(i, j - 1, m, nodePairs, kArray);
			}
		}
		else
		{
			kArray.push_back(k);
			findChord(i + 1, j - 1, m, nodePairs, kArray);
		}
	}
}

int main(int argc, char *argv[])
{
	// show the input arg
	if (argc != 3)
	{
		help_message();
		return 0;
	}

	// read input and build node vector
	fstream fin(argv[1]);
	fstream fout;
	fout.open(argv[2], ios::out);
	int n;
	int a, b;
	fin >> n;
	vector<int> nodePairs(n, 0);
	vector<int> kArray;
	while (fin >> a >> b)
	{
		nodePairs[a] = b;
		nodePairs[b] = a;
	}

	vector<vector<int>> m = mps(n, nodePairs); // mps number
	findChord(0, n - 1, m, nodePairs, kArray); // find chord array

	// write file
	fout << m[0][n - 1] << endl;
	for (int i = 0; i < kArray.size(); i++)
	{
		fout << kArray[i] << " " << findAnotherEndpoint(kArray[i], nodePairs) << endl;
	}
	fin.close();
	fout.close();

	return 0;
}
