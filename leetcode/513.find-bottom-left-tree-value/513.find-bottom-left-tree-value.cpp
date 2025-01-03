/*
 * @lc app=leetcode id=513 lang=cpp
 *
 * [513] Find Bottom Left Tree Value
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
    struct Target {
        int depth;
        TreeNode *node;
        Target(int d, TreeNode *node) : depth(d), node(node) {}
    };
    void dfs(TreeNode *root, int depth, Target *t) {
        if (root->left == nullptr && root->right == nullptr) {
            if (t->depth < depth) {
                t->depth = depth;
                t->node = root;
            }
            return;
        }
        if (root->left != nullptr)
            dfs(root->left, depth + 1, t);
        if (root->right != nullptr)
            dfs(root->right, depth + 1, t);
    }

    int findBottomLeftValue(TreeNode *root) {
        Target t(0, root);
        dfs(root, 0, &t);
        return t.node->val;
    }
};
// @lc code=end
