/*
 * @lc app=leetcode id=15 lang=cpp
 *
 * [15] 3Sum
 */

// @lc code=start
class Solution {
   public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;

        sort(nums.begin(), nums.end());
        if (nums[0] > 0)
            return {};
        for (int i = 0; i < nums.size(); i++) {
            if (i > 0 && nums[i] == nums[i - 1])
                continue;
            int l = i + 1, r = nums.size() - 1;
            int total;
            while (r > l) {
                total = nums[i] + nums[l] + nums[r];
                if (total > 0) {
                    r--;
                } else if (total < 0) {
                    l++;
                } else {
                    result.push_back({nums[i], nums[l], nums[r]});
                    r--;
                    l++;
                    while (r > l && nums[r + 1] == nums[r]) r--;
                    while (r > l && nums[l - 1] == nums[l]) l++;
                }
            }
        }
        return result;
    }
};
// @lc code=end
