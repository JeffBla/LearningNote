/*
 * @lc app=leetcode id=450 lang=cpp
 *
 * [450] Delete Node in a BST
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
    TreeNode* deleteNode(TreeNode* root, int key) {
        if (root == nullptr)
            return root;
        if (root->val > key) {
            root->left = deleteNode(root->left, key);
        } else if (root->val < key) {
            root->right = deleteNode(root->right, key);
        } else {
            if (root->left == nullptr && root->right == nullptr) {
                delete root;
                return nullptr;
            } else if (root->left == nullptr) {
                auto remain_node = root->right;
                delete root;
                return remain_node;
            } else if (root->right == nullptr) {
                auto remain_node = root->left;
                delete root;
                return remain_node;
            } else {
                auto pRight = &root->right;
                while ((*pRight)->left != nullptr)
                    pRight = &((*pRight)->left);
                swap((*pRight)->val, root->val);
                *pRight = deleteNode(*pRight, key);
                return root;
            }
        }
        return root;
    }
};
// @lc code=end
