#include "notificationCalculator.hpp"
#include <log.hpp>
#include <string>
#include <taskHelper.hpp>
#include <vector>

NCShared::Task CalculateNextNotification(long currentUnixTime)
{
    std::vector<NCShared::Task> tasks = NCShared::GetAllTasks();

    NCShared::Task taskToNotify;
    taskToNotify.dueDate = 9999999999;

    for (NCShared::Task task : tasks)
    {
        if (task.completed == true){
            continue;
		}
        if (task.dueDate == 0){
            continue;
		}
        if (task.dueDate < currentUnixTime){
            continue;
		}
        if (task.dueDate < taskToNotify.dueDate){
            taskToNotify = task;
		}
    }
    return taskToNotify;
}
