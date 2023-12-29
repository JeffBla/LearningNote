#include <omp.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

using namespace std;

int n;
map<int, vector<pair<int, int>>> weight;  // i -> j: i, cost, j

void GetInput() {
    char filename[50];
    scanf("%s", filename);
    ifstream inFile(filename);

    int num;
    inFile >> n;  // i j c
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inFile >> num;
            weight[i].emplace_back(num, j);
        }
    }
}

void Dijkstra(vector<int> &dist) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> min_heap;

    dist[0] = 0;
    auto adj_0 = weight[0];

    for (int i = 0; i < adj_0.size(); i++) {
        dist[adj_0[i].second] = dist[0] + adj_0[i].first;
        min_heap.emplace(dist[adj_0[i].second], adj_0[i].second);
    }

    for (int i = 0; i < n - 1; i++) {
        int curr;
        int curr_w;
        curr = min_heap.top().second;
        curr_w = min_heap.top().first;
        min_heap.pop();

        auto adj_curr = weight[curr];
        for (int i = 0; i < adj_curr.size(); i++) {
            if (dist[curr] + adj_curr[i].first < dist[adj_curr[i].second]) {
                dist[adj_curr[i].second] = dist[curr] + adj_curr[i].first;
                min_heap.emplace(dist[adj_curr[i].second], adj_curr[i].second);
            }
        }
    }
}

int main() {
    GetInput();

    vector<int> dist(n, INT32_MAX);
    Dijkstra(dist);

    for (int i = 0; i < n; i++) {
        cout << dist[i] << " ";
    }
    return 0;
}
