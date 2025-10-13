#pragma once

#include "TaskManager.h"

// 派生类：使用双向链表实现任务管理器
class DoublyLinkedTaskManager : public TaskManager {
public:
    // 构造函数
    DoublyLinkedTaskManager();
    // 析构函数
    ~DoublyLinkedTaskManager();

    // 重写基类的所有纯虚函数
    void addTask(const Task& task) override;
    bool deleteTask(const std::string& name) override;
    bool updateTask(const std::string& name, const Task& newTask) override;
    std::vector<Task> queryTasks(const std::string& queryString, bool byName) override;
    std::vector<Task> getAllTasksSorted(bool byDueDate) override;
    bool saveToFile(const std::string& filename) override;
    bool loadFromFile(const std::string& filename) override;

private:
    // 双向链表节点结构
    struct Node {
        Task data;
        Node* next;
        Node* prev; // 新增：指向前一个节点的指针
        Node(const Task& task) : data(task), next(nullptr), prev(nullptr) {}
    };

    Node* head; // 指向链表头部
    Node* tail; // 指向链表尾部

    // 清空链表函数
    void clear();
};
