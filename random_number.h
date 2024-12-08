#pragma once	
#include<random>
using namespace std;
mt19937 gen(3304);

double generateRandomFloat(double min = 0.1, double max = 10.0) {
	uniform_real_distribution<> dist(min, max);
	return dist(gen);
}

int generateRandomInt(int min, int max) {
	uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

double generateNormalRandomFloat(double mean = 5.0, double stddev = 1.0) {
	normal_distribution<> dist(mean, stddev);
	if (dist(gen) < 0) {
		return generateNormalRandomFloat(mean, stddev);
	}
	return dist(gen);
}

