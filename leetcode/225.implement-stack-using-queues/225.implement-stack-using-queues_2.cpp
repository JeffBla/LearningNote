/*
 * @lc app=leetcode id=225 lang=cpp
 *
 * [225] Implement Stack using Queues
 */

// @lc code=start
class MyStack {
   public:
    queue<int> q;

    MyStack() {
    }

    void push(int x) {
        q.push(x);
    }

    int pop() {
        int sz = q.size(), val;
        for(int i = 0; i < sz-1; i++){
            val = q.front();
            q.pop();
            q.push(val);
        }
        val = q.front();
        q.pop();
        return val;    
    }

    int top() {
        int val = pop();
        q.push(val);
        return val;
    }

    bool empty() {
        return q.empty();
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
