#include <iostream>
#include <sys/types.h> // for pid_t use
#include <sys/wait.h> // для работы wait () и waitpid ()
#include <unistd.h> // заголовочный файл для работы с getpid(), getppid()

using namespace std;

int main (){
	int size; // variable for tree width
	int i;
	pid_t check = getpid(); // getpid() - получеие идентификатора текущего процесса. Если = 0, то не удалось получить PID. pid_t data type stands for process identification.
	cout << "Input tree width: ";
	cin >> size;
	pid_t *PidArray = new pid_t [size]; // динамический прочесс из идентификаторов процесса

	for (i = 0; i < size; i++) { // на этом этапе создаются потомки в количестве size штук
		if (getpid() == check)
        	fork(); // функция fork() создаёт копию текущего процесса. Она возвращает в родительском процессе идентификатор дочернего, а в дочернем - родительского.
    else
        break;
	}

	if (getpid() == check) {
	    i = 0;
	    cout << "Process tree created. Press ENTER to close the processes.";
	    cin.ignore();
	    cin.get();
	    cout << "Resulting id numbers and exit codes:\n";
	    
	    for (int j=0; j < size; j++)
	        kill(PidArray[j], SIGCONT); // функция принудительного завершения процессов
	        
	    delete [] PidArray;
	} else {
	    PidArray[i-1] = getpid();
	    kill(getpid(), SIGSTOP);
	}

	cout << "ChildID: " << getpid() << " ParentID: " << getppid() << " Exit Code: " << i << endl; // getppid() returns the process ID of the parent of the current process.

	cin.get();
	return 0;
}
