// KURSOVAYA.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// Разработка обучающих приложений, демонстрационных приложений, приложений для тестирования знаний.
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <algorithm>
#include <random>
using namespace std;

// СТРУКТУРЫ
struct ContinentStats {
    string name;
    string file_name;
    int total_asked = 0;// Сколько всего вопросов по континенту задали за все время
    int total_score = 0;// Сколько раз ответили верно за все время
};
struct Country {
    string name;
    string capital;
    string continent;
    int  difficulty = 0; // 1-легкий, 2-средний, 3-сложный, 4-эксперт
};

// ГЛОБАЛЬНЫЕ переменные
const int MAX_COUNTRIES = 200;
const int NUM_CONTINENTS = 6;
int total_countries = 0;
int last_score = 0; // Количество правильных ответов
int last_total = 0; // Всего вопросов было задано
Country countries[MAX_COUNTRIES];
ContinentStats stats[NUM_CONTINENTS];

// ДОП функции 
// Функция для удаления лишних пробелов и спецсимволов по краям строки
void clean(string& s) {
    if (s.empty()) return;
    // справа
    s.erase(s.find_last_not_of(" \n\r\t") + 1);
    // слева
    s.erase(0, s.find_first_not_of(" \n\r\t"));
}
// Защита от ввода букв вместо цифр в меню
int get_int_input() {
    int choice;
    while (!(cin >> choice)) {
        cin.clear(); // Сбрасываем флаг ошибки
        cin.ignore(10000, '\n');
        cout << "Ошибка! Введите число: ";
    }
    return choice;
}

// ОСНОВНЫЕ функции
void print_menu() {
    cout << "\n=========================================\n";
    cout << "                 меню                  \n";
    cout << "=========================================\n";
    cout << "  1. Учиться\n";
    cout << "  2. Проходить тест\n";
    cout << "  3. Мой результат\n";
    cout << "  4. Выход\n";
    cout << "=========================================\n";
    cout << "Ваш выбор: ";
}
void Init_Stats_Array() {
    string continents_rus[NUM_CONTINENTS] = { "Европа", "Азия", "Африка", "Северная Америка", "Южная Америка", "Австралия и Океания" };
    string continents_file[NUM_CONTINENTS] = { "Европа", "Азия", "Африка", "Северная_Америка", "Южная_Америка", "Австралия_и_Океания" };

    for (int i = 0; i < NUM_CONTINENTS; i++) {
        stats[i].name = continents_rus[i];
        stats[i].file_name = continents_file[i];
        stats[i].total_asked = 0;
        stats[i].total_score = 0;
    }
}
void Save_Stats() {
    ofstream f("stats.txt");
    if (!f) {
        cout << "[Ошибка] Не удалось сохранить статистику в файл!\n";
        return;
    }

    for (int i = 0; i < NUM_CONTINENTS; i++) {
        // Записываем в строчку: Имя_для_файла Всего_вопросов Правильных_ответов
        f << stats[i].file_name << " " << stats[i].total_asked << " " << stats[i].total_score << "\n";
    }
    f.close();
}
void Load_Stats() {

    Init_Stats_Array();
    ifstream f("stats.txt");
    if (!f) return;

    string f_name;
    int asked, score;

    while (f >> f_name >> asked >> score) {
        for (int i = 0; i < NUM_CONTINENTS; i++) {
            if (stats[i].file_name == f_name) {
                stats[i].total_asked = asked;
                stats[i].total_score = score;
                break;
            }
        }
    }
    f.close();
}
void Learning() {
    if (total_countries == 0) {
        cout << "\n[Ошибка] Список стран пуст. Сначала загрузите файл!\n\n";
        return;
    }

    cout << "\n=========================================\n";
    cout << "           ВЫБЕРИТЕ ТИП ФИЛЬТРА          \n";
    cout << "=========================================\n";
    cout << "  1. Сортировка по сложности\n";
    cout << "  2. Сортировка по континентам\n";
    cout << "=========================================\n";
    cout << "Ваш выбор: ";

    int filter_type = get_int_input();
    int diff_choice = 0;
    string cont_choice = "";

    if (filter_type == 1) {
        cout << "\nВыберите уровень сложности (1-Легкий, 2-Средний, 3-Сложный, 4-Эксперт, 5-Все): ";
        diff_choice = get_int_input();
        cin.ignore(1000, '\n');
    }
    else if (filter_type == 2) {
        cout << "\nВведите название континента с большой буквы\n";
        cout << "(Европа, Азия, Африка, Северная Америка, Южная Америка, Австралия и Океания): ";
        cin.ignore(1000, '\n');
        getline(cin, cont_choice);
        clean(cont_choice);
    }
    else {
        cout << "\nНекорректный выбор! Возврат в меню.\n\n";
        return;
    }

    cout << "\n=========================================\n";
    cout << "            РЕЖИМ ОБУЧЕНИЯ               \n";
    cout << "=========================================\n";

    int shown_count = 0;

    for (int i = 0; i < total_countries; i++) {
        // Фильтр по сложности
        if (filter_type == 1 && diff_choice != 5 && countries[i].difficulty != diff_choice) {
            continue;
        }
        // Фильтр по континенту
        if (filter_type == 2 && countries[i].continent != cont_choice) {
            continue;
        }

        shown_count++;

        cout << "Страна №" << shown_count << " [" << countries[i].continent << "]\n";
        cout << "--> " << countries[i].name << "\n";
        cout << "--> Нажмите Enter, чтобы узнать столицу...";
        cin.get();

        cout << "Ответ: " << countries[i].capital << "\n";
        cout << "-----------------------------------------\n";
        cout << "--> Нажмите Enter для продолжения...";
        cin.get();
        cout << "\n";
    }

    if (shown_count == 0) {
        cout << "[Предупреждение] Страны с такими параметрами не найдены!\n\n";
    }
    else {
        cout << "=========================================\n";
        cout << " Вы повторили все выбранные страны (" << shown_count << ")! \n";
        cout << "=========================================\n\n";
    }
}
void Testing() {
    if (total_countries == 0) {
        cout << "\n[Ошибка] Список стран пуст. Сначала загрузите файл!\n\n";
        return;
    }

    // Перемешиваем страны перед тестом
    random_device rd;
    mt19937 g(rd());
    shuffle(countries, countries + total_countries, g);

    cout << "\n=========================================\n";
    cout << "           ВЫБЕРИТЕ ТИП ФИЛЬТРА          \n";
    cout << "=========================================\n";
    cout << "  1. Тест по сложности\n";
    cout << "  2. Тест по континентам\n";
    cout << "=========================================\n";
    cout << "Ваш выбор: ";

    int filter_type = get_int_input();
    int diff_choice = 0;
    string cont_choice = "";

    if (filter_type == 1) {
        cout << "\nВыберите уровень сложности (1-Легкий, 2-Средний, 3-Сложный, 4-Эксперт, 5-Все): ";
        diff_choice = get_int_input();
        cin.ignore(1000, '\n');
    }
    else if (filter_type == 2) {
        cout << "\nВведите название континента с большой буквы\n";
        cout << "(Европа, Азия, Африка, Северная Америка, Южная Америка, Австралия и Океания): ";
        cin.ignore(1000, '\n');
        getline(cin, cont_choice);
        clean(cont_choice);
    }
    else {
        cout << "\nНекорректный выбор! Возврат в меню.\n\n";
        return;
    }

    cout << "\n=========================================\n";
    cout << "               НАЧАЛО ТЕСТА              \n";
    cout << "=========================================\n";
    cout << " Вводите название столицы с большой буквы!\n";
    cout << "=========================================\n\n";

    int correct_answers = 0;
    int asked_count = 0;

    for (int i = 0; i < total_countries; i++) {
        // Применяем фильтры
        if (filter_type == 1 && diff_choice != 5 && countries[i].difficulty != diff_choice) {
            continue;
        }
        if (filter_type == 2 && countries[i].continent != cont_choice) {
            continue;
        }

        asked_count++;

        int idx = -1;
        for (int j = 0; j < NUM_CONTINENTS; j++) {
            if (stats[j].name == countries[i].continent) {
                idx = j;
                break;
            }
        }

        if (idx != -1) stats[idx].total_asked++;
  
        cout << "Вопрос №" << asked_count << " (Континент: " << countries[i].continent << ")\n";
        cout << "Какая столица у страны: " << countries[i].name << "?\n";
        cout << "Ваш ответ: ";

        string user_answer;
        getline(cin, user_answer);
        clean(user_answer);

        if (user_answer == countries[i].capital) {
            cout << "[+] Верно!\n\n";
            correct_answers++;
            // Если ответ верный — добавляем балл этому континенту в вечную статистику
            if (idx != -1) stats[idx].total_score++;
        }
        else {
            cout << "[-] Ошибка! Правильный ответ: " << countries[i].capital << "\n\n";
        }
    }

    // Сохраняем результаты текущей сессии (для быстрого вывода)
    last_score = correct_answers;
    last_total = asked_count;

    if (asked_count == 0) {
        cout << "[Предупреждение] Страны с такими параметрами не найдены!\n\n";
    }
    else {
        cout << "=========================================\n";
        cout << " ТЕСТ ЗАВЕРШЕН! \n";
        cout << " Ваш результат в этом тесте: " << correct_answers << " из " << asked_count << "\n";

        double percent = (double)correct_answers / asked_count * 100;
        cout << " Процент правильных ответов: " << percent << "%\n";
        cout << "=========================================\n";

        Save_Stats();
        cout << " [Инфо] Общая статистика успешно обновлена в stats.txt!\n";
        cout << "=========================================\n\n";
    }
}
void Show_Results() {
    cout << "\n=======================================================\n";
    cout << "                  МОЯ СТАТИСТИКА                       \n";
    cout << "=======================================================\n";

    if (last_total > 0) {
        double current_percent = (double)last_score / last_total * 100;
        cout << " Итоги последнего теста: " << last_score << " из " << last_total
            << " (" << current_percent << "%)\n";
        cout << "-------------------------------------------------------\n";
    }
    cout << " Континент            | Вопросы   | Верно     | %\n";
    cout << "-------------------------------------------------------\n";

    int grand_total_asked = 0;
    int grand_total_score = 0;

    for (int i = 0; i < NUM_CONTINENTS; i++) {
        double percent = 0;
        if (stats[i].total_asked > 0) {
            percent = (double)stats[i].total_score / stats[i].total_asked * 100;
        }
        // ТАБЛИЦА
        // название континента
        cout << " " << stats[i].name;
        // ширина столбца 21 символ
        int name_len = stats[i].name.length();
        for (int s = 0; s < 21 - name_len; s++) {
            cout << " ";
        }

        // количество вопросов
        cout << "| " << stats[i].total_asked;
        int asked_len = to_string(stats[i].total_asked).length();
        for (int s = 0; s < 10 - asked_len; s++) {
            cout << " ";
        }

        // правильные ответы
        cout << "| " << stats[i].total_score;
        int score_len = to_string(stats[i].total_score).length();
        for (int s = 0; s < 10 - score_len; s++) {
            cout << " ";
        }

        // процент
        cout << "| " << percent << "%\n";

        grand_total_asked += stats[i].total_asked;
        grand_total_score += stats[i].total_score;
    }

    cout << "-------------------------------------------------------\n";
    double total_percent = 0;
    if (grand_total_asked > 0) {
        total_percent = (double)grand_total_score / grand_total_asked * 100;
    }

    cout << " ВСЕГО ЗА ВСЕ ВРЕМЯ   | " << grand_total_asked;

    int grand_asked_len = to_string(grand_total_asked).length();
    for (int s = 0; s < 10 - grand_asked_len; s++) cout << " ";

    cout << "| " << grand_total_score;

    int grand_score_len = to_string(grand_total_score).length();
    for (int s = 0; s < 10 - grand_score_len; s++) cout << " ";

    cout << "| " << total_percent << "%\n";
    cout << "=======================================================\n\n";
}
void menu(){

    // Главный цикл
    int choice = 0;
    do {
        print_menu();
        choice = get_int_input();
        switch (choice) {
        case 1:
            Learning();
            break;
        case 2:
            Testing();
            break;
        case 3:
            Show_Results();
            break;
        case 4: cout << "До свидания!\n";           
            break;
        default:
            cout << "Введите корректный номер:\n";
        }
    } while (choice != 4);
}

void Load_Countries(const char* filename) {
    ifstream f(filename);

    if (!f) {
        cout << "Файл не найден!\n";
        return;
    }

    int count = 0;
    int currentDiff = 0;

    string line;

    while (getline(f, line)) {

        if (line.empty()) continue;

        if (line.find("ЛЕГКИЙ") != string::npos) { currentDiff = 1; continue; }
        if (line.find("СРЕДНИЙ") != string::npos) { currentDiff = 2; continue; }
        if (line.find("СЛОЖНЫЙ") != string::npos) { currentDiff = 3; continue; }
        if (line.find("ЭКСПЕРТ") != string::npos) { currentDiff = 4; continue; }

        if (line.length() >= 3) {
            if (line.substr(0, 3) == "Q: ") {
                countries[total_countries].name = line.substr(3);
                clean(countries[total_countries].name);
                countries[total_countries].difficulty = currentDiff;
            }
            else if (line.substr(0, 3) == "C: ") {
                countries[total_countries].continent = line.substr(3);
                clean(countries[total_countries].continent);
            }
            else if (line.substr(0, 3) == "A: ") {
                countries[total_countries].capital = line.substr(3);
                clean(countries[total_countries].capital);

                total_countries++;
            }
        }
    }
        f.close();
}
void Load_Data()
{
    Load_Countries("questions.txt");
    cout << "Загружено стран: " << total_countries << "\n\n";
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    Load_Stats();
    Load_Data();
    menu();
    return 0;
}