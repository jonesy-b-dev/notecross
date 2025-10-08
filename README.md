# Notecross
Cross‑device task manager: add/edit via phone or terminal, sync locally or when online, server‑free reminders.

## Installation
Only  tested on linux so far. Build system might improve some day.

### Build from source (linux)
**Dependencies***
* Premake 5
* C++ compiler

**How to build**
* Clone repository
* run Premake `premake5 gmake2`
* run any of the shell scripts

## How To use

### Linux terminal
**--add / -a {newTaskName} {newTaskDue}**
	Add a new task using the AddTask function

**--update / -u {taskId} {newTaskName} {newTaskDue}:**
	Update task using the TaskUpdate function

**--remove /-r {taskId}:**
	Remove task using the TaskRemove function

**--done /-d {taskId}:**
	Remove task using the TaskRemove function

**--list /-l:**
	List all the tasks that are open using the TaskGetAll() function

**--sync / -s:**
	Manually sync the task file using the TaskSync option

### Android
WIP

### Where is windows and IOS?
Maybe someday? No time for it right now.
