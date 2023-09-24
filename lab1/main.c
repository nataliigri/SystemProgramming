#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isVowel(char c) {
    switch (c) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
        case 'A':
        case 'E':
        case 'I':
        case 'O':
        case 'U':
            return 1; // Якщо це голосна, повертаємо 1
        default:
            return 0; // Якщо це не голосна, повертаємо 0
    }
}

int main() {
    FILE *inputFile = fopen("/Users/nataliiagricisin/Desktop/System Programming/lab1/input2.txt", "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Unable to open input file\n");
        return 1;
    }

    char word[31]; // 30 символів для слова, плюс символ завершення строки
    char uniqueWords[1000][31]; // Масив для зберігання унікальних слів
    int uniqueWordsCount = 0;
    int readingWord = 0; // Флаг, що показує, чи зараз читається слово

    char c;
    while ((c = fgetc(inputFile)) != EOF) {
        if (isalpha(c)) {
            if (!readingWord) {
                readingWord = 1;
                memset(word, 0, sizeof(word)); // Очистити слово
            }
            strncat(word, &c, 1); // Додати символ до слова
        } else if (readingWord) {
            readingWord = 0;
            int isValid = 1; // Припускаємо, що слово валідне

            for (int i = 0; i < strlen(word); i++) {
                if (!isVowel(word[i])) {
                    isValid = 0;
                    break;
                }
            }

            if (isValid) {
                int isUnique = 1; // Припускаємо, що слово унікальне

                for (int i = 0; i < uniqueWordsCount; i++) {
                    if (strcmp(uniqueWords[i], word) == 0) {
                        isUnique = 0;
                        break;
                    }
                }

                if (isUnique) {
                    strcpy(uniqueWords[uniqueWordsCount], word);
                    uniqueWordsCount++;
                }
            }
        }
    }

    fclose(inputFile);

    // Виведення унікальних слів, що задовольняють умову
    for (int i = 0; i < uniqueWordsCount; i++) {
        printf("%s\n", uniqueWords[i]);
    }

    return 0;
}


