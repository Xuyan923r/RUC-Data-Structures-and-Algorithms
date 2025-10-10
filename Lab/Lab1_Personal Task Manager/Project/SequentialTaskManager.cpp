#include "SequentialTaskManager.h"
#include <fstream>
#include <algorithm>
#include <iostream>

void SequentialTaskManager::addTask(const Task& task) {
    tasks.push_back(task);
}

bool SequentialTaskManager::deleteTask(const std::string& name) {
    auto it = std::find_if(tasks.begin(), tasks.end(), [&](const Task& task) {
        return task.name == name;
    });

    if (it != tasks.end()) {
        tasks.erase(it);
        return true;
    }

    std::cout << "不存在该任务，无法删除。\n";
    return false;
}

bool SequentialTaskManager::updateTask(const std::string& name, const Task& newTask) {
    auto it = std::find_if(tasks.begin(), tasks.end(), [&](const Task& task) {
        return task.name == name;
    });

    if (it != tasks.end()) {
        *it = newTask;
        return true;
    }

    return false;
}

std::vector<Task> SequentialTaskManager::queryTasks(const std::string& queryString, bool byName) {
    std::vector<Task> result;
    for (const auto& task : tasks) {
        if (byName) {
            if (task.name == queryString) {
                result.push_back(task);
            }
        } else {
            if (task.dueDate == queryString) {
                result.push_back(task);
            }
        }
    }
    return result;
}

std::vector<Task> SequentialTaskManager::getAllTasksSorted(bool byDueDate) {
    std::vector<Task> sortedTasks = tasks;

    if (byDueDate) {
        std::sort(sortedTasks.begin(), sortedTasks.end(), [](const Task& a, const Task& b) {
            return a.dueDate < b.dueDate;
        });
    } else {
        std::sort(sortedTasks.begin(), sortedTasks.end(), [](const Task& a, const Task& b) {
            return a.priority > b.priority;
        });
    }
    return sortedTasks;
}

bool SequentialTaskManager::saveToFile(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        return false;
    }

    for (const auto& task : tasks) {
        outFile << task.name << "," << task.description << "," << task.priority << "," << task.dueDate << "\n";
    }

    outFile.close();
    return true;
}

bool SequentialTaskManager::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        return false;
    }

    tasks.clear();
    std::string line;
    while (std::getline(inFile, line)) {
        Task task;
        size_t pos = 0;
        std::string parts[4];

        for(int i=0; i<3; ++i) {
            pos = line.find(",");
            parts[i] = line.substr(0, pos);
            line.erase(0, pos + 1);
        }
        parts[3] = line;

        task.name = parts[0];
        task.description = parts[1];
        task.priority = std::stoi(parts[2]);
        task.dueDate = parts[3];

        tasks.push_back(task);
    }

    inFile.close();
    return true;
}