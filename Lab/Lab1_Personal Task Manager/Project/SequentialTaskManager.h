#pragma once

#include "TaskManager.h"
#include <vector>

// 派生类：使用顺序表 (std::vector) 实现任务管理器
class SequentialTaskManager : public TaskManager {
public:
    // 重写 (override) 基类的所有纯虚函数
    void addTask(const Task& task) override;
    bool deleteTask(const std::string& name) override;
    bool updateTask(const std::string& name, const Task& newTask) override;
    std::vector<Task> queryTasks(const std::string& queryString, bool byName) override;
    std::vector<Task> getAllTasksSorted(bool byDueDate) override;
    bool saveToFile(const std::string& filename) override;
    bool loadFromFile(const std::string& filename) override;

private:
    std::vector<Task> tasks; // 使用 std::vector 来存储任务
};
