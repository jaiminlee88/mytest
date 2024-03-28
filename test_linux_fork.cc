#include <iostream>
#include <unistd.h> // For fork()
#include <sys/wait.h> // For wait()

int main() {
    pid_t pid = fork(); // Create a new process

    if (pid == -1) {
        // Fork failed
        std::cerr << "Fork failed" << std::endl;
        return 1;
    } else if (pid > 0) {
        // Parent process
        std::cout << "Parent: pid= " << pid << std::endl;
        std::cout << "Parent: process (PID: " << getpid() << ")" << std::endl;
        wait(NULL); // Wait for the child process to finish
        std::cout << "Parent: Child process finished" << std::endl;
    } else {
        // Child process, 子进程自己会返回0
        std::cout << "Child: pid= " << pid << std::endl;
        std::cout << "Child: process (PID: " << getpid() << ")" << std::endl;
        sleep(2); // Simulate some work
        std::cout << "Child: process is exiting" << std::endl;
    }

    return 0;
}