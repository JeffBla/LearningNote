/*
 * @lc app=leetcode id=131 lang=cpp
 *
 * [131] Palindrome Partitioning
 */

// @lc code=start
class Solution {
   public:
    vector<string> path;
    vector<vector<string>> res;
    string curr;
    bool isPalindrome(string s) {
        for (int i = 0, j = s.size() - 1; i < j; i++, j--) {
            if (s[i] != s[j])
                return false;
        }
        return true;
    }
    void backtracking(string s, int s_idx) {
        if (s_idx >= s.size()) {
            res.push_back(path);
            return;
        }

        for (int i = 1; i + s_idx <= s.size(); i++) {
            curr.assign(s, s_idx, i);
            if (!isPalindrome(curr)) {
                continue;
            }
            path.push_back(curr);
            backtracking(s, s_idx + i);
            path.pop_back();
        }
    }
    vector<vector<string>> partition(string s) {
        backtracking(s, 0);
        return res;
    }
};
// @lc code=end
