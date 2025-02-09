/*
 * @lc app=leetcode id=236 lang=cpp
 *
 * [236] Lowest Common Ancestor of a Binary Tree
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
    bool isFound = false;
    TreeNode* target = nullptr;
    bool _findNode(TreeNode* root, TreeNode* p, TreeNode* q) {
        bool isFoundLNode = false, isFoundRNode = false, isFoundMNode = false;
        if (root == nullptr)
            return false;
        isFoundLNode = _findNode(root->left, p, q);

        isFoundRNode = _findNode(root->right, p, q);

        if (isFoundLNode && isFoundRNode) {
            isFound = true;
            target = root;
            return true;
        }

        isFoundMNode = (root == p) || (root == q) ? true : false;
        if (isFoundMNode) {
            if (isFoundLNode || isFoundRNode) {
                isFound = true;
                target = root;
                return true;
            }
        }

        return isFoundMNode || isFoundLNode || isFoundRNode;
    }
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        _findNode(root, p, q);
        return target;
    }
};
// @lc code=end
