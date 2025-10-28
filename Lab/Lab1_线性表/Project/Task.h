#pragma once

#include <string>

// 定义任务的数据结构
struct Task {
    std::string name;        // 任务名称 
    std::string description; // 任务描述
    int priority;            // 任务优先级 (1-5，数字越大优先级越高)
    std::string dueDate;     // 任务截止日期 (格式: YYYY-MM-DD) 
};


// Notes:  为什么只有int priority前面没有std::string
// 因为int是基本数据类型，不需要包含头文件，而std::string是C++标准库中的类，需要包含<string>头文件。