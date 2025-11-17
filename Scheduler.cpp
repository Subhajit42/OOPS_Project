// Scheduler.cpp
#include "Scheduler.h"
#include <iostream>
#include <algorithm>
#include <ctime>

/**
 * @file Scheduler.cpp
 * @brief Implementation of Scheduler methods.
 *
 * @details The implementations mirror the documented behaviour in
 *          `Scheduler.h` and include console output for user feedback.
 */

/**
 * @brief Construct a new Scheduler object and initialize id counter.
 *
 * @note Initializes `nextId` to 1.
 * @return void
 */
Scheduler::Scheduler() : nextId(1) {}

/**
 * @brief Add a new task to the staged list.
 *
 * @param description Human-readable description of the task.
 * @param estimate Estimated duration in minutes.
 * @note Side-effect: appends the new Task to `stagedTasks` and increments `nextId`.
 * @return void
 */
void Scheduler::addTask(const std::string& description, int estimate) {
    Task t(nextId++, description, estimate);
    stagedTasks.push_back(t);
    std::cout << "Added task [#" << t.id << "] to staged tasks.\n";
}

/**
 * @brief Start a staged task by id: record start time and move to activeTasks.
 *
 * @param id Identifier of the task to start.
 * @note Side-effect: task is moved from `stagedTasks` to `activeTasks` and
 *       its `startTime` and `status` are modified.
 * @return void
 */
void Scheduler::startTask(int id) {
    Task* t = findTaskById(id, stagedTasks);
    if (!t) {
        std::cout << "Task [#" << id << "] not found in staged tasks.\n";
        return;
    }

    // Mark active on the actual object in stagedTasks, then move it
    t->markActive();
    // copy to active list and remove from staged
    activeTasks.push_back(*t);
    // remove original from stagedTasks
    stagedTasks.erase(std::remove_if(stagedTasks.begin(), stagedTasks.end(), [id](const Task& x){ return x.id == id; }), stagedTasks.end());

    std::cout << "Started task [#" << id << "].\n";
}

/**
 * @brief Finish an active task by id: record finish time and move to finishedLog.
 *
 * @param id Identifier of the task to finish.
 * @note Side-effect: task is moved from `activeTasks` to `finishedLog` and
 *       its `finishTime` and `status` are modified.
 * @return void
 */
void Scheduler::finishTask(int id) {
    Task* t = findTaskById(id, activeTasks);
    if (!t) {
        std::cout << "Task [#" << id << "] not found in active tasks.\n";
        return;
    }

    t->markFinished();
    finishedLog.push_back(*t);
    activeTasks.erase(std::remove_if(activeTasks.begin(), activeTasks.end(), [id](const Task& x){ return x.id == id; }), activeTasks.end());

    std::cout << "Finished task [#" << id << "].\n";
}

/**
 * @brief Print staged tasks.
 *
 * @note Prints information about tasks in `stagedTasks` to stdout. No
 *       internal state is modified.
 * @return void
 */
void Scheduler::viewStagedTasks() const {
    std::cout << "--- Staged Tasks (" << stagedTasks.size() << ") ---\n";
    if (stagedTasks.empty()) {
        std::cout << "(none)\n";
        return;
    }
    for (const auto& t : stagedTasks) {
        std::cout << t.getDetails() << "\n";
    }
}

/**
 * @brief Print active tasks.
 *
 * @note Prints information about tasks in `activeTasks` to stdout. No
 *       internal state is modified.
 * @return void
 */
void Scheduler::viewActiveTasks() const {
    std::cout << "--- Active Tasks (" << activeTasks.size() << ") ---\n";
    if (activeTasks.empty()) {
        std::cout << "(none)\n";
        return;
    }
    for (const auto& t : activeTasks) {
        std::cout << t.getDetails() << "\n";
    }
}

/**
 * @brief Print finished task log including actual durations.
 *
 * @note Prints the actual duration calculated as (finishTime - startTime)
 *       in seconds and minutes when both timestamps are present.
 * @return void
 */
void Scheduler::printLog() const {
    std::cout << "--- Finished Tasks Log (" << finishedLog.size() << ") ---\n";
    if (finishedLog.empty()) {
        std::cout << "(none)\n";
        return;
    }
    for (const auto& t : finishedLog) {
        std::cout << t.getDetails();
        if (t.startTime != 0 && t.finishTime != 0) {
            long long seconds = static_cast<long long>(std::difftime(t.finishTime, t.startTime));
            long long minutes = seconds / 60;
            long long rem = seconds % 60;
            std::cout << " | Actual: " << seconds << " s (" << minutes << " m " << rem << " s)";
        }
        std::cout << "\n";
    }
}

/**
 * @brief Find a task by id inside a vector and return pointer to it.
 *
 * @param id Task id to locate.
 * @param list Vector to search within.
 * @return Task* Pointer to the element within the vector, or nullptr if not found.
 * @note Returned pointer becomes invalid if the vector is modified in a way
 *       that causes reallocation or if the element is erased.
 */
Task* Scheduler::findTaskById(int id, std::vector<Task>& list) {
    for (auto& t : list) {
        if (t.id == id) return &t;
    }
    return nullptr;
}
