//plz use c++17
#include<iostream>
#include<fstream>
#include<filesystem>
#include"solver.h"
#include"graph.h"

using namespace std;

int main() {
	filesystem::path current_path = filesystem::current_path();
	filesystem::path parent_path = current_path.parent_path();
	ifstream input(parent_path.string() + "/Dataset/intensive_normal_small_100000_dataset.txt");

	if (!input) {
		std::cerr << "Unable to open the file." << std::endl;
		return 1;
	}

	vector<vector<VertexNode>> Graphs(10); //这里数据要根据数据集大小调整
	vector<vector<VertexNode>> Parents(10);
	int n, m;
	int i = 0;
	while (input >> n >> m) {
		vector<VertexNode> graph(n);
		vector<VertexNode> parent(n);
		for (int i = 0; i < m; i++) {
			int u, v;
			input >> u >> v;
			graph[u].adjacency_list.push_back(v);
			parent[v].adjacency_list.push_back(u);
		}
		for (int i = 0; i < n; i++) {
			input >> graph[i].memory;
		}
		Graphs[i] = graph;
		Parents[i] = parent;
		i++;
	}
	cout << "read data successfully" << endl;
	input.close();
	vector<string> algorithm_list = { "Greedy", "DFS", "Inverse_DFS", "Quick", "Quick_v2" };
	bool check = false;
	vector<float> peaks(4, 0.0f);
	vector<double> durations(4, 0.0f);

	for (int j = 0; j < i; j++) {
		vector<VertexNode> graph = Graphs[j];
		vector<VertexNode> parent = Parents[j];
		tester(graph, parent, check, peaks, durations);
	}

	for (int j = 0; j < 4; j++) {
		peaks[j] /= i;
		durations[j] /= i;
	}
	cout << "average result:" << endl;
	cout << "DFS: average peak memory :" << peaks[0] << " average sort time :" << durations[0] << endl;
	cout << "Inverse_DFS: average peak memory :" << peaks[1] << " average sort time :" << durations[1] << endl;
	cout << "Quick: average peak memory :" << peaks[2] << " average sort time :" << durations[2] << endl;
	cout << "Quick_v2: average peak memory :" << peaks[3] << " average sort time :" << durations[3] << endl;

	return 0;
}
