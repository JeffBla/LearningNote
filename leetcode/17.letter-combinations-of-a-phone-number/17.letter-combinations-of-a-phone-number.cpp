/*
 * @lc app=leetcode id=17 lang=cpp
 *
 * [17] Letter Combinations of a Phone Number
 */

// @lc code=start
class Solution {
   public:
    string path;
    vector<string> res;
    map<char, vector<char>> tb{
        {'2', {'a', 'b', 'c'}},
        {'3', {'d', 'e', 'f'}},
        {'4', {'g', 'h', 'i'}},
        {'5', {'j', 'k', 'l'}},
        {'6', {'m', 'n', 'o'}},
        {'7', {'p', 'q', 'r', 's'}},
        {'8', {'t', 'u', 'v'}},
        {'9', {'w', 'x', 'y', 'z'}}};
    void backtracking(string digits, int s) {
        if (digits.size() == 0)
            return;
        if (path.size() == digits.size()) {
            res.push_back(path);
            return;
        }
        for (char ch : tb[digits[s]]) {
            path.push_back(ch);
            backtracking(digits, s + 1);
            path.pop_back();
        }
    }
    vector<string> letterCombinations(string digits) {
        backtracking(digits, 0);
        return res;
    }
};
// @lc code=end
