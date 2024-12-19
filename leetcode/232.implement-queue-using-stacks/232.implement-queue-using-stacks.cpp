/*
 * @lc app=leetcode id=232 lang=cpp
 *
 * [232] Implement Queue using Stacks
 */

// @lc code=start
class MyQueue {
   public:
    stack<int> in_s, out_s;

    MyQueue() {
    }

    void push(int x) {
        in_s.push(x);
    }

    int pop() {
        if (out_s.empty()) {
            int in_s_sz = in_s.size();
            for (int i = 0; i < in_s_sz; i++) {
                out_s.push(in_s.top());
                in_s.pop();
            }
        }
        int out = out_s.top();
        out_s.pop();
        return out;
    }

    int peek() {
        int out = pop();
        out_s.push(out);
        return out;
    }

    bool empty() {
        return out_s.empty() && in_s.empty();
    }
};

/**
 * Your MyQueue object will be instantiated and called as such:
 * MyQueue* obj = new MyQueue();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->peek();
 * bool param_4 = obj->empty();
 */

// @lc code=end
