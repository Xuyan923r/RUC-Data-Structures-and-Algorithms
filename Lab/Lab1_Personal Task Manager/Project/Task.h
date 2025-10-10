#ifndef TASK_H
#define TASK_H

#include <string>

// 定义任务的数据结构
struct Task {
    std::string name;        // 任务名称 
    std::string description; // 任务描述
    int priority;            // 任务优先级 (1-5，数字越大优先级越高)
    std::string dueDate;     // 任务截止日期 (格式: YYYY-MM-DD) 
};

#endif // TASK_H