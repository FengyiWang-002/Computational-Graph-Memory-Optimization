//for dataset
#include<iostream>
#include<string>
#include<fstream>
#include<random>
#include"random_number.h"
#include<set>

using namespace std;

#define small 10
#define medium 500
#define large 10000

#define SPARSE 0.01
#define PROBFOREDGE 0.1 //0.1*2
#define INTENSIVE 0.3

void generateDAG(ofstream& file, int n,
			int MaxPathLength, set<pair<int, int>>& edges,
			vector<double>& weights, vector<vector<int>> layers,
			bool normal_or_not, string mode) {
	weights.resize(n);
	for (int i = 0; i < n; ++i) {
		weights[i] = normal_or_not ? generateNormalRandomFloat(50.0, 15.0) : generateRandomFloat(0.0, 100.0);
	}
	for (int i = 0; i < MaxPathLength; i++) {
		layers[i].push_back(i);
	}
	for (int i = MaxPathLength; i < n; i++) {
		layers[generateRandomInt(0, MaxPathLength - 2)].push_back(i);//方便考虑 最终层节点数为1
	}
	float prob;
	if (mode=="sparse") {
		prob = SPARSE;
	}
	else if (mode == "intensive") {
		prob = INTENSIVE;
	}
	else {
		prob = PROBFOREDGE;
	}
	for (int i = 0; i < MaxPathLength - 1; i++) {
		for (int u : layers[i]) {
			bool hasEdgeToNextLayer = false;
			for (int v : layers[i + 1]) {
				if (generateRandomFloat(0.0, 1.0) < prob) {
					edges.insert({ u, v });
					hasEdgeToNextLayer = true;
				}
			}
			if (!hasEdgeToNextLayer) {
				int v = layers[i + 1][generateRandomInt(0, layers[i + 1].size() - 1)];
				edges.insert({ u, v });
			}
		}
	}

	for (int i = 1; i < MaxPathLength; i++) {
		for (int u : layers[i]) {
			bool hasEdgeToPrevLayer = false;
			for (int v : layers[i - 1]) {
				if (generateRandomFloat(0.0, 1.0) < prob) {
					edges.insert({ v, u });
					hasEdgeToPrevLayer = true;
				}
			}
			if (!hasEdgeToPrevLayer) {
				int v = layers[i - 1][generateRandomInt(0, layers[i - 1].size() - 1)];
				edges.insert({ v, u });
			}
		}
	}
	int num_edges = edges.size();
	file << n << " " << num_edges << endl;
	for (pair<int,int> pair:edges) {
		file << pair.first << " " << pair.second << endl;
	}
	for (int i = 0; i < n; i++) {
		file << weights[i] << endl;
	}
}

void generate_data(ofstream &file, string data_size, bool normal_or_not = false, string mode ="none") {
	int n, MaxPathLength;
	if (data_size == "tiny") {
		n = generateRandomInt(10,20);
		MaxPathLength = generateRandomInt(3, 10);
	}
	else if (data_size == "fair") {
		n = 1000;
		MaxPathLength = 50;
	}
	else if(data_size == "big"){
		n = 20000;
		MaxPathLength = 200;
	}
	else {
		n = 100000;
		MaxPathLength = 500;
	}
	vector<double> weights;
	set<pair<int, int>> edges;
	vector<vector<int>> layers;
	layers.resize(MaxPathLength);
	generateDAG(file, n, MaxPathLength, edges, weights, layers, normal_or_not, mode);
}

void generate_dataset(string dataset_size, string data_size, bool normal_or_not = false,string mode = "none") {
	int size;
	if (dataset_size == "small") {
		size = small;
	}
	else if (dataset_size == "medium") {
		size = medium;
	}
	else if (dataset_size == "large") {
		size = large;
	}
	else {
		size = 1;
	}
	string name = dataset_size + "_" + data_size + "_dataset.txt";
	if (normal_or_not) {
		name = "normal_" + name;
	}
	if (mode == "sparse") {
		name = "sparse_" + name;
	}
	if (mode == "intensive") {
		name = "intensive_" + name;
	}
	ofstream file(name);
	for (int i = 0; i < size; i++) {
		generate_data(file, data_size, normal_or_not, mode);
	}
}

int main() {
	string dataset_sizes[] = {"small","medium","large"};
	string data_sizes[] = {"tiny","fair","big"};
	string modes[] = { "none","sparse","intensive" };
	/*
	generate_dataset("test1", "tiny", true, "none");
	printf("A dataset has been successfully generated！\n");
	generate_dataset("small", "big", true, "none");
	printf("A dataset has been successfully generated！\n");
	generate_dataset("small", "big", true, "sparse");
	printf("A dataset has been successfully generated！\n");
	generate_dataset("small", "big", true, "intensive");
	printf("A dataset has been successfully generated！\n");
	generate_dataset("large", "tiny", true, "none");
	printf("A dataset has been successfully generated！\n");
	generate_dataset("medium", "fair", true, "sparse");
	printf("A dataset has been successfully generated！\n");
	generate_dataset("medium", "fair", true, "intensive");
	printf("A dataset has been successfully generated！\n");
	generate_dataset("medium", "fair", true, "none");
	printf("A dataset has been successfully generated！\n");
	generate_dataset("test2", "100000", true, "none");
	printf("A dataset has been successfully generated！\n");
	generate_dataset("test2", "100000", true, "sparse");
	printf("A dataset has been successfully generated！\n");
	generate_dataset("test2", "100000", true, "intensive");
	printf("A dataset has been successfully generated！\n");
	*/
	generate_dataset("small", "100000", true, "none");
	printf("A dataset has been successfully generated！\n");
	generate_dataset("small", "100000", true, "sparse");
	printf("A dataset has been successfully generated！\n");
	generate_dataset("small", "100000", true, "intensive");
	printf("A dataset has been successfully generated！\n");
	return 0;
}