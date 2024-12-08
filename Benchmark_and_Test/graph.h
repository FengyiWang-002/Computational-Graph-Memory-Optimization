#pragma once
#include<string>
#include<vector>

using namespace std;

typedef struct VertexNode {
	float memory;
	vector<int> adjacency_list;
} VextexNode;

bool are_your_parents_all_visited(int node, int child, vector<VertexNode>& parent, vector<int>& visited) {
	for (int parent : parent[child].adjacency_list) {
		if (visited[parent] == 0) {
			return false;
		}
	}
	return true;
}



