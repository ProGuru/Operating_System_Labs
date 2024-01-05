#include "iostream"
#include <ctime>
#include "vector"
#include "string.h"
#define BLOCK_NUMBERS 64
#define SHEET 2
#define N_TACT 8
#define N_BLOCKS 8
#define N_TEST 4000

int arrayOfSpareBlocks [BLOCK_NUMBERS + 1] = {0};

class memory {
public:
	bool stateOfBlocks [BLOCK_NUMBERS + 1];
	int sizeBlocks;
	int lifeTime;
	void reset() {
		memset(stateOfBlocks, false, BLOCK_NUMBERS*sizeof(bool));
		sizeBlocks = 0;
		lifeTime = -1;
	}
	memory() {
		reset();
	}
};

std::vector<memory> memorys; // данные памяти при каждом цикле
memory ROM;

// обновляем массив arrayOfSpareBlocks
void refreshArrayOfSpareBlocks() {
	memset(arrayOfSpareBlocks, 0, BLOCK_NUMBERS*sizeof(int));
    int consecutiveCount = 0; // количество последовательно идущих битов
	int arrayIndex = 0;
    for (int i = 0; i < BLOCK_NUMBERS; i++) {
    	// если элемент массива ROM = 0, то подсчитывается количество свободных секторов подряд (ROM[i] = 0)
        if (!ROM.stateOfBlocks[i]) { 
            consecutiveCount++;
        } else {
            if (consecutiveCount > 0) {
                arrayOfSpareBlocks[arrayIndex] = consecutiveCount;
                consecutiveCount = 0;
            }
			arrayIndex = i + 1;
        }
    }
    // Добавим последнюю последовательность, если она заканчивается true
    if (consecutiveCount > 0) {
        arrayOfSpareBlocks[arrayIndex] = consecutiveCount;
    }
}

// обновляем массив ROM
void refreshROM() {
	ROM.reset(); // обнуляем ROM
	// в каждом новом такте уменьшаем время жизни блоков массива memory:
	for(int i = 0; i < N_TACT; i++) {
		if(memorys[i].lifeTime > 0) {
			memorys[i].lifeTime--;
		} else if(memorys[i].lifeTime == 0) {
			memorys[i].reset();
		}
	}
	// просматриваем каждый бит j каждого такта i для обновления ROM
	for(int j = 0; j < BLOCK_NUMBERS; j++) { // проходимся по каждому блоку
		int buffer = 0;
		for(int i = 0; i < N_TACT; i++) { 
			buffer += memorys[i].stateOfBlocks[j]; 
		}
		if(buffer == 0) {
			ROM.stateOfBlocks[j] = 0;
		} else {
			ROM.stateOfBlocks[j] = 1;
		}
	}
}

// Отладка
void showMemory(int memoryIndex) {
	std::cout << std::endl << std::endl << "memorys[" << memoryIndex + 1 << "].lifeTime: " << memorys[memoryIndex].lifeTime + 1 << ", memorys[" << memoryIndex + 1 << "].sizeBlocks: " <<  memorys[memoryIndex].sizeBlocks;

	for(int i = 0; i < N_TACT; i++) { 
		std::cout << std::endl << "memorys[" << i + 1 << "]: \t";
		for(int j = 0; j < BLOCK_NUMBERS; j++) {
			std::cout << memorys[i].stateOfBlocks[j]  << " ";
		}
	}

	std::cout << std::endl << "ROM: \t\t";
	for(int j = 0; j < BLOCK_NUMBERS; j++) {
		std::cout << ROM.stateOfBlocks[j]  << " ";
	}

	std::cout << std::endl << "SpareBlocks: \t";
	for(int j = 0; j < BLOCK_NUMBERS; j++) {
		std::cout << arrayOfSpareBlocks[j] << " ";
	}
}

bool firstFit() {
	for(int j = 0; j < N_TACT; j++) {
		memorys[j].reset();
	}
	ROM.reset();
	refreshArrayOfSpareBlocks();
	unsigned int unfitIndicator = N_TACT; // индикатор отказа
	for (int i = 0; i < N_TACT; i++) {
		memorys[i].lifeTime = (std::rand() % (N_TACT - i)) + 1; // случайные числа от 1 до 8 в 1-м такте
		memorys[i].sizeBlocks = ((std::rand() % N_BLOCKS) + 1) * SHEET;

		// пробегаемся по кусочкам свободных мест в arrayOfSpareBlocks
		for(int n = 0; n < BLOCK_NUMBERS; n++) { 
			if (arrayOfSpareBlocks[n] >= memorys[i].sizeBlocks) { 
				for(int b = 0; b < memorys[i].sizeBlocks; b++) {
					memorys[i].stateOfBlocks[n + b] = 1; // храним значения заполненных ячеек от индекса n
				}
				unfitIndicator--;
				break;
			}
		}

		refreshROM();
		refreshArrayOfSpareBlocks(); // должен идти после refreshROM()
		//showMemory(i); // для отладки
	}
	return (bool)unfitIndicator; // если выполнятся все 8 тактов моделировния, то функция вернёт false - индикатор успешного завершения работы алгоритма
}

bool worstFit() {
	for(int j = 0; j < N_TACT; j++) {
		memorys[j].reset();
	}
	ROM.reset();
	refreshArrayOfSpareBlocks();
	unsigned int unfitIndicator = N_TACT; // индикатор отказа
	for (int i = 0; i < N_TACT; i++) {
		memorys[i].lifeTime = (std::rand() % (N_TACT - i)) + 1; // случайные числа от 1 до 8 в 1-м такте
		memorys[i].sizeBlocks = ((std::rand() % N_BLOCKS) + 1) * SHEET;

		// ищем самый большой свободный участок в arrayOfSpareBlocks
		int maxSpaceIndex = 0;
		for(int n = 0; n < BLOCK_NUMBERS; n++) { 
			if (arrayOfSpareBlocks[n] > arrayOfSpareBlocks[maxSpaceIndex]) {
				maxSpaceIndex = n;
			}
		}
		if (arrayOfSpareBlocks[maxSpaceIndex] >= memorys[i].sizeBlocks) { 
			for(int b = 0; b < memorys[i].sizeBlocks; b++) {
				memorys[i].stateOfBlocks[maxSpaceIndex + b] = 1; // храним значения заполненных ячеек от индекса n
			}
			unfitIndicator--;
		}

		refreshROM();
		refreshArrayOfSpareBlocks(); // должен идти после refreshROM()
		//showMemory(i); // для отладки
	}
	return (bool)unfitIndicator; // если выполнятся все 8 тактов моделировния, то функция вернёт false - индикатор успешного завершения работы алгоритма
}

bool bestFit() {
	for(int j = 0; j < N_TACT; j++) {
		memorys[j].reset();
	}
	ROM.reset();
	refreshArrayOfSpareBlocks();
	unsigned int unfitIndicator = N_TACT; // индикатор отказа
	for (int i = 0; i < N_TACT; i++) {
		memorys[i].lifeTime = (std::rand() % (N_TACT - i)) + 1; // случайные числа от 1 до 8 в 1-м такте
		memorys[i].sizeBlocks = ((std::rand() % N_BLOCKS) + 1) * SHEET;

		// ищем самый большой свободный участок в arrayOfSpareBlocks
		int maxSpaceIndex = 0;
		for(int n = 0; n < BLOCK_NUMBERS; n++) { 
			if ((arrayOfSpareBlocks[n] >= memorys[i].sizeBlocks) && (arrayOfSpareBlocks[n] < arrayOfSpareBlocks[maxSpaceIndex])) {
				maxSpaceIndex = n;
			} else if ((arrayOfSpareBlocks[maxSpaceIndex] == 0) && (arrayOfSpareBlocks[n] >= memorys[i].sizeBlocks)) {
				maxSpaceIndex = n;
			}
		}
		if (arrayOfSpareBlocks[maxSpaceIndex] >= memorys[i].sizeBlocks) { 
			for(int b = 0; b < memorys[i].sizeBlocks; b++) {
				memorys[i].stateOfBlocks[maxSpaceIndex + b] = 1; // храним значения заполненных ячеек от индекса n
			}
			unfitIndicator--;
		}

		refreshROM();
		refreshArrayOfSpareBlocks(); // должен идти после refreshROM()
		//showMemory(i); // для отладки
	}
	return (bool)unfitIndicator; // если выполнятся все 8 тактов моделировния, то функция вернёт false - индикатор успешного завершения работы алгоритма
}

int main() {
	srand(time(NULL)); // для генерации действительно случайных значений std::rand()
	ROM.stateOfBlocks[BLOCK_NUMBERS] = '\n';
	arrayOfSpareBlocks[BLOCK_NUMBERS] = '\n';

	// инициализируем все возможные такты
	for(int i = 0; i < N_TACT; i++) {
		memory m;
		m.stateOfBlocks[BLOCK_NUMBERS] = '\n';
		memorys.push_back(m);
	}

	// First fit
	int refuseCount = 0;
	for (int i = 0; i < N_TEST; i++) {
		if (firstFit()) {
			refuseCount++; // фиксируем отказ
		}
	}
	std::cout << "Процент отказов алгоритма First fit: " << (refuseCount * 100 / N_TEST) << " %, число шагов моделирования: " << N_TEST << std::endl;

	// Worst fit
	refuseCount = 0;
	for (int i = 0; i < N_TEST; i++) {
		if (worstFit()) {
			refuseCount++; // фиксируем отказ
		}
	}
	std::cout << "Процент отказов алгоритма Worst fit: " << (refuseCount * 100 / N_TEST) << " %, число шагов моделирования: " << N_TEST << std::endl;

	// Best fit
	refuseCount = 0;
	for (int i = 0; i < N_TEST; i++) {
		if (bestFit()) {
			refuseCount++; // фиксируем отказ
		}
	}
	std::cout << "Процент отказов алгоритма Best fit: " << (refuseCount * 100 / N_TEST) << " %, число шагов моделирования: " << N_TEST << std::endl;
	
	return 0;
}