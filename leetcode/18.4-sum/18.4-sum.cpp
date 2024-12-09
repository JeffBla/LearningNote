/*
 * @lc app=leetcode id=18 lang=cpp
 *
 * [18] 4Sum
 */

// @lc code=start
class Solution {
   public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> result;
        for (int d = nums.size() - 1; d >= 0; d--) {
            if (d < nums.size() - 1 && nums[d] == nums[d + 1])
                continue;
            for (int a = 0; a < d - 2; a++) {
                if (a > 0 && nums[a] == nums[a - 1])
                    continue;
                int l = a + 1, r = d - 1;
                long long total;
                while (r > l) {
                    total = nums[a] + nums[l];
                    total += nums[r] + nums[d];
                    // cout << a << " " << l << " " << r << " " << d << endl;
                    // cout << nums[a] << nums[l] << nums[r] << nums[d] << endl;
                    if (total > target) {
                        r--;
                    } else if (total < target) {
                        l++;
                    } else {
                        result.push_back({nums[a], nums[l], nums[r], nums[d]});
                        while (r > l && nums[r - 1] == nums[r]) r--;
                        while (r > l && nums[l + 1] == nums[l]) l++;

                        r--;
                        l++;
                    }
                }
            }
        }
        return result;
    }
};
// @lc code=end
