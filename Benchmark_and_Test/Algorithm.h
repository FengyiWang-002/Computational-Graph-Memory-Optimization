#pragma once
#include<string>
#include<stack>
#include<vector>
#include"graph.h"
#include<chrono>
#include<bitset>
#include<thread>
#include<unordered_set>
#include<queue>

using namespace std;

const int MAX_NODES = 100000;
const float ALPHA = 1.0;
//bitset<MAX_NODES> visited;

bool are_childs_executed(int node, vector<VertexNode>& graph, vector<int>& node_executed) {
	for (int child : graph[node].adjacency_list) {
		if (node_executed[child] == 0) {
			return false;
		}
	}
	return true;
}
bool are_childs_available(int node, vector<VertexNode>& graph, vector<int>& available_time) {
	for (int child : graph[node].adjacency_list) {
		if (available_time[child] >= 0) {
			return false;
		}
	}
	return true;
}

auto memory_peak_order_check(vector<VertexNode>& graph, vector<int>& topo_order) {
	int n = graph.size();
	vector<float> memory_used(n, 0.0f);
	float current_memory = 0.0f;
	float peak_memory = 0.0f;

	vector<int> node_executed(n, 0);
	unordered_set<int> waiting_for_release;
	for (int i = 0; i < topo_order.size(); i++) {
		cout << endl << "The " << i << "th: topo_order: " << topo_order[i] << " waiting_to_release: ";
		for (int node : waiting_for_release) {
			cout << node << " ";
		}
		int node = topo_order[i];
		current_memory += graph[node].memory;
		node_executed[node] = true;
		memory_used[i] = current_memory;
		peak_memory = max(peak_memory, current_memory);
		waiting_for_release.insert(node);
		vector<int> nodes_to_remove;
		for (int node : waiting_for_release) {
			if (are_childs_executed(node, graph, node_executed)) {
				current_memory = current_memory - graph[node].memory;
				nodes_to_remove.push_back(node);
			}
		}
		for (int node : nodes_to_remove) {
			waiting_for_release.erase(node);
		}
	}
	cout << endl << "waiting_to_release: ";
	for (int node : waiting_for_release) {
		cout << node << " ";
	}
	cout << endl;
	if (!waiting_for_release.size() == 0) { cout << "wrong!" << endl; }
	return make_tuple(memory_used, peak_memory);
}

auto memory_peak(vector<VertexNode>& graph, vector<int>& topo_order) {
	int n = graph.size();
	vector<float> memory_used(n, 0.0f);
	float current_memory = 0.0f;
	float peak_memory = 0.0f;

	vector<int> node_executed(n, 0);
	unordered_set<int> waiting_for_release;
	for (int i = 0; i < topo_order.size(); i++) {
		int node = topo_order[i];
		current_memory += graph[node].memory;
		node_executed[node] = true;
		memory_used[i] = current_memory;
		peak_memory = max(peak_memory, current_memory);
		waiting_for_release.insert(node);
		vector<int> nodes_to_remove;
		for (int node : waiting_for_release) {
			if (are_childs_executed(node, graph, node_executed)) {
				current_memory = current_memory - graph[node].memory;
				nodes_to_remove.push_back(node);
			}
		}
		for (int node : nodes_to_remove) {
			waiting_for_release.erase(node);
		}
	}
	if (!waiting_for_release.size() == 0) { cout << "wrong!" << endl; }
	return make_tuple(memory_used, peak_memory);
}

void dfs(int node, vector<VertexNode>& graph, vector<int>& visited, stack<int>& topological_stack) {
	visited[node] = true;
	for (int neighbor : graph[node].adjacency_list) {
		if (visited[neighbor] == false) {
			dfs(neighbor, graph, visited, topological_stack);
		}
	}
	topological_stack.push(node);
};

vector<vector<int>> SortByLayers(vector<VertexNode>& graph) {
	int n = graph.size(); 
	vector<int> indegree(n, 0);
	vector<vector<int>> layers;
	for (int i = 0; i < n; ++i) {
		for (int neighbor : graph[i].adjacency_list) {
			indegree[neighbor]++;
		}
	}
	queue<int> q;
	for (int i = 0; i < n; ++i) {
		if (indegree[i] == 0) {
			q.push(i);
		}
	}
	while (!q.empty()) {
		int currentLayerSize = q.size();
		vector<int> currentLayer;

		for (int i = 0; i < currentLayerSize; ++i) {
			int node = q.front();
			q.pop();
			currentLayer.push_back(node);

			for (int neighbor : graph[node].adjacency_list) {
				if (--indegree[neighbor] == 0) {
					q.push(neighbor);
				}
			}
		}
		layers.push_back(currentLayer);
	}
	cout << "num_layers:" << layers.size() << endl;

	return layers;
}

void dfs_without_recursion(int node, vector<VertexNode>& graph, vector<VertexNode>& parent, vector<int>& visited, vector<int>& topological_order) {
	stack<int> s;
	s.push(node);

	while (!s.empty()) {
		int current = s.top();
		s.pop();

		if (visited[current] == false) {
			visited[current] = true;
			topological_order.push_back(current);

			for (int child : graph[current].adjacency_list) {
				if (visited[child] == 0&&are_your_parents_all_visited(node, child, parent, visited)) {
					s.push(child);
				}
			}
		}
	}
}

vector<int> DFS(vector<VertexNode>& graph, vector<VertexNode>& parent) {
	int n = graph.size();
	vector<int> visited(n, 0);
	vector<int> topological_order;

	auto start = chrono::high_resolution_clock::now();

	vector<vector<int>> layers = SortByLayers(graph);
	for (int i = 0; i < layers.size(); i++) {
		vector<int> layer = layers[i];
		for (int j = 0; j < layer.size(); j++) {
			if (visited[layer[j]] == 0) {
				dfs_without_recursion(layer[j], graph, parent, visited, topological_order);
			}
		}
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<long long, nano> duration = end - start;
	cout << "dfs time:" << double(duration.count()) / 1000000000 << "s" << endl;
	auto end_1 = chrono::high_resolution_clock::now();
	return topological_order;
}

vector<int> inverse_order(vector<int> topological_order) {
	vector<int> inverse_topological_order;
	for (int i = topological_order.size() - 1; i >= 0; i--) {
		inverse_topological_order.push_back(topological_order[i]);
	}
	return inverse_topological_order;

}

vector<int> inverse_DFS(vector<VertexNode>& parent, vector<VertexNode>& graph) {
	int n = graph.size();
	vector<int> visited(n, 0);
	vector<int> topological_order;

	auto start = chrono::high_resolution_clock::now();

	vector<vector<int>> layers = SortByLayers(graph);
	for (int i = 0; i < layers.size(); i++) {
		vector<int> layer = layers[i];
		for (int j = 0; j < layer.size(); j++) {
			if (visited[layer[j]] == 0) {
				dfs_without_recursion(layer[j], graph, parent, visited, topological_order);
			}
		}
	}
	topological_order = inverse_order(topological_order);
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<long long, nano> duration = end - start;
	cout << "inverse_dfs time:" << double(duration.count()) / 1000000000 << "s" << endl;
	auto end_1 = chrono::high_resolution_clock::now();
	return topological_order;
}

void early_release(int node, vector<VertexNode>& graph, vector<VertexNode>& parent, vector<int>& visited, vector<int>& topological_order) {
	visited[node] = 1;
	topological_order.push_back(node);
	for (int child : graph[node].adjacency_list) {
		for (int parent : parent[child].adjacency_list) {
			if (visited[parent] == 0) {
				visited[parent] = 1;
				topological_order.push_back(parent);
			}
		}
	}
}

vector<int> Quick_Choice(vector<VertexNode>& graph, vector<VertexNode>& parent) {
	int n = graph.size();
	vector<int> visited(n, 0);
	vector<int> topological_order;
	vector<vector<int>> layers = SortByLayers(graph);
	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < layers.size();i++) {
		vector<int> layer = layers[i];
		for (int j = 0; j < layer.size();j++) {
			int node = layer[j];
			if (visited[node] == 0) {
				early_release(node, graph, parent, visited, topological_order);
			}
		}
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<long long, nano> duration = end - start;
	cout << "quick choice time:" << double(duration.count()) / 1000000000 << "s" << endl;
	return topological_order;
}
vector<float> Weights(vector<VertexNode>& graph, vector<VertexNode>& parent) {
	int n = graph.size();
	vector<float> weights(n, 0.0f);
	for (int i = 0; i < n; i++) {
		for (int j : graph[i].adjacency_list) {
			weights[i] += graph[j].memory;
		}
		weights[i] += ALPHA * graph[i].memory;
	}
	return weights;
}

vector<int> Quicksort(vector<int>& adjacency_list, vector<float>& weights) {
	vector<pair<int, float>> node_value_pairs;
	for (int node : adjacency_list) {
		node_value_pairs.emplace_back(node, weights[node]);
	}
	sort(node_value_pairs.begin(), node_value_pairs.end(), [](pair<int, float>& a, pair<int, float>& b) {
		return a.second > b.second; 
		});

	vector<int> sorted_nodes;
	for (const auto& pair : node_value_pairs) {
		sorted_nodes.push_back(pair.first);
	}

	return sorted_nodes;
}

void early_release_v2(int node, vector<VertexNode>& graph, vector<VertexNode>& parent,
	vector<int>& visited, vector<int>& topological_order, vector<float>& in_and_out) {
	visited[node] = 1;
	topological_order.push_back(node);
	for (int child : graph[node].adjacency_list) {
		vector<int> parent_order = Quicksort(parent[child].adjacency_list, in_and_out);
		for (int i = 0; i < parent_order.size(); i++){
			if (visited[parent_order[i]] == 0) {
				visited[parent_order[i]] = 1;
				topological_order.push_back(parent_order[i]);
			}
		}
	}
}

vector<int> Quick_Choice_v2(vector<VertexNode>& graph, vector<VertexNode>& parent) {
	int n = graph.size();
	vector<int> visited(n, 0);
	vector<int> topological_order;
	vector<vector<int>> layers = SortByLayers(graph);
	vector<float> weights = Weights(graph, parent);
	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < layers.size(); i++) {
		vector<int> layer = layers[i];
		for (int j = 0; j < layer.size(); j++) {
			int node = layer[j];
			if (visited[node] == 0) {
				early_release_v2(node, graph, parent, visited, topological_order, weights);
			}
		}
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<long long, nano> duration = end - start;
	cout << "quickv2 choice time:" << double(duration.count()) / 1000000000 << "s" << endl;
	return topological_order;
}


//TOBEDONE

/* 
auto new_peak(vector<VertexNode>& graph, vector<VertexNode>& parent, 
	int node, float& peak_memory, vector<float>& used_memory,vector<int>& available_time) {
	float new_peak = peak_memory;
	int position;
	vector<int> need_to_remove;
	float new_memory;
	if (available_time[node] == 0) {
		new_memory = 0.0f;
	}
	else(new_memory = used_memory[available_time[node] - 1]);
	for (int i = available_time[node]; i < used_memory.size(); i++) {
		for (int j : parent[node].adjacency_list) {
			if (are_childs_available(j, graph, available_time)) {
				new_memory -= graph[j].memory;
			}
		}
	}
	return make_tuple(new_peak, position, need_to_remove);
}

vector<int> Insert(vector<int>& topological_order, int node, int position) {
	vector<int> new_topological_order;
	for (int i = 0; i < topological_order.size(); i++) {
		if (i == position) {
			new_topological_order.push_back(node);
		}
		new_topological_order.push_back(topological_order[i]);
	}
	return new_topological_order;
}

void greedy(vector<VertexNode>& graph, vector<VertexNode>& parent, unordered_set<int>& available, float& peak_memory, vector<float>& used_memory,
	vector<int>& topological_order, unordered_set<int>& waiting_for_release) {
	int position;
	int index = -1;
	int flag = 0;
	vector<int> need_to_remove;
	for (int i : available) {
		auto memory = new_peak(graph, parent,i, peak_memory, used_memory);
		float new_peak = get<0>(memory);
		if (new_peak < peak_memory||flag==0){
			peak_memory = new_peak;
			position = get<1>(memory);
			need_to_remove = get<2>(memory);
			index = i;
			flag = 1;
		}
	}
	topological_order = Insert(topological_order, index, position);
	available.erase(index);
	waiting_for_release.insert(index);
	for (int i : need_to_remove) {
		waiting_for_release.erase(i);
	}
	for (int i : graph[index].adjacency_list) {
		if (are_your_parents_all_visited(index, i, parent, topological_order)) {
			available.insert(i);
		}
	}
}


vector<int> Greedy(vector<VertexNode>& graph, vector<VertexNode>& parent) {
	int n = graph.size();
	vector<int> topological_order;
	unordered_set<int> available;
	unordered_set<int> waiting_for_release;
	vector<vector<int>> layers = SortByLayers(graph);
	for (int node : layers[0]) {
		available.insert(node);
	}
	auto start = chrono::high_resolution_clock::now();
	
	while (!available.empty()) {
		greedy();
	}


	auto end = chrono::high_resolution_clock::now();
	chrono::duration<long long, nano> duration = end - start;
	cout << "greedy time:" << double(duration.count()) / 1000000000 << "s" << endl;

	return topological_order;
}
*/

vector<int> Algorithm(vector<VertexNode>& graph,vector<VertexNode>& parent, string algorithm) {
	if (algorithm == "DFS") {
		return DFS(graph, parent);
	}
	else if(algorithm == "Quick"){
		return Quick_Choice(graph, parent);
	}
	else if (algorithm == "Inverse_DFS") {
		return inverse_DFS(graph, parent);
	}
	//else if (algorithm == "Greedy") {
	//	return Greedy(graph, parent);
	//}
	else if (algorithm == "Quick_v2") {
		return Quick_Choice_v2(graph, parent);
	}
}



