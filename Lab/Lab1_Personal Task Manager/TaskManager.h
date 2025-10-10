#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <string>
#include <vector>
#include "Task.h"

// 定义任务管理器的抽象基类 (接口)
class TaskManager {
public:
    // 虚析构函数，确保通过基类指针删除派生类对象时能正确释放资源
    virtual ~TaskManager() {}

    // 添加任务
    virtual void addTask(const Task& task) = 0;

    // 根据任务名称删除任务
    virtual bool deleteTask(const std::string& name) = 0;

    // 修改任务 
    virtual bool updateTask(const std::string& name, const Task& newTask) = 0;

    // 查询任务 (根据名称或截止日期) 
    virtual std::vector<Task> queryTasks(const std::string& queryString, bool byName) = 0;

    // 获取所有任务并排序 (按截止日期或优先级)
    virtual std::vector<Task> getAllTasksSorted(bool byDueDate) = 0;

    // 保存任务列表到文件 
    virtual bool saveToFile(const std::string& filename) = 0;
    
    // 从文件读取任务列表 
    virtual bool loadFromFile(const std::string& filename) = 0;
};

#endif // TASK_MANAGER_H