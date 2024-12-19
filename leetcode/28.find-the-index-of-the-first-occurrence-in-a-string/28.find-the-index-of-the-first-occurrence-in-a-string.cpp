/*
 * @lc app=leetcode id=28 lang=cpp
 *
 * [28] Find the Index of the First Occurrence in a String
 */

// @lc code=start
class Solution {
   public:
    vector<int> next;

    void buildNext(vector<int> &next, string &substr) {
        int i = 0;
        next[0] = 0;
        for (int j = 1; j < substr.size(); j++) {
            while (i > 0 && substr[j] != substr[i])
                i = next[i - 1];
            if (substr[i] == substr[j])
                i++;
            next[j] = i;
        }
    }

    void printNext(vector<int> &next) {
        for (int e : next) {
            cout << e << " ";
        }
        cout << endl;
    }

    int strStr(string haystack, string needle) {
        vector<int> next(needle.size());
        buildNext(next, needle);

        // printNext(next);

        int i = 0;
        for (int j = 0; j < haystack.size(); j++) {
            while (i > 0 && haystack[j] != needle[i]) {
                i = next[i - 1];
            }

            if (haystack[j] == needle[i])
                i++;

            if (i >= needle.size()) {
                return j - needle.size() + 1;
            }
        }
        return -1;
    }
};
// @lc code=end
