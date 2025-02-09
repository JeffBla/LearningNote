/*
 * @lc app=leetcode id=101 lang=cpp
 *
 * [101] Symmetric Tree
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
    bool dfs(TreeNode *right, TreeNode *left) {
        bool res = true;
        if (right->val != left->val)
            return false;
        if (right->right != nullptr) {
            if (left->left != nullptr)
                res = dfs(right->right, left->left);
            else
                return false;
        } else {
            if (left->left != nullptr)
                return false;
        }
        if (!res)
            return false;
        if (right->left != nullptr) {
            if (left->right != nullptr)
                res = dfs(right->left, left->right);
            else
                return false;
        } else {
            if (left->right != nullptr)
                return false;
        }
        return res;
    }

    bool isSymmetric(TreeNode *root) {
        if (root != nullptr) {
            if (root->right != nullptr) {
                if (root->left != nullptr)
                    return dfs(root->right, root->left);
                return false;
            } else {
                if (root->left != nullptr)
                    return false;
            }
        }
        return true;
    }
};
// @lc code=end
