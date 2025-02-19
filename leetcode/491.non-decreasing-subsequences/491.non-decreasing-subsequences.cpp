/*
 * @lc app=leetcode id=491 lang=cpp
 *
 * [491] Non-decreasing Subsequences
 */

// @lc code=start
class Solution {
   public:
    vector<int> path;
    set<vector<int>> res;
    vector<int> usage;
    void backtracking(vector<int>& nums, int s) {
        for (int i = s; i < nums.size(); i++) {
            if (!path.empty() && path.back() > nums[i])
                continue;
            path.push_back(nums[i]);
            usage[i] = true;
            if (path.size() >= 2)
                res.insert(path);
            backtracking(nums, i + 1);
            usage[i] = false;
            path.pop_back();
        }
    }
    vector<vector<int>> findSubsequences(vector<int>& nums) {
        usage.assign(nums.size(), false);
        backtracking(nums, 0);
        return vector<vector<int>>(res.begin(), res.end());
    }
};
// @lc code=end
