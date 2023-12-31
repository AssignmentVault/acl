# ACL (algorithm course library)

A simple library created for a course of analysis and development of algorithms at ITMO University. The library provides dynamic array, sorting algorithm, memory allocator and tests for all of this strcutures.

# Create Build

* First of all, after you have cloned the repository into your folder, recursively initialize all the submodules of the project using the git command:

  ```
  git submodule update --init --recursive
  ```

* After successfully initializing the project submodules, run the necessary batch files in the repository root.\

  **GenerateProject.bat** - generates acl library msvs project\
  **GenerateTestProjects.bat** - generates all the library test msvs projects\
  **DeleteProjects.bat** - deletes all msvs projects from repository recursively

# Tasks

1. [Dynamic array task (RU)](https://github.com/AssignmentVault/acl/blob/master/tasks/P21_2_Dynamic_Array.pdf)
2. [Quick sorting algorithm task (RU)](https://github.com/AssignmentVault/acl/blob/master/tasks/P21_3_QuickSort.pdf)
3. [Memory manager task (RU)](https://github.com/AssignmentVault/acl/blob/master/tasks/P21_4_MemoryManager.pdf)
