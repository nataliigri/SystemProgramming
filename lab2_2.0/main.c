#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_ALPHABET 100

typedef struct {
    char symbol;
    int nextState;
} Transition;

typedef struct {
    int stateID;
    Transition transitions[MAX_ALPHABET];
    int numTransitions;
} State;

typedef struct {
    int initialState;
    int acceptingStates[MAX_STATES];
    int numAcceptingStates;
    State states[MAX_STATES];
    int numStates;
} NFA;

void addTransition(NFA* nfa, int fromState, char symbol, int toState) {
    State* state = &nfa->states[fromState];
    Transition* transition = &state->transitions[state->numTransitions];
    transition->symbol = symbol;
    transition->nextState = toState;
    state->numTransitions++;
}

bool accept(NFA* nfa, const char* input) {
    int currentStates[MAX_STATES];
    int nextStates[MAX_STATES];
    int numCurrentStates = 1;
    currentStates[0] = nfa->initialState;

    for (int i = 0; input[i] != '\0'; i++) {
        char symbol = input[i];
        int numNextStates = 0;

        for (int j = 0; j < numCurrentStates; j++) {
            int currentState = currentStates[j];
            State* state = &nfa->states[currentState];

            for (int k = 0; k < state->numTransitions; k++) {
                Transition* transition = &state->transitions[k];

                if (transition->symbol == symbol || transition->symbol == 'e') {
                    nextStates[numNextStates++] = transition->nextState;
                }
            }
        }

        if (numNextStates == 0) {
            return false;
        }

        numCurrentStates = numNextStates;
    }

    for (int i = 0; i < numCurrentStates; i++) {
        int currentState = currentStates[i];

        for (int j = 0; j < nfa->numAcceptingStates; j++) {
            if (currentState == nfa->acceptingStates[j]) {
                return true;
            }
        }
    }

    return false;
}

int main() {
    NFA nfa = {0, {0, 1, 2, 3}, 4, 0, {{0, {{0, 0}, {0, 0}, {0, 0}}, 0},
                                       {1, {{0, 0}, {0, 0}, {0, 0}}, 0},
                                       {2, {{0, 0}, {0, 0}, {0, 0}}, 0},
                                       {3, {{0, 0}, {0, 0}, {0, 0}}, 0}}};

    addTransition(&nfa, 0, 'a', 1);
    addTransition(&nfa, 0, 'b', 2);
    addTransition(&nfa, 0, 'c', 3);

    addTransition(&nfa, 1, 'a', 0);
    addTransition(&nfa, 1, 'b', 2);
    addTransition(&nfa, 1, 'c', 3);

    addTransition(&nfa, 2, 'a', 1);
    addTransition(&nfa, 2, 'b', 0);
    addTransition(&nfa, 2, 'c', 3);

    addTransition(&nfa, 3, 'a', 1);
    addTransition(&nfa, 3, 'b', 2);
    addTransition(&nfa, 3, 'c', 3);

    FILE* file1 = fopen("test1.txt", "r");
    if (!file1) {
        printf("Could not open file 'test1.txt'\n");
        return 1;
    }

    FILE* file2 = fopen("test2.txt", "r");
    if (!file2) {
        printf("Could not open file 'test2.txt'\n");
        fclose(file1);
        return 1;
    }

    char line[100];

    while (fgets(line, sizeof(line), file1)) {
        line[strcspn(line, "\n")] = 0; // Видаляє символ нового рядка, який fgets може прочитати

        bool result = accept(&nfa, line);
        printf("%s: %s\n", line, result ? "Accepted" : "Rejected");
    }

    while (fgets(line, sizeof(line), file2)) {
        line[strcspn(line, "\n")] = 0; // Видаляє символ нового рядка, який fgets може прочитати

        bool result = accept(&nfa, line);
        printf("%s: %s\n", line, result ? "Accepted" : "Rejected");
    }

    fclose(file1);
    fclose(file2);

    return 0;
}


