#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

void createProcess(int depth, int currentDepth);

int main() {
    int treeDepth;
    
    std::cout << "Введите глубину дерева процессов: ";
    std::cin >> treeDepth;
    
    if (treeDepth < 1) {
        std::cerr << "Глубина дерева должна быть положительным числом." << std::endl;
        return 1;
    }
    
    createProcess(treeDepth, 1);
    std::cin.get();
    
    return 0;
}

void createProcess(int depth, int currentDepth) {
    if (currentDepth > depth) {
        return;
    }

    pid_t childPid = fork(); // Создание нового процесса

    if (childPid < 0) {
        std::cerr << "Ошибка при порождении процесса." << std::endl;
        exit(EXIT_FAILURE);
    } else if (childPid == 0) {
        // Код, выполняемый в дочернем процессе
        std::cout << "Дочерний процесс (PID " << getpid() << ") на глубине " << currentDepth << ", родительский процесс (PPID " << getppid() << ")" << std::endl;

        // Рекурсивный вызов для создания следующего уровня дерева
        createProcess(depth, currentDepth + 1);

        // Выход из дочернего процесса
        exit(EXIT_SUCCESS);
    } else {
        // Код, выполняемый в родительском процессе
        int status;
        waitpid(childPid, &status, 0); // Ожидание завершения дочернего процесса

        if (WIFEXITED(status)) {
            std::cout << "Дочерний процесс (PID " << childPid << ") завершился с кодом возврата " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cerr << "Дочерний процесс (PID " << childPid << ") завершился некорректно." << std::endl;
        }
    }
}

