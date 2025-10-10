#include "DoublyLinkedTaskManager.h"
#include <fstream>
#include <algorithm>
#include <iostream>

// 构造函数
DoublyLinkedTaskManager::DoublyLinkedTaskManager() : head(nullptr), tail(nullptr) {}

// 析构函数
DoublyLinkedTaskManager::~DoublyLinkedTaskManager() {
    clear();
}

// 私有辅助函数：清空链表
void DoublyLinkedTaskManager::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    tail = nullptr;
}

// 1. 添加任务 (得益于tail指针，效率更高)
void DoublyLinkedTaskManager::addTask(const Task& task) {
    Node* newNode = new Node(task);
    if (head == nullptr) { // 链表为空
        head = newNode;
        tail = newNode;
    } else { // 链表不为空
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode; // 更新尾指针
    }
}

// 2. 删除任务 (指针操作更复杂)
bool DoublyLinkedTaskManager::deleteTask(const std::string& name) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.name == name) {
            // 找到了要删除的节点
            if (current->prev != nullptr) {
                current->prev->next = current->next;
            } else { // 是头节点
                head = current->next;
            }

            if (current->next != nullptr) {
                current->next->prev = current->prev;
            } else { // 是尾节点
                tail = current->prev;
            }

            delete current;
            std::cout << "任务 '" << name << "' 删除成功！\n";
            return true;
        }
        current = current->next;
    }

    std::cout << "不存在该任务，无法删除。\n";
    return false;
}

// 3. 修改任务
bool DoublyLinkedTaskManager::updateTask(const std::string& name, const Task& newTask) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.name == name) {
            current->data = newTask;
            return true;
        }
        current = current->next;
    }
    return false;
}

// 4. 查询任务
std::vector<Task> DoublyLinkedTaskManager::queryTasks(const std::string& queryString, bool byName) {
    std::vector<Task> result;
    Node* current = head;
    while (current != nullptr) {
        bool match = byName ? (current->data.name == queryString) : (current->data.dueDate == queryString);
        if (match) {
            result.push_back(current->data);
        }
        current = current->next;
    }
    return result;
}

// 5. 查看任务列表 (排序) - 逻辑与单向链表实现相同
std::vector<Task> DoublyLinkedTaskManager::getAllTasksSorted(bool byDueDate) {
    std::vector<Task> allTasks;
    Node* current = head;
    while (current != nullptr) {
        allTasks.push_back(current->data);
        current = current->next;
    }

    if (byDueDate) {
        std::sort(allTasks.begin(), allTasks.end(), [](const Task& a, const Task& b) {
            return a.dueDate < b.dueDate;
        });
    } else {
        std::sort(allTasks.begin(), allTasks.end(), [](const Task& a, const Task& b) {
            return a.priority > b.priority;
        });
    }
    return allTasks;
}

// 6. 保存到文件 - 逻辑与单向链表实现相同
bool DoublyLinkedTaskManager::saveToFile(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) return false;

    Node* current = head;
    while (current != nullptr) {
        outFile << current->data.name << "," << current->data.description << "," 
                << current->data.priority << "," << current->data.dueDate << "\n";
        current = current->next;
    }
    outFile.close();
    return true;
}

// 7. 从文件加载
bool DoublyLinkedTaskManager::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) return false;

    clear(); // 加载前先清空

    std::string line;
    while (std::getline(inFile, line)) {
        Task task;
        size_t pos = 0;
        
        pos = line.find(",");
        task.name = line.substr(0, pos);
        line.erase(0, pos + 1);

        pos = line.find(",");
        task.description = line.substr(0, pos);
        line.erase(0, pos + 1);

        pos = line.find(",");
        task.priority = std::stoi(line.substr(0, pos));
        line.erase(0, pos + 1);
        
        task.dueDate = line;

        addTask(task); // 调用addTask来添加
    }

    inFile.close();
    return true;
}