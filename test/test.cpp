#include <iostream>
#include <string>

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

int main() {
	std::string str1, str2;
	std::string code1, code2;
	std::string flight1, flight2;

	// Ввод 1-й строки от пользователя
	std::cout << "Введите первую строку: ";
	std::getline(std::cin, str1);

	if(str1.size() > 7 || str1.size() < 1) {
		std::cout << "Неверный размер первой строки." << std::endl;
		std::cout << "Максимальный размер строки — 7 символов. Минимальный - 1 символ." << std::endl;
		return 1;
	}

	if (!isValidStr(str1)) {
		std::cout << "Неверный формат первой строки." << std::endl;
		std::cout << "Строка может содержать латинские символы, цифры и пробел." << std::endl;
		return 1;
	}

	if (getCode(str1, code1)) {
		std::cout << "Неверный формат первой строки." << std::endl;
		std::cout << "См. п. \"Код авиакомпании\"." << std::endl;
		return 1;
	}

	flight1 = str1.substr(code1.size());
	// Удаляем лидирующие нули и пробелы из номера рейса
	removeFirstSpace(flight1);
    flight1 = removeLeadingZeros(flight1);

    if (!isNumericStr(flight1) || flight1.size() < 1) {
		std::cout << "Неверный формат первой строки." << std::endl;
		std::cout << "Номер рейса может быть представлен набором цифр от 0 до 9 размером от 1 до 5 цифры." << std::endl;
		return 1;
	}

	// Ввод 2-й строки от пользователя
	std::cout << "Введите вторую строку: ";
	std::getline(std::cin, str2);

	if(str2.size() > 7 || str2.size() < 1) {
		std::cout << "Неверный размер второй строки." << std::endl;
		std::cout << "Максимальный размер строки — 7 символов. Минимальный - 1 символ." << std::endl;
		return 1;
	}

	if (!isValidStr(str2)) {
		std::cout << "Неверный формат второй строки." << std::endl;
		std::cout << "Строка может содержать латинские символы, цифры и пробел." << std::endl;
		return 1;
	}

	if (getCode(str2, code2)) {
		std::cout << "Неверный формат второй строки." << std::endl;
		std::cout << "См. п. \"Код авиакомпании\"." << std::endl;
		return 1;
	}

	flight2 = str2.substr(code2.size());
	// Удаляем лидирующие нули и пробелы из номера рейса
	removeFirstSpace(flight2);
    flight2 = removeLeadingZeros(flight2);

    if (!isNumericStr(flight2) || flight2.size() < 1) {
		std::cout << "Неверный формат второй строки." << std::endl;
		std::cout << "Номер рейса может быть представлен набором цифр от 0 до 9 размером от 1 до 5 цифры." << std::endl;
		return 1;
	}

	std::cout << "1 Код авиакомпании: " << (code1.size()? code1 : "отсутствует") << ". Номер рейса " << flight1 << std::endl;
	std::cout << "2 Код авиакомпании: " << (code2.size()? code2 : "отсутствует") << ". Номер рейса " << flight2 << std::endl;
    // Сравниваем коды авиакомпаний и номера рейсов
    if (code1 == code2 && flight1 == flight2) {
		std::cout << "Строки равны." << std::endl;
	} else {
		std::cout << "Строки не равны." << std::endl;
	}

	return 0;
}

