#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>

#define DELAY_PROCESS_1 2
#define DELAY_PROCESS_2 3

void check_on_error(pid_t pid_of_prosses);

int main() {
	std::vector<std::string> dialog { 
		"— Алё!",
		"— Да да?",
		"— Ну как там с деньгами?",
		"— А?",
		"— Как с деньгами-то там?",
		"— Чё с деньгами?",
		"— Чё?",
		"— Куда ты звонишь?",
		"— Тебе звоню.",
		"— Кому?",
		"— Ну тебе.",
		"— Кому тебе?",
		"— А вот тебе вот.",
		"— Ты кто такой?",
		"— Михал Палыч.",
		"— Какой Михал Палыч?",
		"— Терентьев.",
		"— Такого не знаю, ты ошибся номером, друг.",
		"— Кто?",
		"— Ты.",
		"— Чё с деньгами?",
		"— Какими деньгами?",
		"— Ну которые я внес в капитал.",
		"— Куда?",
		"— В капитал прожиточного минимума.",
		"— Ты пьяный или кто, сынок?",
		"— Я тре... Я Михал Палыч Терентьев.",
		"— Кто такой?",
		"— Пьяный.",
		"— Вот именно, ну и все, завяжи лямку, пожалуйста.",
		"— Куда... Чё завязать?",
		"— Завяжи лямку!",
		"— А чё ты кричишь?",
		"— Да ничего!",
		"— Алё!",
		"— Да.",
		"— Ну чё там?",
		"— Чего?!",
		"— Как с деньгами обстоят во... Обстоит вопрос?",
		"— С какими деньгами?",
		"— Которые я вложил в капитал.",
		"— В какой капитал?",
		"— Прожиточного минимума.",
		"— Какой?",
		"— В смысле в какой?",
		"— Ты куда звонишь?! По какому номеру звонишь?",
		"— По твоему номеру.",
		"— Ты, сынок!",
		"— Чё?!",
		"— Сынок!",
		"— Чё?",
		"— Ты куда звонишь?!",
		"— Чё ты кричишь?",
		"— По какому телефону звонишь?!",
		"— Двадцать два, пятьсот пять.",
		"— Ну? Какой капитал?",
		"— Прожи... Прожиточного минимума!",
		"— Ну, прожиточного.",
		"— Ну а чё ты кричишь?",
		"— Хватит!",
		"— Ты чё?",
		"— Ты понял, да?!",
		"— Я не понимаю куд...",
		"— Всё, замолчи!",
		"— Алё.",
		"— Нехороший дочерний процесс!",
		"— А чё я?",
		"— Ты приедь сюда, я с тобой поговорю!",
		"— А какой адрес?",
		"— Вот приедь сюда!",
		"— Да не надо.",
		"— Ты понял, да?!",
		"— За что?",
		"— За всё!",
		"— А чё я сделал?",
		"— Сынок ёбаный!",
		"— Чё с деньгами?",
		"— Ты понял меня?!",
		"— Чё с деньгами, я спрашиваю?",
		"— Вот всё, какие то деньги, непонятные.",
		"— Кто?",
		"— Ты!",
		"— Почему?",
		"— Всё, замолчи.",
		"— Алё. Алё.",
		"— Я щас ещё узнаю с какого ты телефона звонишь!",
		"— Я звоню щас из офиса.",
		"— С какого офиса?",
		"— Находящейся по улице Красноармейской, четыре.",
		"— Вот, номер твой телефон скажи мне.",
		"— Алё.",
		"— Скажи номер телефона своего.",
		"— Зачем?",
		"— А чтобы я приехал к тебе.",
		"— За что это?",
		"— А за всё! Ты понял, да?",
		"— Ничё не понял.",
		"— Я тебе сказал, сынок.",
		"— Что?",
		"— Ты понял меня?",
		"— Папа!",
		"— Всё.",
		"— Чё всё то?",
		"— Ты давай номер телефона или сюда приезжай.",
		"— А с деньгами как вопрос обстоит?",
		"— С какими деньгами?!",
		"— Которые я вложил.",
		"— Куда ты вложил?!",
		"— В капитал.",
		"— В какой?",
		"— Прожиточного минимума.",
		"— Нехороший человек!",
		"— Что?"
	};

	int mes1 = 0;
	int mes2 = 1;
/*
	The pipe function is used to create a pipe (канал). It takes a single argument, a pointer to an array of two integers. The first element of the array is used to read from the pipe, and the second element is used to write to the pipe
*/
    int pipe_fd[2]; 

// The pipe function is created, and the file descriptor for the pipe is stored in the fd array.
	check_on_error(pipe(pipe_fd)); // If the function returns -1, the pipe creation has failed.
	
    pid_t child1_pid = fork(); // создаём дочерний процесс № 1: child1_pid
    
    check_on_error(child1_pid); // If the function returns -1, the fork creation has failed.
/*
  fork() function: On  success,  the PID of the child process is returned in the parent, and 0 is returned in the child.  On failure, -1 is returned in the parent, no child  process  is  created
*/
	if (child1_pid == 0) {  // Дочерний процесс № 1 (если == 0 значит не имеет потомков)
		close(pipe_fd[0]);  // Закрываем читающий конец пайпа

		while (true) {
			std::string child1_message = "Сообщение от процесса № 1: " + dialog[mes1++];
			mes1++;
/*
	The write function is used to write data to a file descriptor, in this case the write end of the pipe represented by fd[1]. It takes three arguments: the file descriptor to write to, a pointer to the data, and the number of bytes to be written.
*/
			write(pipe_fd[1], child1_message.c_str(), child1_message.size()); // передаём указатель на сообщение. функция c_str передаёт указатель;
			std::cout << "Дочерний процесс № 1 отправил сообщение\n";
			sleep(DELAY_PROCESS_1);  // ждём 2 секунды перед следующим сообщением
		}

		close(pipe_fd[1]);  // Закрываем записывающий конец пайпа
	} else {
		pid_t child2_pid = fork(); // создаём дочерний процесс № 2: child1_pid
        
		check_on_error(child2_pid);

		if (child2_pid == 0) {  // Дочерний процесс № 2 (если == 0 значит не имеет потомков)
			close(pipe_fd[0]);  // Закрываем читающий конец пайпа

			while (true) {
				std::string message = "Сообщение от процесса № 2: " + dialog[mes2++];
				mes2++;
				write(pipe_fd[1], message.c_str(), message.size());
				std::cout << "Дочерний процесс № 2 отправил сообщение\n";
				sleep(DELAY_PROCESS_2);  // ждём 3 секунды перед следующим сообщением
			}

			close(pipe_fd[1]);  // Закрываем записывающий конец пайпа
		} else {  // Родительский процесс
			close(pipe_fd[1]);  // Закрываем записывающий конец пайпа

			while (true) {
				char buffer[256];
/*
	The read function is used to read data from a file descriptor, in this case, the read end of the pipe represented by fd[0]. It takes three arguments: the file descriptor to read from, a pointer to the buffer where the data will be stored, and the number of bytes to be read.
*/
				ssize_t bytes_read = read(pipe_fd[0], buffer, sizeof(buffer));
				if (bytes_read > 0) {
					buffer[bytes_read] = '\0';
					std::cout << "Родительский процесс получил сообщение: " << buffer << std::endl << std::endl;
				}
			}

			close(pipe_fd[0]);  // Закрываем читающий конец пайпа
	        
			waitpid(child1_pid, nullptr, 0);  // Ждем завершения дочернего процесса 1
			waitpid(child2_pid, nullptr, 0);  // Ждем завершения дочернего процесса 2
		}
	}

	return 0;
}

// check error while creation child prosses
void check_on_error(int error_code) {
	if (error_code == -1)  
		perror("произошла ошибка создания процесса fork или канала pipe");
	return;
}
