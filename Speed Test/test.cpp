#include<iostream>
#include<chrono>
#include<stack>
#include<vector>

using namespace std;

#define nanos 1000000000

int recursion_me(int n, int flag) {
	if (flag > 1000000) {
		return 1;
	}
	else {
		n *= 2;
		return recursion_me(n, flag + 1) + 1;
	}
}

int stack_test(int n) {
	stack<int> s;
	for (int i = 0; i < n; i++) {
		s.push(i);
	}
	return s.size();
}

stack<int> build_a_stack(int n) {
	stack<int> s;
	for (int i = 0; i < n; i++) {
		s.push(i);
	}
	return s;
}

vector<int> stack_to_vec(stack<int> s) {
	vector<int> v;
	while (!s.empty()) {
		v.push_back(s.top());
		s.pop();
	}
	return v;
}

int main() {
	/*
	auto start = chrono::high_resolution_clock::now();
	stack<int> s = build_a_stack(100000);
	auto end = chrono::high_resolution_clock::now();
	vector<int> v = stack_to_vec(s);
	auto end_1 = chrono::high_resolution_clock::now();
	chrono::duration<long long, nano> duration = end - start;
	cout << "build a stack costs" << double(duration.count()) / nanos << "s" << endl;
	chrono::duration<long long, nano> duration_1 = end_1 - end;
	cout << "trans costs" << double(duration_1.count()) / nanos << "s" << endl;
	*/
}