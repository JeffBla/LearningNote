/*
 * @lc app=leetcode id=501 lang=cpp
 *
 * [501] Find Mode in Binary Search Tree
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
    int maxCount = 0;
    int count = 0;
    TreeNode *pre = nullptr;
    vector<int> res;
    void searchNode(TreeNode *root) {
        if (root == nullptr)
            return;

        searchNode(root->left);

        if (pre == nullptr) {
            count = 1;
        } else if (pre->val == root->val) {
            count++;
        } else {
            count = 1;
        }
        if (count > maxCount) {
            maxCount = count;
            res.clear();
            res.push_back(root->val);
        } else if (count == maxCount) {
            res.push_back(root->val);
        }
        pre = root;

        searchNode(root->right);
    }

   public:
    vector<int> findMode(TreeNode *root) {
        searchNode(root);
        return res;
    }
};
// @lc code=end
