#include "iostream"
#include "vector"
#include "string.h"
#define BLOCK_NUMBERS 64
#define SHEET 2
#define N_TACT 8
#define N_BLOCKS 8

bool ROM[BLOCK_NUMBERS] = {0};
int arrayOfSpareBlocks [BLOCK_NUMBERS];

void detektSpareBlocks() {
    int sizeOfarrayOfSpareBlocks = 0;
    int consecutiveCount = 0;

    for (int i = 0; i < BLOCK_NUMBERS; ++i) {
    	// если элемент массива А = 0, то подсчитывается количество свободных секторов подряд
        if (!ROM[i]) { 
            consecutiveCount++;
            sizeOfarrayOfSpareBlocks++;
        } else {
            if (consecutiveCount > 0) {
                arrayOfSpareBlocks[sizeOfarrayOfSpareBlocks++] = consecutiveCount;
                consecutiveCount = 0;
            }
        }
    }

    // Добавим последнюю последовательность, если она заканчивается true
    if (consecutiveCount > 0) {
        arrayOfSpareBlocks[sizeOfarrayOfSpareBlocks++] = consecutiveCount;
    }
}

class memory {
	public:
	bool stateOfBlocks [BLOCK_NUMBERS] = {0}; // текущее состояние памяти
	int sizeBlocks;
	int lifeTime;
	
	void printM() {
		std::cout << "ОК" << std::endl;
	}

	void reset() {
		memset(stateOfBlocks, 0, BLOCK_NUMBERS*sizeof(int));
		sizeBlocks = 0;
		lifeTime = 0;
	}
};
	
std::vector<memory> memorys;

void refreshMemory() {
	int buffer;
	for(int j = 0; j < BLOCK_NUMBERS; j++) { // проходимся по каждому блоку
		buffer = 0;
		for(int i = 0; i < N_TACT; i++) { // просматриваем каждый такт
			memorys[i].lifeTime--;
			if(memorys[i].lifeTime > 0) {
				buffer += memorys[i].stateOfBlocks[j];
			}
		}
		if(buffer == 0) {
			ROM[j] = 0;
		} else {
			ROM[j] = 1;
		}
	}
}

int main() {

	ROM[BLOCK_NUMBERS] = '\n';
	arrayOfSpareBlocks[BLOCK_NUMBERS] = '\n';

	memory m;
	// инициализируем все возможные такты
	for(int i = 0; i < N_TACT; i++) {
		m.stateOfBlocks[BLOCK_NUMBERS - 1] = '\n';
		memorys.push_back(m);
	}

	/*
	for(int i = 0; i < N_TACT; i++) {
		memorys[i].printM();
	}
	*/

	// First fit
	for (int i = 0; i < N_TACT; i++) {
		refreshMemory();
		detektSpareBlocks();
		memorys[i].lifeTime = rand() % N_TACT;
		memorys[i].sizeBlocks = (rand() % N_BLOCKS + 1) * SHEET;

		for(int n = 0; n < BLOCK_NUMBERS; n++) { // пробегаемся по кусочкам свободных мест
			if (arrayOfSpareBlocks[n] >= memorys[i].sizeBlocks) { 
				for(int b = 0; b < memorys[i].sizeBlocks; b++) {
					memorys[i].stateOfBlocks[n + b] = 1; // храним значения заполненных ячеек от индекса n
					ROM[n + b] = 1; // заполняем ROM от индекса n
				}
				std::cout << "Блок подошёл" << std::endl;
				break;
			} else {
				std::cout << "Блок НЕ влез" << std::endl;
			}
		}
	}

	return 0;
}

