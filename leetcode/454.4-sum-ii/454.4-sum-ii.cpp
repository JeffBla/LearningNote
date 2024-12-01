/*
 * @lc app=leetcode id=454 lang=cpp
 *
 * [454] 4Sum II
 */

// @lc code=start
class Solution {
   public:
    int fourSumCount(vector<int>& nums1, vector<int>& nums2, vector<int>& nums3, vector<int>& nums4) {
        vector<int> nums1_2;
        for (int i = 0; i < nums1.size(); i++) {
            for (int j = 0; j < nums2.size(); j++) {
                nums1_2.push_back(nums1[i] + nums2[j]);
            }
        }

        vector<int> nums3_4;
        for (int i = 0; i < nums3.size(); i++) {
            for (int j = 0; j < nums4.size(); j++) {
                nums3_4.push_back(nums3[i] + nums4[j]);
            }
        }

        int cnt = 0;
        for (int i = 0; i < nums1_2.size(); i++) {
            for (int j = 0; j < nums3_4.size(); j++) {
                if (nums1_2[i] + nums3_4[j] == 0)
                    cnt++;
            }
        }

        return cnt;
    }
};
// @lc code=end
