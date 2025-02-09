/*
 * @lc app=leetcode id=110 lang=cpp
 *
 * [110] Balanced Binary Tree
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
   public:
    int dfs(TreeNode *root, bool *isBalanced) {
        int h, r_h = 0, l_h = 0;
        if (!*isBalanced)
            return 0;
        if (root->left != nullptr)
            l_h = dfs(root->left, isBalanced) + 1;
        if (root->right != nullptr)
            r_h = dfs(root->right, isBalanced) + 1;
        h = max(r_h, l_h);
        if (*isBalanced)
            *isBalanced = abs(r_h - l_h) <= 1;
        return h;
    }
    bool isBalanced(TreeNode *root) {
        bool isBalanced = true;
        if (root)
            dfs(root, &isBalanced);
        return isBalanced;
    }
};
// @lc code=end
