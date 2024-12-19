/*
 * @lc app=leetcode id=20 lang=cpp
 *
 * [20] Valid Parentheses
 */

// @lc code=start
class Solution {
   public:
    stack<char> st;
    bool isValid(string s) {
        char target;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == '(')
                st.push(')');
            else if (s[i] == '[')
                st.push(']');
            else if (s[i] == '{')
                st.push('}');
            else if (st.empty() || st.top() != s[i])
                return false;
            else
                st.pop();
        }
        return st.empty();
    }
};
// @lc code=end
