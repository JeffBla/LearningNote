/*
 * @lc app=leetcode id=239 lang=cpp
 *
 * [239] Sliding Window Maximum
 */

// @lc code=start
class Solution {
   public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        multiset<int> st;
        vector<int> ans;
        for (int i = 0; i < nums.size(); i++) {
            if (i >= k) {
                ans.push_back(*st.rbegin());
                st.erase(st.find(nums[i - k]));
            }
            st.insert(nums[i]);
        }
        ans.push_back(*st.rbegin());
        return ans;
    }
};
// @lc code=end
