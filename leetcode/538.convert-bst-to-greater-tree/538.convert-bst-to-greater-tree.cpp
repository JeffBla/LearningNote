/*
 * @lc app=leetcode id=538 lang=cpp
 *
 * [538] Convert BST to Greater Tree
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
    int cnt = 0;
    TreeNode* convertBST(TreeNode* root) {
        if (!root)
            return root;

        convertBST(root->right);

        cnt += root->val;
        root->val = cnt;

        convertBST(root->left);
        return root;
    }
};
// @lc code=end
