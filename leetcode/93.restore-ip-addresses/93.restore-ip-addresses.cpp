/*
 * @lc app=leetcode id=93 lang=cpp
 *
 * [93] Restore IP Addresses
 */

// @lc code=start
class Solution {
   public:
    vector<string> path;
    vector<string> res;
    string num;
    bool isIPNum(string s) {
        if (s[0] != '0' && s.size() <= 3) {
            int real = stoi(s);
            if (real >= 0 && real <= 255)
                return true;
        } else if (s.size() == 1) {
            return true;
        }
        return false;
    }
    void backtracking(string s, int s_idx) {
        if (path.size() == 3) {
            num.assign(s, s_idx, s.size() - s_idx);
            if (!isIPNum(num))
                return;

            string out = path[0];
            for (int i = 1; i < path.size(); i++) {
                out += '.' + path[i];
            }
            out += '.' + num;
            res.push_back(out);
            return;
        }

        for (int i = 1; i + s_idx < s.size(); i++) {
            num.assign(s, s_idx, i);
            if (!isIPNum(num))
                continue;
            path.push_back(num);
            backtracking(s, s_idx + i);
            path.pop_back();
        }
    }
    vector<string> restoreIpAddresses(string s) {
        backtracking(s, 0);
        return res;
    }
};
// @lc code=end
