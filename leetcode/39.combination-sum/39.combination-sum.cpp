/*
 * @lc app=leetcode id=39 lang=cpp
 *
 * [39] Combination Sum
 */

// @lc code=start
class Solution {
   public:
    vector<int> path;
    vector<vector<int>> res;
    int sum = 0;
    void backtracking(vector<int>& candidates, int target, int s) {
        if (sum == target) {
            res.push_back(path);
            return;
        }

        for (int i = s; i < candidates.size() && sum + candidates[i] <= target; i++) {
            sum += candidates[i];
            path.push_back(candidates[i]);
            backtracking(candidates, target, i);
            path.pop_back();
            sum -= candidates[i];
        }
    }
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        backtracking(candidates, target, 0);
        return res;
    }
};
// @lc code=end
