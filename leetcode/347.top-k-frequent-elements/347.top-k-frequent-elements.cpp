/*
 * @lc app=leetcode id=347 lang=cpp
 *
 * [347] Top K Frequent Elements
 */

// @lc code=start
class Solution {
   public:
    unordered_map<int, int> cnter;
    struct mComparison {
        bool operator()(const pair<int, int> &lhs, const pair<int, int> &rhs) {
            return lhs.second > rhs.second;
        }
    };

    vector<int> topKFrequent(vector<int> &nums, int k) {
        for (int num : nums) {
            cnter[num]++;
        }

        priority_queue<pair<int, int>, vector<pair<int, int>>, mComparison> q;
        for (auto it = cnter.begin(); it != cnter.end(); it++) {
            q.push(*it);
            if (q.size() > k) {
                q.pop();
            }
        }

        vector<int> ans(k);
        for (int i = k - 1; i >= 0; i--) {
            ans[i] = q.top().first;
            q.pop();
        }
        return ans;
    }
};
// @lc code=end
