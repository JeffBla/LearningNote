/*
 * @lc app=leetcode id=36 lang=cpp
 *
 * [36] Valid Sudoku
 */

// @lc code=start
class Solution {
   public:
    bool isValidSudoku(vector<vector<char>>& board) {
        unordered_set<string> st;
        string t1, t2, t3;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] != '.') {
                    t1 = to_string(i) + "r" + board[i][j];
                    t2 = to_string(j) + "c" + board[i][j];
                    t3 = to_string(i / 3) + to_string(j / 3) + 'b' + board[i][j];
                    if (st.count(t1) || st.count(t2) || st.count(t3))
                        return false;
                    st.insert(t1);
                    st.insert(t2);
                    st.insert(t3);
                }
            }
        }
        return true;
    }
};
// @lc code=end
