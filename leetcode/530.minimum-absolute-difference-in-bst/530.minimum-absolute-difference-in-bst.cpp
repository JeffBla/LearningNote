/*
 * @lc app=leetcode id=530 lang=cpp
 *
 * [530] Minimum Absolute Difference in BST
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
    void dfs(TreeNode *root, int *min, int **former) {
        if (root == nullptr)
            return;
        dfs(root->left, min, former);
        if (*former == nullptr)
            *former = new int();
        else {
            int curr = abs(root->val - **former);
            *min = curr < *min ? curr : *min;
        }
        **former = root->val;

        dfs(root->right, min, former);
    }
    int getMinimumDifference(TreeNode *root) {
        int min = 1 << 30;
        int **former = new int *();
        dfs(root, &min, former);
        return min;
    }
};
// @lc code=end
