#include "SequentialTaskManager.h"
#include <fstream>
#include <iostream>

void SequentialTaskManager::addTask(const Task& task) {
    tasks.push_back(task);
}

bool SequentialTaskManager::deleteTask(const std::string& name) {
    int length = static_cast<int>(tasks.size());
    for (int i = 0; i < length; ++i) {
        if (tasks[static_cast<std::size_t>(i)].name == name) {
            tasks.erase(tasks.begin() + i);
            return true;
        }
    }

    std::cout << "不存在该任务，无法删除。\n";
    return false;
}

bool SequentialTaskManager::updateTask(const std::string& name, const Task& newTask) {
    for (auto& task : tasks) {
        if (task.name == name) {
            task = newTask;
            return true;
        }
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

    if (sortedTasks.empty()) {
        return sortedTasks;
    }

    if (byDueDate) {
        int limit = static_cast<int>(sortedTasks.size());
        for (int i = 0; i < limit; ++i) {
            for (int j = 0; j < limit - 1; ++j) {
                std::size_t idx = static_cast<std::size_t>(j);
                std::size_t nextIdx = static_cast<std::size_t>(j + 1);
                if (sortedTasks[idx].dueDate > sortedTasks[nextIdx].dueDate) {
                    Task temp = sortedTasks[idx];
                    sortedTasks[idx] = sortedTasks[nextIdx];
                    sortedTasks[nextIdx] = temp;
                }
            }
        }
    } else {
        int limit = static_cast<int>(sortedTasks.size());
        for (int i = 0; i < limit; ++i) {
            for (int j = 0; j < limit - 1; ++j) {
                std::size_t idx = static_cast<std::size_t>(j);
                std::size_t nextIdx = static_cast<std::size_t>(j + 1);
                if (sortedTasks[idx].priority < sortedTasks[nextIdx].priority) {
                    Task temp = sortedTasks[idx];
                    sortedTasks[idx] = sortedTasks[nextIdx];
                    sortedTasks[nextIdx] = temp;
                }
            }
        }
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
