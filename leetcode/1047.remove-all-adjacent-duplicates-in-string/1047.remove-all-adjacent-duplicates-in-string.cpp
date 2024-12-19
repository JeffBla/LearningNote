/*
 * @lc app=leetcode id=1047 lang=cpp
 *
 * [1047] Remove All Adjacent Duplicates In String
 */

// @lc code=start
class Solution {
   public:
    string removeDuplicates(string S) {
        string result;
        for (char s : S) {
            if (result.empty() || result.back() != s) {
                result.push_back(s);
            } else {
                result.pop_back();
            }
        }
        return result;
    }
};
// @lc code=end
