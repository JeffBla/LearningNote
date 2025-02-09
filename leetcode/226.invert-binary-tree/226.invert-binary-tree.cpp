/*
 * @lc app=leetcode id=226 lang=cpp
 *
 * [226] Invert Binary Tree
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
    void _invertTree(TreeNode* root) {
        swap(root->left, root->right);
        if (root->left != nullptr)
            _invertTree(root->left);
        if (root->right != nullptr)
            _invertTree(root->right);
    }

    TreeNode* invertTree(TreeNode* root) {
        if (root != nullptr)
            _invertTree(root);
        return root;
    }
};
// @lc code=end
