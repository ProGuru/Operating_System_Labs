#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> // заголовочный файл для работы с getpid(), getppid()

using namespace std;

int main (){
int size;
int i;
pid_t check = getpid();
std::cout << "Input tree width: ";
std::cin >> size;
pid_t *PidArray = new pid_t [size];

for (i = 0; i < size; i++){
    if (getpid() == check)
        fork();
    else
        break;
}

if (getpid() == check){
    i = 0;
    std::cout << "Process tree created. Press ENTER to close the processes.";
    cin.ignore();
    cin.get();
    std::cout << "Resulting id numbers and exit codes:\n";
    for (int j=0; j<size; j++)
        kill(PidArray[j], SIGCONT);
    delete [] PidArray;
}
else{
    PidArray[i-1] = getpid();
    kill(getpid(), SIGSTOP);
}


std::cout << "ChildID: " << getpid() << " ParentID: " << getppid() << " Exit Code: " << i << endl;

return 0;
}
