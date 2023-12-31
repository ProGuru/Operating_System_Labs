#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h> // заголовочный файл для работы с getpid(), getppid()
#include <sys/wait.h>// для работы wait () и waitpid ()

void createProcess(int depth, int currentDepth);

int main() {
    int treeDepth;
    
    std::cout << "Вариант задания, при котором дерево процессов растёт в глубину.\n\nВведите глубину дерева процессов: ";
    std::cin >> treeDepth;
    
    if (treeDepth < 1) {
        std::cerr << "Глубина дерева должна быть положительным числом." << std::endl;
        return 1;
    }
        std::cout << "Дочерний процесс (PID " << getpid() << "), родительский процесс (PPID " << getppid() << ") ID: " << 0 << std::endl;
    createProcess(treeDepth, 1);
    
    return 0;
}

void createProcess(int depth, int currentDepth) {

	if (currentDepth > depth) {
		return;
	}
	
    pid_t childPid = fork(); // Создание нового процесса. функция fork() создаёт копию текущего процесса. Она возвращает в родительском процессе идентификатор дочернего, а в дочернем - родительского.

    if (childPid < 0) { // returns 0 on success, -1 on failure
        std::cerr << "Ошибка при порождении процесса." << std::endl;
        exit(EXIT_FAILURE);
    } else if (childPid == 0) {
        // Код, выполняемый в дочернем процессе
        std::cout << "Дочерний процесс (PID " << getpid() << "), родительский процесс (PPID " << getppid() << ") ID: " << currentDepth << std::endl;

        // Рекурсивный вызов для создания следующего уровня дерева
        createProcess(depth, currentDepth + 1);

        // Выход из дочернего процесса
        exit(EXIT_SUCCESS);
    } else {
    	
        // Код, выполняемый в родительском процессе
        int status;
        waitpid(childPid, &status, 0); // Ожидание завершения дочернего процесса

        if (WIFEXITED(status)) {
        std::cin.ignore().get(); //Pause Command for Linux Terminal
            std::cout << "Дочерний процесс (PID " << childPid << ") завершился с кодом возврата " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cerr << "Дочерний процесс (PID " << childPid << ") завершился некорректно." << std::endl;
        }
    }
}

