/*
 * @lc app=leetcode id=98 lang=cpp
 *
 * [98] Validate Binary Search Tree
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
    void dfs(TreeNode *root, bool *isValid, int64_t *curr_max) {
        if (root == nullptr)
            return;
        if (*isValid)
            dfs(root->left, isValid, curr_max);
        if (root->val <= *curr_max)
            *isValid = false;
        else
            *curr_max = root->val;
        if (*isValid)
            dfs(root->right, isValid, curr_max);
    }

    bool isValidBST(TreeNode *root) {
        bool isValid = true;
        int64_t curr_max = -pow(2, 32);
        dfs(root, &isValid, &curr_max);
        return isValid;
    }
};
// @lc code=end
