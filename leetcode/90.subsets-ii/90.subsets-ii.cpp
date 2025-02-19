/*
 * @lc app=leetcode id=90 lang=cpp
 *
 * [90] Subsets II
 */

// @lc code=start
class Solution {
   public:
    vector<int> path;
    vector<bool> usage;
    int cnt = 0;
    bool backtracking(int n, int idx) {
        if (cnt >= n) {
            return true;
        } else if (path[idx] != 0) {
            return backtracking(n, idx + 1);
        }

        for (int i = n; i > 0; i--) {
            if (usage[i])
                continue;
            if (i != 1) {
                if ((idx + i) >= path.size())
                    continue;
                if (path[idx + i] != 0)
                    continue;
            }
            path[idx] = i;
            if (i != 1)
                path[idx + i] = i;
            usage[i] = true;
            cnt++;
            // print();
            if (backtracking(n, idx + 1))
                return true;
            cnt--;
            usage[i] = false;
            if (i != 1)
                path[idx + i] = 0;
            path[idx] = 0;
        }
        return false;
    }
    void print() {
        for (int i = 0; i < path.size(); i++) {
            cout << path[i] << " ";
        }
        cout << endl;
    }
    vector<int> constructDistancedSequence(int n) {
        usage.assign(n + 1, false);
        path.assign(n * 2 - 1, 0);
        backtracking(n, 0);
        return path;
    }
};
// @lc code=end
