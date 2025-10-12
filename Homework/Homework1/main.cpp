// 通用配置

#include <iostream>
#include <vector>
#include <algorithm> // swap 用

using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x): val(x), next(nullptr) {}
};

void printVector(const vector<int>& a) {
    cout << "[";
    for (int i = 0; i < a.size(); i++) {
        cout << a[i];
        if (i != a.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}

void printList(ListNode* head) {
    for (ListNode* p = head; p; p = p->next) {
        cout << p->val << " -> ";
    }
    cout << "nullptr" << endl;
}

void freeList(ListNode* head) {
    while (head) {
        ListNode* t = head;
        head = head->next;
        delete t;
    }
}

//问题1: 线性表逆置 
void reverseVector(vector<int>& a) {
    int i = 0, j = a.size() - 1;
    while (i < j) {
        swap(a[i], a[j]);
        i++, j--;
    }
}

ListNode* reverseList(ListNode* head) {
    ListNode* pre = nullptr;
    ListNode* p = head;
    while (p) {
        ListNode* q = p->next;
        p->next = pre;
        pre = p;
        p = q;
    }
    return pre;
}

// 问题2: 删除重复元素
void removeDupVector(vector<int>& a) {
    int k = 0;
    for (int i = 0; i < a.size(); i++) {
        bool dup = false;
        for (int j = 0; j < k; j++) {
            if (a[i] == a[j]) {
                dup = true;
                break;
            }
        }
        if (!dup) a[k++] = a[i];
    }
    a.resize(k);
}

void removeDupList(ListNode* head) {
    for (ListNode* p = head; p; p = p->next) {
        ListNode* q = p;
        while (q->next) {
            if (q->next->val == p->val) {
                ListNode* t = q->next;
                q->next = t->next;
                delete t;
            } else {
                q = q->next;
            }
        }
    }
}

// 问题3: 合并两个有序链表
ListNode* mergeLists(ListNode* a, ListNode* b) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (a && b) {
        if (a->val <= b->val) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    tail->next = a ? a : b;
    return dummy.next;
}

// 问题4: 循环左移
void reverseRange(vector<int>& a, int l, int r) {
    while (l < r) swap(a[l++], a[r--]);
}

void rotateLeft(vector<int>& a, int k) {
    int n = a.size();
    if (n == 0) return;
    k %= n;
    if (k == 0) return;
    reverseRange(a, 0, k - 1);
    reverseRange(a, k, n - 1);
    reverseRange(a, 0, n - 1);
}

ListNode* rotateLeftList(ListNode* head, int k) {
    if (!head) return head;
    int n = 1;
    ListNode* p = head;
    while (p->next) {
        p = p->next;
        n++;
    }
    k %= n;
    if (k == 0) return head;
    p->next = head;
    ListNode* newTail = head;
    for (int i = 0; i < k - 1; i++) newTail = newTail->next;
    ListNode* newHead = newTail->next;
    newTail->next = nullptr;
    return newHead;
}

// --- 问题5: 约瑟夫环 ---
int josephus(int n, int m) {
    if (n <= 0 || m <= 0) return -1;
    ListNode* head = new ListNode(1);
    ListNode* p = head;
    for (int i = 2; i <= n; i++) {
        p->next = new ListNode(i);
        p = p->next;
    }
    p->next = head; // 环
    ListNode* pre = p;
    while (pre->next != pre) {
        for (int i = 0; i < m - 1; i++) pre = pre->next;
        ListNode* t = pre->next;
        pre->next = t->next;
        cout << "淘汰: " << t->val << endl;
        delete t;
    }
    int ans = pre->val;
    delete pre;
    return ans;
}

// --- 主函数测试 ---
int main() {
    // 问题1
    vector<int> v1 = {4, 2, 7, 3, 0};
    cout << "原顺序表: "; printVector(v1);
    reverseVector(v1);
    cout << "逆置后: "; printVector(v1);

    ListNode* h1 = new ListNode(4);
    h1->next = new ListNode(2);
    h1->next->next = new ListNode(7);
    h1->next->next->next = new ListNode(3);
    h1->next->next->next->next = new ListNode(0);
    cout << "原链表: "; printList(h1);
    h1 = reverseList(h1);
    cout << "逆置后: "; printList(h1);
    freeList(h1);

    // 问题2
    vector<int> v2 = {1,2,2,3,4,4,5};
    cout << "\n去重前: "; printVector(v2);
    removeDupVector(v2);
    cout << "去重后: "; printVector(v2);

    ListNode* h2 = new ListNode(1);
    h2->next = new ListNode(2);
    h2->next->next = new ListNode(2);
    h2->next->next->next = new ListNode(3);
    h2->next->next->next->next = new ListNode(4);
    h2->next->next->next->next->next = new ListNode(4);
    h2->next->next->next->next->next->next = new ListNode(5);
    cout << "链表去重前: "; printList(h2);
    removeDupList(h2);
    cout << "链表去重后: "; printList(h2);
    freeList(h2);

    // 问题3
    ListNode* a = new ListNode(1);
    a->next = new ListNode(3);
    a->next->next = new ListNode(5);
    ListNode* b = new ListNode(2);
    b->next = new ListNode(4);
    b->next->next = new ListNode(6);
    cout << "\nA: "; printList(a);
    cout << "B: "; printList(b);
    ListNode* merged = mergeLists(a, b);
    cout << "合并后: "; printList(merged);
    freeList(merged);

    // 问题4
    vector<int> v4 = {1,2,3,4,5};
    cout << "\n原顺序表: "; printVector(v4);
    rotateLeft(v4, 2);
    cout << "左移2位: "; printVector(v4);

    ListNode* h4 = new ListNode(1);
    h4->next = new ListNode(2);
    h4->next->next = new ListNode(3);
    h4->next->next->next = new ListNode(4);
    h4->next->next->next->next = new ListNode(5);
    cout << "链表原: "; printList(h4);
    h4 = rotateLeftList(h4, 2);
    cout << "链表左移2位: "; printList(h4);
    freeList(h4);

    // 问题5
    cout << "\n约瑟夫环 (n=5, m=3)" << endl;
    int res = josephus(5, 3);
    cout << "最后剩下: " << res << endl;

    return 0;
}
