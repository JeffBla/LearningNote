/*
 * @lc app=leetcode id=225 lang=cpp
 *
 * [225] Implement Stack using Queues
 */

// @lc code=start
class MyStack {
   public:
    queue<int> q1, q2;

    MyStack() {
    }

    void push(int x) {
        q1.push(x);
    }

    int pop() {
        int out;
        if (q1.empty()) {
            int q2_sz = q2.size();
            for (int i = 0; i < q2_sz - 1; i++) {
                q1.push(q2.front());
                q2.pop();
            }
            out = q2.front();
            q2.pop();
        } else {
            int q1_sz = q1.size();
            for (int i = 0; i < q1_sz - 1; i++) {
                q2.push(q1.front());
                q1.pop();
            }
            out = q1.front();
            q1.pop();
        }
        return out;
    }

    int top() {
        int out = pop();
        push(out);
        return out;
    }

    bool empty() {
        if (q1.empty() && q2.empty())
            return true;
        return false;
    }
};

/**
 * Your MyStack object will be instantiated and called as such:
 * MyStack* obj = new MyStack();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->top();
 * bool param_4 = obj->empty();
 */
// @lc code=end
