#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

class SegmentTreeNode {
    int sum;
    int start;
    int end;
    SegmentTreeNode* left;
    SegmentTreeNode* right;

    friend class NumArray;
    public:
        SegmentTreeNode(int sum, int start, int end, 
        SegmentTreeNode* left = nullptr, SegmentTreeNode* right = nullptr):
            sum(sum), start(start), end(end), left(left), right(right) {}
        
        SegmentTreeNode(const SegmentTreeNode&) = delete;
        SegmentTreeNode& operator=(const SegmentTreeNode&) = delete;
        
        ~SegmentTreeNode() {
            delete left;
            delete right;
            left = right = nullptr;
        }
        
        int get_sum() {
            return sum;
        }
        
        void set_sum(int new_sum) {
            this->sum = new_sum;
        }
    
};

class NumArray {
private: 
    std::unique_ptr<SegmentTreeNode> root_;
    vector<int> nums_;

    SegmentTreeNode* build_tree(vector<int>& nums, int start, int end) {
        if (start == end) {
            auto new_node = new SegmentTreeNode(nums[start], start, end);
            return new_node;
        }
        int mid = (start + end) / 2;
        auto left_subtree = build_tree(nums, start, mid);
        auto right_subtree = build_tree(nums, mid + 1, end);
        auto node_sum = left_subtree->sum + right_subtree->sum;
        auto new_node = new SegmentTreeNode(node_sum, start, end, left_subtree, right_subtree);
        return new_node;
    }
    
    void update_tree(SegmentTreeNode* root, int index, int val) {
        if (root->start == index and root->end == index) {
            root->sum = val;
            return;
        }
        int mid = (root->start + root->end) / 2;
        if (index <= mid) {
            update_tree(root->left, index, val);
        } else {
            update_tree(root->right, index, val);
        }
        root->sum = root->left->sum + root->right->sum;
    }
    
    int sum_range(SegmentTreeNode* root, int i, int j) {
        if (root->start == i and root->end == j) {
            return root->sum;
        }
        int mid = (root->start + root->end) / 2;
        if (j <= mid) {
            return sum_range(root->left, i, j);
        } else if (i > mid) {
            return sum_range(root->right, i, j);
        } 
        return sum_range(root->left, i, mid) + sum_range(root->right, mid + 1, j);
    }

public:
    NumArray(vector<int>& nums) {
        nums_.swap(nums);
        if (!nums_.empty()) {
            root_.reset(build_tree(nums_, 0, nums_.size() - 1));
        }
    }
    
    void update(int i, int val) {
        update_tree(root_.get(), i, val);
    }
    
    int sumRange(int i, int j) {
        return sum_range(root_.get(), i, j);
    }
};