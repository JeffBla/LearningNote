/*
 * @lc app=leetcode id=216 lang=cpp
 *
 * [216] Combination Sum III
 */

// @lc code=start
class Solution {
   public:
    vector<int> path;
    vector<vector<int>> res;
    void backtracking(int start, int k, int n) {
        int sum = 0;
        if (path.size() == k) {
            for (auto v : path) {
                sum += v;
            }
            if (sum == n) {
                res.push_back(path);
                return;
            } else if (sum > n) {
                return;
            }
        }

        for (int i = start; i <= 9; i++) {
            path.push_back(i);
            backtracking(i + 1, k, n);
            path.pop_back();
        }
    }
    vector<vector<int>> combinationSum3(int k, int n) {
        backtracking(1, k, n);
        return res;
    }
};
// @lc code=end
