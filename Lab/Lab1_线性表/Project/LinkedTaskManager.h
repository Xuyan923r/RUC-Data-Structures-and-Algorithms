#pragma once

#include "TaskManager.h"

// 派生类：使用单向链表实现任务管理器
class LinkedTaskManager : public TaskManager {
public:
    // 构造函数，初始化头指针
    LinkedTaskManager();
    // 析构函数，释放链表所有节点的内存
    ~LinkedTaskManager();

    void addTask(const Task& task) override;
    bool deleteTask(const std::string& name) override;
    bool updateTask(const std::string& name, const Task& newTask) override;
    std::vector<Task> queryTasks(const std::string& queryString, bool byName) override;
    std::vector<Task> getAllTasksSorted(bool byDueDate) override;
    bool saveToFile(const std::string& filename) override;
    bool loadFromFile(const std::string& filename) override;

private:
    struct Node {
        Task data;
        Node* next;
        Node(const Task& task) : data(task), next(nullptr) {}
    };

    Node* head; // 指向链表头部的指针

    // 清空链表函数
    void clear(); 
};
