/*
 * @lc app=leetcode id=77 lang=cpp
 *
 * [77] Combinations
 */

// @lc code=start
class Solution {
   public:
    vector<vector<int>> res;
    vector<int> path;
    void backtracking(int start_idx, int n, int k) {
        if (path.size() == k) {
            res.push_back(path);
            return;
        }
        for (int i = start_idx; i <= n - (k - path.size()) + 1; i++) {
            path.push_back(i);
            backtracking(i + 1, n, k);
            path.pop_back();
        }
    }
    vector<vector<int>> combine(int n, int k) {
        backtracking(1, n, k);
        return res;
    }
};
// @lc code=end
