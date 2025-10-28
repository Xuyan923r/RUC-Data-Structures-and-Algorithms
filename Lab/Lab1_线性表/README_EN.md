# Task Management System

## Project Overview
This project is a task management system that supports users in managing tasks through three different data structures: sequential list, singly linked list, and doubly linked list. Users can choose the desired implementation based on their needs.

## Project Structure
```
Lab1/
├── DoublyLinkedTaskManager.cpp   // Task manager implemented with a doubly linked list
├── DoublyLinkedTaskManager.h     // Header file for the doubly linked list implementation
├── LinkedTaskManager.cpp         // Task manager implemented with a singly linked list
├── LinkedTaskManager.h           // Header file for the singly linked list implementation
├── SequentialTaskManager.cpp     // Task manager implemented with a sequential list
├── SequentialTaskManager.h       // Header file for the sequential list implementation
├── Task.h                        // Definition of the task data structure
├── TaskManager.h                 // Abstract base class for task managers
├── main.cpp                      // Main program entry point with user interface
├── tasks_10000.txt               // Example task data file
└── 实验1线性表的应用.docx        // Experiment documentation (in Chinese)
```

### File Descriptions
- **DoublyLinkedTaskManager.cpp / .h**
  - Implements the task manager using a doubly linked list.
  - Supports efficient task insertion and deletion, especially for operations at both ends of the list.

- **LinkedTaskManager.cpp / .h**
  - Implements the task manager using a singly linked list.
  - Simple implementation, suitable for scenarios with small task volumes and operations focused on the tail of the list.

- **SequentialTaskManager.cpp / .h**
  - Implements the task manager using a sequential list (`std::vector`).
  - Suitable for scenarios with small task volumes and frequent query operations.

- **Task.h**
  - Defines the basic task data structure, including task name, description, priority, and due date.

- **TaskManager.h**
  - Defines the abstract base class for task managers, containing pure virtual functions for all task management operations.

- **main.cpp**
  - Provides a user interface for selecting data structures and performing task management operations.

- **tasks_10000.txt**
  - Example task data file for testing task loading functionality.

- **实验1线性表的应用.docx**
  - Experiment documentation describing the background, objectives, and requirements of the experiment.

## Comparison of the Three Data Structures

| Feature             | Sequential List (`std::vector`) | Singly Linked List      | Doubly Linked List      |
|---------------------|---------------------------------|-------------------------|-------------------------|
| **Insertion Efficiency** | Low (efficient at the tail, inefficient elsewhere) | High (efficient at the tail) | High (efficient at both ends) |
| **Deletion Efficiency**   | Low (requires shifting elements) | Moderate (requires traversal) | High (efficient at both ends) |
| **Query Efficiency**      | High (random access)            | Low (requires traversal)       | Low (requires traversal)       |
| **Memory Usage**          | Contiguous memory, potential waste | Scattered memory, space-saving | Scattered memory, extra storage for previous pointers |
| **Implementation Complexity** | Simple                      | Moderate                     | Complex                      |

### Suitable Scenarios
- **Sequential List**: Best for small task volumes and frequent query operations.
- **Singly Linked List**: Suitable for moderate task volumes and operations focused on the tail.
- **Doubly Linked List**: Ideal for large task volumes and frequent operations at both ends of the list.

## Usage Instructions
1. Compile the project:
   ```bash
   g++ -o TaskManager main.cpp SequentialTaskManager.cpp LinkedTaskManager.cpp DoublyLinkedTaskManager.cpp -std=c++11
   ```

2. Run the program:
   ```bash
   ./TaskManager
   ```

3. Follow the prompts to select a data structure and perform task management operations.

## Experiment Summary
Through this experiment, we gained a deep understanding of the characteristics and applicable scenarios of sequential lists, singly linked lists, and doubly linked lists. By comparing the performance of these three data structures, we can better choose the appropriate data structure based on actual needs.