/*
 * @lc app=leetcode id=106 lang=cpp
 *
 * [106] Construct Binary Tree from Inorder and Postorder Traversal
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
    TreeNode* _buildTree(vector<int>& inorder, int start, int back, int root_idx_in,
                         vector<int>& postorder, int root_idx_post) {
        auto root = new TreeNode(inorder[root_idx_in]);

        int new_root_idx_in_r, new_root_idx_post_r = root_idx_post - 1, len_r = back - root_idx_in;
        int new_root_idx_in_l, new_root_idx_post_l = new_root_idx_post_r - len_r;

        // find l root
        int i = start;
        for (; i <= root_idx_in - 1; i++) {
            if (inorder[i] == postorder[new_root_idx_post_l]) {
                new_root_idx_in_l = i;
                break;
            }
        }
        if (i > root_idx_in - 1)
            root->left = nullptr;
        else
            root->left = _buildTree(inorder, start, root_idx_in - 1, new_root_idx_in_l, postorder, new_root_idx_post_l);

        // fine r root
        i = root_idx_in + 1;
        for (; i <= back; i++) {
            if (inorder[i] == postorder[new_root_idx_post_r]) {
                new_root_idx_in_r = i;
                break;
            }
        }
        if (i > back)
            root->right = nullptr;
        else
            root->right = _buildTree(inorder, root_idx_in + 1, back, new_root_idx_in_r, postorder, new_root_idx_post_r);
        return root;
    }

    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        int root_idx_in, root_idx_post = postorder.size() - 1;
        // find root
        for (int i = 0; i < inorder.size(); i++) {
            if (inorder[i] == postorder[root_idx_post]) {
                root_idx_in = i;
                break;
            }
        }
        return _buildTree(inorder, 0, inorder.size() - 1, root_idx_in, postorder, root_idx_post);
    }
};
// @lc code=end
