/*
 * @lc app=leetcode id=459 lang=cpp
 *
 * [459] Repeated Substring Pattern
 */

// @lc code=start
class Solution {
   public:
    bool repeatedSubstringPattern(string s) {
        int sub_i = 0, end = 1;
        for (int i = 1; i < s.size(); i++) {
            // substr exceeds the max possible len
            if (end > s.size() / 2)
                return false;
            if (s[i] != s[sub_i]) {
                sub_i = 0;
                end = i + 1;
            } else {
                sub_i = (sub_i + 1) % end;
            }
        }
        if (end == s.size() || sub_i != 0)
            return false;
        else
            return true;
    }
};
// @lc code=end
