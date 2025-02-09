/*
 * @lc app=leetcode id=669 lang=cpp
 *
 * [669] Trim a Binary Search Tree
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
    TreeNode* trimLeft(TreeNode* root, int low) {
        decltype(root) curr = root, prev = nullptr;
        while (curr) {
            if (curr->val < low) {
                if (prev) {
                    prev->left = curr->right;
                } else {
                    root = curr->right;
                }
                curr = curr->right;
            } else {
                prev = curr;
                curr = curr->left;
            }
        }
        return root;
    }
    TreeNode* trimRigh(TreeNode* root, int high) {
        decltype(root) curr = root, prev = nullptr;
        while (curr) {
            if (curr->val > high) {
                if (prev) {
                    prev->right = curr->left;
                } else {
                    root = curr->left;
                }
                curr = curr->left;
            } else {
                prev = curr;
                curr = curr->right;
            }
        }
        return root;
    }
    TreeNode* trimBST(TreeNode* root, int low, int high) {
        root = trimLeft(root, low);
        root = trimRigh(root, high);
        return root;
    }
};
// @lc code=end
