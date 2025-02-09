/*
 * @lc app=leetcode id=235 lang=cpp
 *
 * [235] Lowest Common Ancestor of a Binary Search Tree
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */

class Solution {
   public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        int val_p, val_q;
        if (p->val < q->val) {
            val_p = p->val;
            val_q = q->val;
        } else {
            val_p = q->val;
            val_q = p->val;
        }

        if (root == nullptr)
            return root;
        if (root->val >= val_p && root->val <= val_q) {
            return root;
        } else if (root->val > val_q) {
            return lowestCommonAncestor(root->left, p, q);
        } else {
            return lowestCommonAncestor(root->right, p, q);
        }
    }
};
// @lc code=end
