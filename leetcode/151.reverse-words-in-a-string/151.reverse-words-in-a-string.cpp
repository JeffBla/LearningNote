/*
 * @lc app=leetcode id=151 lang=cpp
 *
 * [151] Reverse Words in a String
 */

// @lc code=start
class Solution {
   public:
    void reverse(string &s, int start, int end) {
        for (int i = start, j = end - 1; i < j; i++, j--) {
            swap(s[i], s[j]);
        }
    }

    void removeExtraSpace(string &str) {
        int s = 0;
        for (int f = 0; f < str.size(); f++) {
            if (str[f] != ' ' || (f > 0 && str[f] == ' ' && str[f - 1] != ' ')) {
                str[s++] = str[f];
            }
        }
        if (str[s - 1] == ' ')
            str.resize(s - 1);
        else
            str.resize(s);
    }

    string reverseWords(string s) {
        removeExtraSpace(s);
        reverse(s, 0, s.size());
        for (int i = 0, slow = 0; i <= s.size(); i++) {
            if (s[i] == ' ' || i == s.size()) {
                reverse(s, slow, i);
                slow = i + 1;
            }
        }
        return s;
    }
};

// @lc code=end
