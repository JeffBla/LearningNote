/*
 * @lc app=leetcode id=459 lang=cpp
 *
 * [459] Repeated Substring Pattern
 */

// @lc code=start
class Solution {
   public:
    // KMP algo
    void buildNext(vector<int> &next, string &s) {
        int i = 0;
        next[0] = 0;
        for (int j = 1; j < s.size(); j++) {
            while (i > 0 && s[i] != s[j])
                i = next[i - 1];
            if (s[i] == s[j])
                i++;
            next[j] = i;
        }
    }
    bool repeatedSubstringPattern(string s) {
        vector<int> next(s.size());
        buildNext(next, s);
        int sub_len = next[s.size() - 1], sub_i = 0;
        if (sub_len == 0)
            return false;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] != s[sub_i])
                return false;
            sub_i = (sub_i + 1) % sub_len;
        }
        return true;
    }
};
// @lc code=end
