#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 100

typedef struct {
    char alphabet[MAX_STATES];
    long states[MAX_STATES];
    long initialState;
    long finalStates[MAX_STATES];
    long numFinalStates;
    long transitions[MAX_STATES][MAX_STATES];
} Automaton;

void initializeAutomaton(Automaton* automaton) {
    memset(automaton->transitions, -1, sizeof(automaton->transitions));
}

void addTransition(Automaton* automaton, int fromState, char input, int toState) {
    int inputIndex;
    for (inputIndex = 0; inputIndex < strlen(automaton->alphabet); inputIndex++) {
        if (automaton->alphabet[inputIndex] == input) {
            break;
        }
    }
    automaton->transitions[fromState][inputIndex] = toState;
}

void transition(Automaton* automaton, int* currentState, char input) {
    long inputIndex;
    for (inputIndex = 0; inputIndex < strlen(automaton->alphabet); inputIndex++) {
        if (automaton->alphabet[inputIndex] == input) {
            break;
        }
    }
    *currentState = automaton->transitions[*currentState][inputIndex];
}

bool accepts(Automaton* automaton, char* word) {
    long currentState = automaton->initialState;
    for (int i = 0; i < strlen(word); i++) {
        transition(automaton, &currentState, word[i]);
    }
    for (int i = 0; i < automaton->numFinalStates; i++) {
        if (currentState == automaton->finalStates[i]) {
            return true;
        }
    }
    return false;
}

int main() {
    Automaton automaton;
    FILE* file = fopen("automaton.txt", "r");
    if (file == NULL) {
        perror("Could not open file");
        return 1;
    }

    char line[100];

    fgets(line, sizeof(line), file); // ||A||
    fgets(automaton.alphabet, sizeof(automaton.alphabet), file);
    strtok(automaton.alphabet, "\n"); // видаляємо символ нового рядка

    fgets(line, sizeof(line), file); // ||S||
    fgets(line, sizeof(line), file);
    int numStates = strtol(line, NULL, 10);

    for (int i = 0; i < numStates; i++) {
        fgets(line, sizeof(line), file);
        automaton.states[i] = strtol(line, NULL, 10);
    }

    fgets(line, sizeof(line), file); // Initial state
    fgets(line, sizeof(line), file);
    automaton.initialState = strtol(line, NULL, 10);

    fgets(line, sizeof(line), file); // ||F||
    fgets(line, sizeof(line), file);
    automaton.numFinalStates = strtol(line, NULL, 10);

    for (int i = 0; i < automaton.numFinalStates; i++) {
        fgets(line, sizeof(line), file);
        automaton.finalStates[i] = strtol(line, NULL, 10);
    }

    int fromState, toState;
    char input;
    while (fgets(line, sizeof(line), file) != NULL) {
        char* token = strtok(line, " ");
        fromState = strtol(token, NULL, 10);

        token = strtok(NULL, " ");
        input = token[0];

        token = strtok(NULL, " ");
        toState = strtol(token, NULL, 10);

        addTransition(&automaton, fromState, input, toState);
    }

    fclose(file);

    if (accepts(&automaton, "abc")) {
        printf("Automaton accepts the word 'abc'\n");
    } else {
        printf("Automaton does not accept the word 'abc'\n");
    }

    return 0;
}

