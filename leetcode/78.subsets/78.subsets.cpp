/*
 * @lc app=leetcode id=78 lang=cpp
 *
 * [78] Subsets
 */

// @lc code=start
class Solution {
   public:
    vector<int> path;
    vector<vector<int>> res;
    void backtracking(vector<int>& nums, int s) {
        res.push_back(path);
        for (int i = s; i < nums.size(); i++) {
            path.push_back(nums[i]);
            backtracking(nums, i + 1);
            path.pop_back();
        }
    }
    vector<vector<int>> subsets(vector<int>& nums) {
        backtracking(nums, 0);
        return res;
    }
};
// @lc code=end
