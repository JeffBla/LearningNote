/*
 * @lc app=leetcode id=40 lang=cpp
 *
 * [40] Combination Sum II
 */

// @lc code=start
class Solution {
   public:
    vector<int> path;
    vector<vector<int>> res;
    int sum = 0;
    vector<bool> usage;
    void backtracking(vector<int>& candidates, int target, int s) {
        if (sum == target) {
            res.push_back(path);
            return;
        } else if (sum > target) {
            return;
        }

        for (int i = s; i < candidates.size(); i++) {
            int candi = candidates[i];

            if (i > 0 && candidates[i - 1] == candidates[i] && !usage[i - 1])
                continue;

            usage[i] = true;
            sum += candi;
            path.push_back(candi);
            backtracking(candidates, target, i + 1);
            path.pop_back();
            sum -= candi;
            usage[i] = false;
        }
    }
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        usage.assign(candidates.size(), false);
        backtracking(candidates, target, 0);
        return res;
    }
};
// @lc code=end
