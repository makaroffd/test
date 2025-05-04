#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <thread>
#include <vector>

bool isAlphanumericSpace(char ch) {
    if (std::isalnum(ch) || std::isspace(ch)) {
        return true;
    }
    return false;
}

// Функция проверяющая что строка содержит только [A-Z 0-9] space
bool isValidStr(const std::string& str) {
	if (str.empty()) {
		return false;
	}
	
	for (char ch : str) {
		if (!isAlphanumericSpace(ch)) {
			return false;
		}
	}
	return true;
}


// Функция проверяющая что строка содержит только [0-9]
bool isNumericStr(const std::string& str) {
	if (str.empty()) {
		return false;
	}
	
	for (char ch : str) {
		if (!std::isdigit(ch)) {
			return false;
		}
	}
	return true;
}

// Функция для получения кода авиакомпании 
// принимает две строки: входную и строку для результата (кода)
// возвращае 0 при успехе и 1 при неравильных первых символах
// На всякий случай приводим все буквы к заглавным
int getCode(const std::string& str, std::string& output) {
	int ret = 0;
	if (str.empty()) {
		return 1;
	}
	
	if (std::isdigit(str[0]) && (std::isdigit(str[1]) || 1 == str.size())) {
		// Код отсутствует
	} else if (std::isalpha(str[0]) && std::isalpha(str[1])) {
		int tmp_key_lenght = 2;
		if (std::isalpha(str[2])) {
			if (!std::isdigit(str[3])) {
				// Код вида ([A-Z] [A-Z] [A-Z] ![0-9])
				return 1;
			}
			
			// Код вида ([A-Z] [A-Z] [A-Z])
			tmp_key_lenght = 3;
		}
		// Код вида ([A-Z] [A-Z] space) или ([A-Z] [A-Z])
		for (int i = 0; i < tmp_key_lenght; ++i) {
			auto ch = str[i];
			output += std::toupper(ch);
		}
	} else if ((std::isdigit(str[0]) && std::isalpha(str[1])) || (std::isalpha(str[0]) && std::isdigit(str[1]))){
		// Код вида ([A-Z 0-9] [A-Z 0-9] space)  или ([A-Z 0-9] [A-Z 0-9])
		for (int i = 0; i < 2; ++i) {
			auto ch = str[i];
			if (std::isalpha(ch)) {
				output += std::toupper(ch);
			} else {
				output += ch;
			}
		}
	} else {
		ret = 1;
	}
	
	return ret;
}

// Функция для удаления пробела (который пришёл из кода авиакомпании) в номере рейса
void removeFirstSpace(std::string& str) {
    if (!str.empty() && std::isspace(str[0])) {
        // Удаляем первый символ (пробел)
        str.erase(0, 1);
    }
}

// Функция для удаления лидирующих нулей в номере рейса
std::string removeLeadingZeros(const std::string& str) {
    std::string result;
    bool leadingZero = true;
    for (char c : str) {
        if (c == '0' && leadingZero) {
            continue;
        }
        result += c;
        leadingZero = false;
    }
    return result;
}

// Функция для обработки файла и сохранения уникальных комбинаций в другой файл
void processFile(const std::string& inputFileName, const std::string& outputFileName) {
	std::ifstream inFile(inputFileName);
	std::ofstream outFile(outputFileName);
	std::unordered_set<std::string> uniqueCombinations;

	if (!inFile.is_open() || !outFile.is_open()) {
		std::cerr << "Error opening files" << std::endl;
		return;
	}
	
	// Для упрощения предпологаем, что все строки в файле правильные (не проверяем формат, размер итп)

	std::string line;
	while (std::getline(inFile, line)) {
		std::string code, flight;
		
		if(line.size() > 7 || line.size() < 1) {
			std::cout << "Ошибка в записи" << "\"" << line << "\"." << std::endl;
			std::cout << "Максимальный размер строки — 7 символов. Минимальный - 1 символ." << std::endl;
			return;
		}
		
		if (!isValidStr(line)) {
			std::cout << "Ошибка в записи" << "\"" << line << "\"." << std::endl;
			std::cout << "Строка может содержать латинские символы, цифры и пробел." << std::endl;
			return;
		}
	
		if (getCode(line, code)) {
			std::cout << "Ошибка в записи" << "\"" << line << "\"." << std::endl;
			std::cout << "См. п. \"Код авиакомпании\"." << std::endl;
			return;
		}
		
		flight = line.substr(code.size());
		// Удаляем лидирующие нули и пробел из номера рейса
		removeFirstSpace(flight);
		flight = removeLeadingZeros(flight);
		
		if (!isNumericStr(flight) || flight.size() < 1) {
			std::cout << "Ошибка в записи" << "\"" << line << "\"." << std::endl;
			std::cout << "Номер рейса может быть представлен набором цифр от 0 до 9 размером от 1 до 5 цифры." << std::endl;
			return;
		}
		
		std::string combination = code + flight;
		// Если комбинация не встречалась запоминаем её и записываем в файл
		if (uniqueCombinations.find(combination) == uniqueCombinations.end()) {
			uniqueCombinations.insert(combination);
			outFile << combination << std::endl;
		}
	}

	inFile.close();
	outFile.close();
}

int main() {
	// Вектор для хранения потоков
	std::vector<std::thread> threads;

	// Создаем потоки для обработки файлов
	threads.emplace_back(processFile, "1_in.txt", "1_out.txt");
	threads.emplace_back(processFile, "2_in.txt", "2_out.txt");

	// Ждем завершения всех потоков
	for (auto& thread : threads) {
		thread.join();
	}

	return 0;
}

