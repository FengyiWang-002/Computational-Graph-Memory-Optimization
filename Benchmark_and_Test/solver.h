#pragma once
#include"algorithm.h"
#include<chrono>
#include"graph.h"
#include<tuple>

#define micros 1000000
#define nanos 1000000000

using namespace std;

void solver(vector<VertexNode>& graph, vector<VertexNode>& parent, string algorithm, bool check, vector<float>& peaks, 
	vector<double>& durations) {
	cout << "number of nodes:" << graph.size() << endl;
	auto start = chrono::high_resolution_clock::now();
	vector<int> topological_order = Algorithm(graph, parent, algorithm);
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<long long, std::nano> duration = end - start;
	auto start1 = chrono::high_resolution_clock::now();
	if (check) {
		auto memory = memory_peak_order_check(graph, topological_order);
	}
	auto memory = memory_peak(graph, topological_order);
	vector<float> memory_used = get<0>(memory);
	float peak_memory = get<1>(memory);
	auto end1 = chrono::high_resolution_clock::now();
	chrono::duration<long long, std::nano> duration1 = end1 - start1;

	if (algorithm == "DFS") {
		peaks[0] += peak_memory;
		durations[0] += double(duration.count()) / nanos;
	}
	else if (algorithm == "Inverse_DFS") {
		peaks[1] += peak_memory;
		durations[1] += double(duration.count()) / nanos;
	}
	else if (algorithm == "Quick") {
		peaks[2] += peak_memory;
		durations[2] += double(duration.count()) / nanos;
	}
	else if (algorithm == "Quick_v2") {
		peaks[3] += peak_memory;
		durations[3] += double(duration.count()) / nanos;
	}

	cout << "total sort time:" << double(duration.count()) / nanos << "s" << endl;
    cout << "peak memory is " << peak_memory << endl;
	cout << "postprocess time:" << double(duration1.count()) / nanos << "s" << endl;
	cout << endl;
}

void tester(vector<VertexNode> graph, vector<VertexNode>& parent, bool check, vector<float>& peaks, vector<double>& durations) {
	for (string algorithm : {"DFS","Inverse_DFS","Quick","Quick_v2"}) {
		solver(graph, parent, algorithm, check, peaks, durations);
	}
}
