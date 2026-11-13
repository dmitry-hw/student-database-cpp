#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

struct TZap {
    char FIO[30];       // Фамилия и инициалы
    int god;            // Год рождения (1950..2010)
    char grupa[15];     // Номер группы
    int fiz;            // Оценка по физике 
    int mat;            // Оценка по математике 
    int inf;            // Оценка по информатике 
    int xim;            // Оценка по химии 
    double s_b;         // Средний балл
};

void Out(TZap z, FILE *Ft);
void OutHeader(FILE *Ft);
void InputZap(TZap *z);
int CheckBukvaVGD(char c);

// функции защиты ввода
int InputInt(const char* prompt, int minVal, int maxVal);
void InputFIO(const char* prompt, char* dest, int maxLen);
void InputGroup(const char* prompt, char* dest);

int main() {
SetConsoleOutputCP (65001);
SetConsoleCP(65001);
    int kod, sub_kod, confirm;
    TZap currentZap;
    char target_grupa[15];
    const char* File_Zap = "zapisi.dat";
    const char* File_Rez = "rezult.txt";
    int size = sizeof(TZap);

    FILE *Ft = fopen(File_Rez, "w");
    if (Ft == NULL) {
        printf("\nОшибка создания текстового файла отчета!\n");
        return 1;
    }

    while (1) {
        printf("\n================ МЕНЮ ================\n");
        printf(" 1 - Создание (Create)\n");
        printf(" 2 - Просмотр (View)\n");
        printf(" 3 - Коррекция (добавление или редактирование)\n");
        printf(" 4 - Решение индивидуального задания (Вариант 6)\n");
        printf(" 5 - Сортировка по среднему баллу\n");
        printf(" 0 - ВЫХОД (EXIT)\n");
        printf("======================================");

        kod = InputInt("\nВыберите пункт меню (0..5): ", 0, 5);

        switch (kod) {
        case 1: {
            FILE *Fz = fopen(File_Zap, "rb");
            if (Fz != NULL) {
                fseek(Fz, 0, SEEK_END);
                long len = ftell(Fz);
                fclose(Fz);
                if (len > 0) {
                    printf("\n ВНИМАНИЕ: Файл уже существует и содержит данные!\n");
                    confirm = InputInt(" Перезаписать файл и удалить старые записи? (1 - Да, 0 - Нет): ", 0, 1);
                    if (confirm == 0) {
                        printf(" Создание отменено. Старые данные сохранены.\n");
                        break;
                    }
                }
            }

            Fz = fopen(File_Zap, "wb");
            if (Fz == NULL) {
                printf("\nОшибка создания бинарного файла!\n");
                break;
            }
            fclose(Fz);
            printf("\nСоздан новый пустой файл %s!\n", File_Zap);
            fprintf(Ft, "\n--- Создан новый файл %s ---\n", File_Zap);
            break;
        }

        case 2: {
            FILE *Fz = fopen(File_Zap, "rb");
            if (Fz == NULL) {
                printf("\nФайл не найден! Сначала создайте его (пункт 1).\n");
                break;
            }

            printf("\n\t--------- Informations (Содержимое файла) ---------\n");
            fprintf(Ft, "\n\t--------- Informations (Содержимое файла) ---------\n");
            OutHeader(Ft);

            while (fread(&currentZap, size, 1, Fz) == 1) {
                Out(currentZap, Ft);
            }
            fclose(Fz);
            break;
        }

        case 3: {
            printf("\n --- КОРРЕКЦИЯ ---\n");
            printf(" 1 - Добавить новую запись в конец файла\n");
            printf(" 2 - Редактировать существующую запись по номеру\n");
            printf(" 0 - Отмена\n");
            sub_kod = InputInt(" Ваш выбор (0..2): ", 0, 2);

            if (sub_kod == 0) break;
            if (sub_kod == 1) {
                FILE *Fz = fopen(File_Zap, "ab");
                if (Fz == NULL) {
                    printf("\nОшибка открытия файла для дозаписи!\n");
                    break;
                }
                InputZap(&currentZap);
                fwrite(&currentZap, size, 1, Fz);
                fclose(Fz);
                printf("Запись успешно добавлена в конец файла!\n");
            }
            else if (sub_kod == 2) {
                FILE *Fz = fopen(File_Zap, "r+b");
                if (Fz == NULL) {
                    printf("\nФайл не найден! Сначала создайте его (пункт 1).\n");
                    break;
                }

                fseek(Fz, 0, SEEK_END);
                long len = ftell(Fz);
                int kol = (int)(len / size);

                if (kol == 0) {
                    printf("Файл пуст, редактировать нечего!\n");
                    fclose(Fz);
                    break;
                }

                printf("\nВсего записей в файле: %d.\n", kol);
                int nomer = InputInt("Введите номер записи для изменения: ", 1, kol);

                printf("\nВведите новые данные для студента №%d:\n", nomer);
                InputZap(&currentZap);

                fseek(Fz, (long)(nomer - 1) * size, SEEK_SET);
                fwrite(&currentZap, size, 1, Fz);
                fclose(Fz);
                printf("Запись успешно отредактирована!\n");
            }
            break;
        }

        case 4: {
            FILE *Fz = fopen(File_Zap, "rb");
            if (Fz == NULL) {
                printf("\nФайл не найден! Сначала создайте его (пункт 1).\n");
                break;
            }

            InputGroup("\nВведите номер интересующей группы: ", target_grupa);

            printf("\n\t--- РЕЗУЛЬТАТ ВАРИАНТА 6 (Группа: %s, Фамилии на В, Г, Д) ---\n", target_grupa);
            fprintf(Ft, "\n\t--- РЕЗУЛЬТАТ ВАРИАНТА 6 (Группа: %s, Фамилии на В, Г, Д) ---\n", target_grupa);
            OutHeader(Ft);

            int foundCount = 0;
            while (fread(&currentZap, size, 1, Fz) == 1) {
                if (strcmp(currentZap.grupa, target_grupa) == 0 && CheckBukvaVGD(currentZap.FIO[0])) {
                    Out(currentZap, Ft);
                    foundCount++;
                }
            }
            fclose(Fz);

            if (foundCount == 0) {
                printf("Студенты по заданному условию не найдены.\n");
                fprintf(Ft, "Студенты по заданному условию не найдены.\n");
            }
            break;
        }

        case 5: {
            FILE *Fz = fopen(File_Zap, "rb");
            if (Fz == NULL) {
                printf("\nФайл не найден! Сначала создайте его (пункт 1).\n");
                break;
            }

            fseek(Fz, 0, SEEK_END);
            long len = ftell(Fz);
            int kol = (int)(len / size);
            rewind(Fz);

            if (kol == 0) {
                printf("Файл пуст! Сортировать нечего.\n");
                fclose(Fz);
                break;
            }

            TZap* mas_Z = new TZap[kol];
            for (int i = 0; i < kol; i++) {
                fread(&mas_Z[i], size, 1, Fz);
            }
            fclose(Fz);

            for (int i = 0; i < kol - 1; i++) {
                for (int j = i + 1; j < kol; j++) {
                    if (mas_Z[i].s_b > mas_Z[j].s_b) {
                        TZap temp = mas_Z[i];
                        mas_Z[i] = mas_Z[j];
                        mas_Z[j] = temp;
                    }
                }
            }

            printf("\n\t--------- S O R T (по возрастанию ср. балла) ---------\n");
            fprintf(Ft, "\n\t--------- S O R T (по возрастанию ср. балла) ---------\n");
            OutHeader(Ft);

            for (int i = 0; i < kol; i++) {
                Out(mas_Z[i], Ft);
            }

            delete[] mas_Z;
            break;
        }
        // ---------------- 0. ВЫХОД ----------------
        case 0:
            fclose(Ft);
            printf("\nРабота завершена. Все результаты сохранены в файл %s\n", File_Rez);
            return 0;
        }
    }
}

int InputInt(const char* prompt, int minVal, int maxVal) {
    char buf[256];
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }

        if (strchr(buf, '\n') == NULL) {
            int ch;
            while ((ch = getchar())!='\n' && ch != EOF);
            // этот цикл просто чистит символы за лимитом, т.к. getchar() не возвращает символы
            printf(" [Ошибка]: Слишком длинный ввод! Повторите попытку.\n");
            continue;
        }

        buf[strcspn(buf, "\n")]='\0';

        int i = 0;
        while (buf[i] == ' ' || buf[i] == '\t') i++;

        if (buf[i] == '\0') {
            printf(" [Ошибка]: Вы ничего не ввели! Введите число.\n");
            continue;
        }

        int start = i;
        if (buf[i] == '-' || buf[i] == '+') i++;
        if (buf[i] == '\0') {
            printf(" [Ошибка]: Введите цифры, а не просто знак!\n");
            continue;
        }
        
        int valid = 1;
        int digitCount = 0;
        for (; buf[i] != '\0'; i++) {
            if (buf[i] < '0' || buf[i] > '9') {
                valid = 0;
                break;
            }
            digitCount++;
        }

        if (!valid) {
            // для компилятора любое ненулевое значение истинно
            printf(" [Ошибка]: Недопустимые символы (буквы, ';', пробелы)! Введите целое число.\n");
            continue;
        }

        if (digitCount > 9) {
            printf(" [Ошибка]: Число слишком большое! Диапазон: от %d до %d.\n", minVal, maxVal);
            continue;
        }

        long val = strtol(buf + start, NULL, 10);
        //buf+start значит сдвинутся от начала на start элементов
        if (val < minVal || val > maxVal) {
            printf(" [Ошибка]: Число вне диапазона! Введите значение от %d до %d.\n", minVal, maxVal);
            continue;
        }

        return (int)val;
    }
}

void InputFIO(const char* prompt, char* dest, int maxLen) {
    char buf[256];
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }

        if (strchr(buf, '\n') == NULL) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            printf(" [Ошибка]: Слишком длинная строка!\n");
            continue;
        }

        buf[strcspn(buf, "\n")] = '\0';

        int start = 0;
        while (buf[start] == ' ' || buf[start] == '\t') start++;

        int len = (int)strlen(buf + start);
        if (len < 2) {
            printf(" [Ошибка]: ФИО слишком короткое! (например, Война Д.В.).\n");
            continue;
        }
        if (len >= maxLen) {
            printf(" [Ошибка]: ФИО слишком длинное (максимум %d символов)!\n", maxLen - 1);
            continue;
        }

        unsigned char first = (unsigned char)buf[start];
        int firstIsLetter = (first >= 'A' && first <= 'Z') || (first >= 'a' && first <= 'z') ||
                            (first >= 192 && first <= 255) || first == 168 || first == 184;
        if (!firstIsLetter) {
            printf(" [Ошибка]: Фамилия должна начинаться с буквы!\n");
            continue;
        }

        int valid = 1;
        for (int i = start; buf[i] != '\0'; i++) {
            unsigned char c = (unsigned char)buf[i];
            int isRus = (c >= 192 && c <= 255) || c == 168 || c == 184;
            int isEng = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
            int isAllowedPunct = (c == ' ' || c == '.' || c == '-');

            if (!isRus && !isEng && !isAllowedPunct) {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            printf(" [Ошибка]: В ФИО запрещены цифры и спецсимволы (';', ',', '/' и т.д.)!\n");
            continue;
        }
        strcpy(dest, buf + start);
        return;
    }
}

void InputGroup(const char* prompt, char* dest) {
    int grpNum = InputInt(prompt, 1, 1000000);
    sprintf(dest, "%d", grpNum);
}

void InputZap(TZap *z) {
    printf("\n --- Ввод анкетных данных студента ---\n");
    InputFIO(" Фамилия И.О. (например, Война Д.В.): ", (*z).FIO, sizeof((*z).FIO));
    (*z).god = InputInt(" Год рождения (1950..2010): ", 1950, 2010);
    InputGroup(" Номер группы (число 1..1000000): ", (*z).grupa);

    printf(" --- Ввод оценок за семестр (от 1 до 10) ---\n");
    (*z).fiz = InputInt("   Физика (1..10): ", 1, 10);
    (*z).mat = InputInt("   Математика (1..10): ", 1, 10);
    (*z).inf = InputInt("   Информатика (1..10): ", 1, 10);
    (*z).xim = InputInt("   Химия (1..10): ", 1, 10);

    (*z).s_b = ((*z).fiz + (*z).mat + (*z).inf + (*z).xim) / 4.0;
}

void OutHeader(FILE *Ft) {
    printf("---------------------------------------------------------------------------\n");
    printf("| %-20s | %-4s | %-8s | %-3s | %-3s | %-3s | %-3s | %-6s |\n",
           "Фамилия И.О.        ", "Год ", "Группа  ", "Физ", "Мат", "Инф", "Хим", "Ср.Б. ");
    printf("---------------------------------------------------------------------------\n");

    fprintf(Ft, "---------------------------------------------------------------------------\n");
    fprintf(Ft, "| %-20s | %-4s | %-8s | %-3s | %-3s | %-3s | %-3s | %-6s |\n",
            "Фамилия И.О.        ", "Год ", "Группа  ", "Физ", "Мат", "Инф", "Хим", "Ср.Б. ");
    fprintf(Ft, "---------------------------------------------------------------------------\n");
}

void Out(TZap z, FILE *Ft) {
    printf("| %-20s | %-4d | %-8s | %-3d | %-3d | %-3d | %-3d | %6.3lf |\n",
           z.FIO, z.god, z.grupa, z.fiz, z.mat, z.inf, z.xim, z.s_b);
    fprintf(Ft, "| %-20s | %-4d | %-8s | %-3d | %-3d | %-3d | %-3d | %6.3lf |\n",
            z.FIO, z.god, z.grupa, z.fiz, z.mat, z.inf, z.xim, z.s_b);
}

int CheckBukvaVGD(char c) {
    unsigned char uc = (unsigned char)c;
    // Проверка по кодам для В(192/224), Г(193/225), Д(194/226) и латиницы
    if (uc == 192 || uc == 224 || uc == 'B' || uc == 'b' || uc == 'V'||  uc == 'v' || // В
        uc == 193 || uc == 225 || uc == 'G'||  uc == 'g' ||                           // Г
        uc == 194 || uc == 226 || uc == 'D' || uc == 'd') {                           // Д
        return 1;
    }
    return 0;
}