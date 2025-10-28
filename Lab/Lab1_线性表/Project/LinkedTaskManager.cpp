#include "LinkedTaskManager.h"
#include <fstream>
#include <iostream>

// 构造函数
LinkedTaskManager::LinkedTaskManager() : head(nullptr) {}

// 析构函数
LinkedTaskManager::~LinkedTaskManager() {
    clear();
}

// 私有辅助函数：清空链表
void LinkedTaskManager::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
}

// 1. 添加任务
void LinkedTaskManager::addTask(const Task& task) {
    Node* newNode = new Node(task);
    if (head == nullptr) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// 2. 删除任务
bool LinkedTaskManager::deleteTask(const std::string& name) {
    if (head == nullptr) return false;

    // 特殊情况：要删除的是头节点
    if (head->data.name == name) {
        Node* temp = head;
        head = head->next;
        delete temp;
        return true;
    }

    Node* current = head;
    while (current->next != nullptr && current->next->data.name != name) {
        current = current->next;
    }

    if (current->next != nullptr) { // 找到了
        Node* temp = current->next;
        current->next = temp->next;
        delete temp;
        return true;
    }
    
    std::cout << "提示: 未找到名为 '" << name << "' 的任务，无法删除。\n";
    return false; // 未找到
}

// 3. 修改任务
bool LinkedTaskManager::updateTask(const std::string& name, const Task& newTask) {
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
std::vector<Task> LinkedTaskManager::queryTasks(const std::string& queryString, bool byName) {
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

// 5. 查看任务列表 (排序)
std::vector<Task> LinkedTaskManager::getAllTasksSorted(bool byDueDate) {
    std::vector<Task> allTasks;
    Node* current = head;
    // 步骤1: 将链表中的所有任务复制到 vector 中
    while (current != nullptr) {
        allTasks.push_back(current->data);
        current = current->next;
    }

    if (byDueDate) {
        int len = static_cast<int>(allTasks.size());
        for (int i = 1; i < len; ++i) {
            Task currentTask = allTasks[static_cast<std::size_t>(i)];
            int back = i - 1;
            while (back >= 0) {
                std::size_t pos = static_cast<std::size_t>(back);
                if (currentTask.dueDate < allTasks[pos].dueDate) {
                    allTasks[pos + 1] = allTasks[pos];
                    --back;
                } else {
                    break;
                }
            }
            allTasks[static_cast<std::size_t>(back + 1)] = currentTask;
        }
    } else {
        int len = static_cast<int>(allTasks.size());
        for (int i = 1; i < len; ++i) {
            Task currentTask = allTasks[static_cast<std::size_t>(i)];
            int back = i - 1;
            while (back >= 0) {
                std::size_t pos = static_cast<std::size_t>(back);
                if (currentTask.priority > allTasks[pos].priority) {
                    allTasks[pos + 1] = allTasks[pos];
                    --back;
                } else {
                    break;
                }
            }
            allTasks[static_cast<std::size_t>(back + 1)] = currentTask;
        }
    }

    return allTasks;
}

// 6. 保存到文件
bool LinkedTaskManager::saveToFile(const std::string& filename) {
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
bool LinkedTaskManager::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) return false;

    clear(); // 加载前先清空现有链表

    std::string line;
    while (std::getline(inFile, line)) {
        Task task;
        size_t pos = 0;
        
        // 简单的CSV解析
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

        addTask(task); // 直接调用 addTask 方法来添加到链表
    }

    inFile.close();
    return true;
}
