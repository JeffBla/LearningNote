/*
 * @lc app=leetcode id=239 lang=cpp
 *
 * [239] Sliding Window Maximum
 */

// @lc code=start
class Solution {
   public:
    class mQueue {
       public:
        deque<int> q;
        int push(int val) {
            while (!q.empty() && val > q.back()) {
                q.pop_back();
            }
            q.push_back(val);
            return val;
        }
        int pop(int val) {
            int out = q.front();
            if (out == val)
                q.pop_front();
            return out;
        }
        int front() {
            return q.front();
        }
    };
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector<int> out;
        mQueue q;
        for (int i = 0; i < nums.size(); i++) {
            if (i >= k) {
                out.push_back(q.front());
                q.pop(nums[i - k]);
            }
            q.push(nums[i]);
        }
        out.push_back(q.front());
        return out;
    }
};
// @lc code=end
