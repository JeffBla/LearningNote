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
    dist[0] = 0;
    auto adj_0 = weight[0];
    int global_curr, global_curr_w;
    omp_lock_t global_weight_lock;
    omp_init_lock(&global_weight_lock);
#pragma omp parallel
    {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> min_heap;

#pragma omp for
        for (int i = 0; i < adj_0.size(); i++) {
            dist[adj_0[i].second] = dist[0] + adj_0[i].first;
            min_heap.emplace(dist[adj_0[i].second], adj_0[i].second);
        }

        for (int i = 0; i < n - 1; i++) {
#pragma omp single
            global_curr = INT32_MAX, global_curr_w = INT32_MAX;

            int curr;
            int curr_w;
            if (!min_heap.empty()) {
                do {
                    curr = min_heap.top().second;
                    curr_w = min_heap.top().first;
                    if (dist[curr] < curr_w) {
                        min_heap.pop();
                        if (min_heap.empty()) {
                            curr = INT32_MAX;
                            curr_w = INT32_MAX;
                            break;
                        }
                    } else {
                        break;
                    }
                } while (1);
            } else {
                curr = INT32_MAX;
                curr_w = INT32_MAX;
            }

            if (global_curr_w > curr_w) {
                omp_set_lock(&global_weight_lock);
                if (global_curr_w > curr_w) {
                    global_curr = curr;
                    global_curr_w = curr_w;
                }
                omp_unset_lock(&global_weight_lock);
            }

#pragma omp barrier
            if (global_curr == curr) {
                dist[global_curr] = global_curr_w;
                min_heap.pop();
            }
#pragma omp barrier

            auto adj_curr = weight[global_curr];
#pragma omp for
            for (int i = 0; i < adj_curr.size(); i++) {
                if (dist[global_curr] + adj_curr[i].first < dist[adj_curr[i].second]) {
                    dist[adj_curr[i].second] = dist[global_curr] + adj_curr[i].first;
                    min_heap.emplace(dist[adj_curr[i].second], adj_curr[i].second);
                }
            }
        }
    }
    omp_destroy_lock(&global_weight_lock);
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
