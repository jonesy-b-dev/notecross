# Notecross
Cross‑device task manager: add/edit via phone or terminal, sync locally or when online, server‑free reminders.

This repository contains all the source code from all the Notecross components.
* Background daemon (Android/Linux)
* CLI tool (Linux)
* Android app (Android) (not started yet)

## Installation
So far no prebuild binaris availible, you will need to build from source for now.

### Build from source (linux)
**Dependencies***
* [Premake 5](https://premake.github.io/)
* C++ compiler (tested with gcc)
* [Libnotify](https://www.linuxfromscratch.org/blfs/view/svn/x/libnotify.html)

**How to build**
Run the following after installing all dependencies
```
premake5 gmake2
./runall.sh
```
The daemon should now be running in the background, daemon logs can be found in `/tmp/notecross.log`

**To use the cli tool from everywhere**
* run `sudo cp notecross-cli/bin/Debug/notecross /usr/local/bin/`

## How To use
`notecross {option}`
### Notecross CLI options
**--add / -a {Task Name} {Task Due}**
	Add a new task to your list

**--update / -u {Task ID} {New Task Name} {New Task Due}**
	Update an existing task using the task ID

**--remove / -r {Task ID}**
	Remove task using the task ID

**--done / -d {Task ID}**
	Mark task as done (not removing it) using the task ID

**--listall / -la**
	List all the tasks including completed ones

**--list / -l**
	List all the tasks that are open right now

**--sync / -s**
	Manually sync the task file with your other devices

**--help / -h**
	Provide this message


#### Date format
Task due date needs to be formatted as following:

`{amount}{size}`

**Examples**
* 15d (exactly 15 days from moment of execution)
* 3h (3 hours)

**Supported size formats**
* m (minutes)
* h (hours)
* d (days)


### Android
WIP

### Where is windows and IOS?
Maybe someday? No time for it right now.
