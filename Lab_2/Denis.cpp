#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>  // заголовочный файл для работы с getpid(), getppid()
#include <sys/types.h> // for pid_t use
#include <sys/wait.h> // для работы wait () и waitpid ()
int level = 1;
char const offsets[] = "\t\t\t\t\t\t\t\t"; //offsets
	int createChildProcess(int(*child_fn)()) { //function create child process
		int child_pid = fork(); // При успешной отправке сигнала функция возвращает вызывающему процессу значение 0, в противном случае — число -1.
		switch(child_pid) {
		case 0: // Child process
			++level;
			exit(child_fn()); //Функция exit() обеспечивает нормальное завершение вызывающего процесса. 
		case -1: // Fail
			abort();
		default: // Parent process
			printf("%.*s %d породил %d\n", level, offsets, getpid(), child_pid); 
		return child_pid;
		}
	}
	void waitForChild() {
		int wstatus;
		int child_pid = wait(&wstatus);//wait for processes to change status if(child_pid == -1)
		if(child_pid == -1)
			abort();// Функция abort () вызывает аварийное окончание вызывающего процесса 
		printf("%.*s %u Завершился процесс с номером PID: \n", level, offsets, (unsigned)child_pid);
	}	
	
	int emptyFucntion() {return 0; }

	int functionWithTwoFork() {
		createChildProcess(emptyFucntion);
		createChildProcess(emptyFucntion);
		waitForChild();
		waitForChild();
		return 0;
	}
	
	int functionWithThreeForks() {
		createChildProcess(functionWithTwoFork); 
		createChildProcess(functionWithTwoFork); 
		createChildProcess(functionWithTwoFork); 
		waitForChild(); 
		waitForChild(); 
		waitForChild(); 
		return 0;
	}
	
	int functionWithOneFork() {
		createChildProcess(functionWithThreeForks);
		waitForChild();
		return 0;
	}		

	int main() {
		printf("%d Программе был выделен PID с номером: \n", getpid());
		createChildProcess(emptyFucntion);
		createChildProcess(functionWithOneFork);
		waitForChild();
		waitForChild();
		printf("%d завершился процесс с номером PID: \n", getpid());
		std::cin.get();
		return 0;
	}
