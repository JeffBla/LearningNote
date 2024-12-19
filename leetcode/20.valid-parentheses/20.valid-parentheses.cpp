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
            if (s[i] == '(' || s[i] == '[' || s[i] == '{') {
                st.push(s[i]);
            } else {
                if (st.empty())
                    return false;
                target = st.top();
                if ((target == '(' && s[i] == ')') || (target == '[' && s[i] == ']') || (target == '{' && s[i] == '}'))
                    st.pop();
                else
                    return false;
            }
        }
        if (st.empty())
            return true;
        return false;
    }
};
// @lc code=end
