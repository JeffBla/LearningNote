/*
 * @lc app=leetcode id=150 lang=cpp
 *
 * [150] Evaluate Reverse Polish Notation
 */

// @lc code=start
class Solution {
   public:
    stack<int> st;
    int evalRPN(vector<string>& tokens) {
        int num1, num2, res;
        for (string str : tokens) {
            if (str == "+" || str == "-" || str == "*" || str == "/") {
                num2 = st.top();
                st.pop();
                num1 = st.top();
                st.pop();
                if (str == "+") {
                    res = num1 + num2;
                } else if (str == "-") {
                    res = num1 - num2;
                } else if (str == "*") {
                    res = num1 * num2;
                } else if (str == "/") {
                    res = num1 / num2;
                }
                st.push(res);
                cout << res << " ";
            } else {
                st.push(stoi(str));
                cout << str << " ";
            }
        }
        return st.top();
    }
};
// @lc code=end
