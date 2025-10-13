#include <iostream>
#include <string>
#include <limits>   // 提供数值极限
#include <vector>
#include <chrono>   // 获取当前时间
#include <ctime>    // 日期计算
#include <sstream>  // 解析日期字符串

#include "TaskManager.h"
#include "SequentialTaskManager.h"   // 顺序表实现
#include "LinkedTaskManager.h"       // 单向链表实现
#include "DoublyLinkedTaskManager.h" // 双向链表实现


// --- 原有的函数声明 ---
void printMenu();
void handleAddTask(TaskManager* manager);
void handleDeleteTask(TaskManager* manager);
void handleUpdateTask(TaskManager* manager);
void handleQueryTasks(TaskManager* manager);
void handleDisplayTasks(TaskManager* manager, bool byDueDate);
void handleSaveToFile(TaskManager* manager);
void handleLoadFromFile(TaskManager* manager);
void handleCheckUpcomingTasks(TaskManager* manager);
void printTasks(const std::vector<Task>& tasks);
std::vector<Task> filterUpcomingTasks(const std::vector<Task>& allTasks, int daysThreshold); 

int main() {
    TaskManager* manager = nullptr;
    int choice;

    std::cout << "=========================================\n";
    std::cout << "个人任务管理系统\n";
    std::cout << "=========================================\n";
    
    std::cout << "请选择数据结构 (1: 顺序表, 2: 单向链表, 3: 双向链表): ";
    std::cin >> choice;

    if (choice == 1) {
        manager = new SequentialTaskManager();
        std::cout << "\n已选择 [顺序表] 实现。\n";
    } else if (choice == 2) {
        manager = new LinkedTaskManager();
        std::cout << "\n已选择 [单向链表] 实现。\n";
    } else if (choice == 3) {
        manager = new DoublyLinkedTaskManager();
        std::cout << "\n已选择 [双向链表] 实现。\n";
    }
    else {
        std::cout << "无效选择，程序退出。\n";
        return 1;
    }

    // 主循环
    while (true) {
        printMenu();
        std::cout << "请输入您的选择: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "错误：请输入一个数字。\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: handleAddTask(manager); break;
            case 2: handleDeleteTask(manager); break;
            case 3: handleUpdateTask(manager); break;
            case 4: handleQueryTasks(manager); break;
            case 5: handleDisplayTasks(manager, true); break;  // true： 按ddl排序
            case 6: handleDisplayTasks(manager, false); break; // false：按优先级排序
            case 7: handleSaveToFile(manager); break;
            case 8: handleLoadFromFile(manager); break;
            case 9: handleCheckUpcomingTasks(manager); break;
            case 0:
                std::cout << "感谢使用，再见！\n";
                delete manager;
                return 0;
            default:
                std::cout << "无效选择，请重新输入。\n";
                break;
        }
    }

    return 0;
}

// 打印主菜单
void printMenu() {
    std::cout << "\n---------- 主菜单 ----------\n";
    std::cout << "1. 添加新任务\n";
    std::cout << "2. 删除任务\n";
    std::cout << "3. 修改任务\n";
    std::cout << "4. 查询任务\n";
    std::cout << "5. 按截止日期显示所有任务\n";
    std::cout << "6. 按优先级显示所有任务\n";
    std::cout << "7. 保存任务到文件\n";
    std::cout << "8. 从文件加载任务\n";
    std::cout << "9. 检查即将到期的任务\n";
    std::cout << "0. 退出系统\n";
    std::cout << "---------------------------\n";
}


void cleanInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << "任务列表为空或未找到匹配项。\n";
        return;
    }
    std::cout << "--------------------------------------------------\n";
    for (const Task& task : tasks) {
        std::cout << "任务名称: " << task.name << "\n"
                  << "  描述: " << task.description << "\n"
                  << "  优先级: " << task.priority << "\n"
                  << "  截止日期: " << task.dueDate << "\n";
        std::cout << "--------------------------------------------------\n";
    }
}

void handleAddTask(TaskManager* manager) {
    Task newTask;
    cleanInputBuffer(); 
    std::cout << "请输入任务名称: ";
    std::getline(std::cin, newTask.name);
    std::cout << "请输入任务描述: ";
    std::getline(std::cin, newTask.description);
    std::cout << "请输入优先级(1-5)(数字越大表示越紧急): ";
    std::cin >> newTask.priority;
    std::cout << "请输入截止时间(YYYY-MM-DD): ";
    std::cin >> newTask.dueDate;
    manager->addTask(newTask);
    std::cout << "添加成功！\n";
}

void handleDeleteTask(TaskManager* manager) {
    std::string name;
    cleanInputBuffer();
    std::cout << "请输入要删除的任务名称: ";
    std::getline(std::cin, name);   // getline 可以读取包含空格的任务名 cin不行
    manager->deleteTask(name);
}

void handleUpdateTask(TaskManager* manager) {
    std::string name;
    cleanInputBuffer();
    std::cout << "请输入要修改的任务名称: ";
    std::getline(std::cin, name);

    if (manager->queryTasks(name, true).empty()) {
        std::cout << "未找到该任务。\n";
        return;
    }

    Task updatedTask;
    updatedTask.name = name;
    std::cout << "请输入新的任务描述: ";
    std::getline(std::cin, updatedTask.description);
    std::cout << "请输入新的优先级 (1-5): ";
    std::cin >> updatedTask.priority;
    std::cout << "请输入新的截止日期 (YYYY-MM-DD): ";
    std::cin >> updatedTask.dueDate;

    if (manager->updateTask(name, updatedTask)) {
        std::cout << "修改成功！\n";
    }
}

void handleQueryTasks(TaskManager* manager) {
    int choice;
    std::cout << "请选择查询方式 (1: 按名称, 2: 按截止日期): ";
    std::cin >> choice;
    cleanInputBuffer();
    
    std::string query;
    std::vector<Task> results;

    if (choice == 1) {
        std::cout << "请输入任务名称: ";
        std::getline(std::cin, query);
        results = manager->queryTasks(query, true);
    } else if (choice == 2) {
        std::cout << "请输入截止日期 (YYYY-MM-DD): ";
        std::getline(std::cin, query);
        results = manager->queryTasks(query, false);
    } else {
        std::cout << "无效选择。\n";
        return;
    }
    std::cout << "\n查询结果:\n";
    printTasks(results);
}

void handleDisplayTasks(TaskManager* manager, bool byDueDate) {
    std::vector<Task> tasks = manager->getAllTasksSorted(byDueDate);
    if (byDueDate) {
        std::cout << "\n---所有任务 (按截止日期排序)---\n";
    } else {
        std::cout << "\n---所有任务 (按优先级排序)---\n";
    }
    printTasks(tasks);
}

void handleSaveToFile(TaskManager* manager) {
    std::string filename;
    cleanInputBuffer();
    std::cout << "请输入要保存的文件名 (例如: tasks.txt): ";
    std::getline(std::cin, filename);
    if (manager->saveToFile(filename)) {
        std::cout << "保存成功！\n";
    } else {
        std::cout << "保存失败！\n";
    }
}

void handleLoadFromFile(TaskManager* manager) {
    std::string filename;
    cleanInputBuffer();
    std::cout << "请输入要导入的任务文件地址: ";
    std::getline(std::cin, filename);
    if (manager->loadFromFile(filename)) {
        std::cout << "导入成功！\n";
    } else {
        std::cout << "文件不存在，导入失败！\n";
    }
}

void handleCheckUpcomingTasks(TaskManager* manager) {
    int daysThreshold;
    std::cout << "请输入要检查的天数 (例如，输入 7 会查找未来一周内到期的任务): ";
    std::cin >> daysThreshold;

    // 检查用户输入是否为有效数字
    if (std::cin.fail() || daysThreshold < 0) {
        std::cout << "错误：请输入一个有效的非负整数。\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    std::cout << "\n--- 检查未来 " << daysThreshold << " 天内到期的任务 ---\n";
    
    // 步骤 1: 从 manager 获取所有任务
    std::vector<Task> allTasks = manager->getAllTasksSorted(false);

    // 步骤 2: 将所有任务和用户输入的天数交给独立的筛选函数处理
    std::vector<Task> upcomingTasks = filterUpcomingTasks(allTasks, daysThreshold);
    
    printTasks(upcomingTasks);
}

// 输入: "YYYY-MM-DD" 格式的日期字符串 返回: 该日期距离今天的天数。负数表示已过期。
int daysUntil(const std::string& dateStr) {
    std::tm dueDate = {};
    std::stringstream ss(dateStr);
    char delimiter;
    int year, month, day;

    ss >> year >> delimiter >> month >> delimiter >> day;
    dueDate.tm_year = year - 1900;
    dueDate.tm_mon = month - 1;
    dueDate.tm_mday = day;

    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* today = std::localtime(&now_time);

    std::tm today_date_only = *today;
    today_date_only.tm_hour = 0; today_date_only.tm_min = 0; today_date_only.tm_sec = 0;

    std::time_t due_time = std::mktime(&dueDate);
    std::time_t today_time = std::mktime(&today_date_only);

    if (due_time == -1) return -9999; // 无效日期
    
    double seconds_diff = std::difftime(due_time, today_time);
    return static_cast<int>(seconds_diff / (60 * 60 * 24));
}

// 独立的提醒服务函数 (与数据结构无关)
std::vector<Task> filterUpcomingTasks(const std::vector<Task>& allTasks, int daysThreshold) {
    std::vector<Task> result;
    for (const auto& task : allTasks) {
        int remainingDays = daysUntil(task.dueDate);
        if (remainingDays >= 0 && remainingDays <= daysThreshold) {
            result.push_back(task);
        }
    }
    return result;
}

