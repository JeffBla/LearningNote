/*
 * @lc app=leetcode id=654 lang=cpp
 *
 * [654] Maximum Binary Tree
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
    TreeNode* _buildTree(vector<int>& nums, int start, int back) {
        int max_id = start;
        for (int i = start; i <= back; i++) {
            if (nums[max_id] < nums[i])
                max_id = i;
        }

        auto root = new TreeNode(nums[max_id]);
        if (max_id <= start)
            root->left = nullptr;
        else
            root->left = _buildTree(nums, start, max_id - 1);
        if (max_id >= back)
            root->right = nullptr;
        else
            root->right = _buildTree(nums, max_id + 1, back);
        return root;
    }
    TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
        return _buildTree(nums, 0, nums.size() - 1);
    }
};
// @lc code=end
