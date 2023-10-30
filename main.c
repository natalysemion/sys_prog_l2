#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_STATES 100
#define MAX_ALPHABET_SIZE 26

struct Transition {
    int from_state;
    char input_symbol;
    int to_state;
};

struct Automaton {
    int alphabet_size;
    char alphabet[MAX_ALPHABET_SIZE];
    int num_states;
    int states[MAX_STATES];
    int initial_state;
    int num_final_states;
    int final_states[MAX_STATES];
    struct Transition transitions[MAX_STATES * MAX_ALPHABET_SIZE];
    int num_transitions;
};

void findStatesForSymbol(struct Automaton *automaton, char symbol, int *result_states, int *result_count) {
    int i;
    *result_count = 0;

    for (i = 0; i < automaton->num_transitions; i++) {
        if (automaton->transitions[i].input_symbol == symbol) {
            result_states[(*result_count)++] = automaton->transitions[i].from_state;
        }
    }
}


bool is_final_state(const struct Automaton automaton, int state) {
    for (int i = 0; i < automaton.num_final_states; i++) {
        if (automaton.final_states[i] == state) {
            return true;
        }
    }
    return false;
}

int get_transition_state(const struct Automaton automaton, int current_state, char input_symbol) {
    for (int i = 0; i < automaton.num_transitions; i++) {
        if (automaton.transitions[i].from_state == current_state &&
            automaton.transitions[i].input_symbol == input_symbol) {
            return automaton.transitions[i].to_state;
        }
    }
    return -1;
}


bool dfs(const struct Automaton automaton, int current_state, int to_state, char input_symbol, bool visited[]) {
    if (current_state == to_state) {
        return true; // Path found
    }

    visited[current_state] = true;

    for (int i = 0; i < automaton.num_transitions; i++) {
        if (!visited[automaton.transitions[i].to_state] &&
            automaton.transitions[i].from_state == current_state &&
            automaton.transitions[i].input_symbol == input_symbol) {
            if (dfs(automaton, automaton.transitions[i].to_state, to_state, input_symbol, visited)) {
                return true; // Path found
            }
        }
    }

    return false; // No path found
}

int can_transition(const struct Automaton automaton, int from_state, int to_state, char input_symbol) {
    bool *visited = (bool*)malloc(automaton.num_states * sizeof(bool));
    if (visited == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < automaton.num_states; i++) {
        visited[i] = false;
    }

    return dfs(automaton, from_state, to_state, input_symbol, visited);
}

//з початкового до target
bool canReachStateFromStart(struct Automaton *automaton, int target_state) {
    bool visited[MAX_STATES] = {0}; // Масив, що вказує, чи був відвіданий стан
    int queue[MAX_STATES];
    int front = 0, rear = 0;

    // Додати початковий стан до черги
    queue[rear++] = automaton->initial_state;
    visited[automaton->initial_state] = true;

    while (front < rear) {
        int current_state = queue[front++];

        // Перевірити, чи цей стан є цільовим
        if (current_state == target_state) {
            return true;
        }

        // Перейти до всіх можливих наступних станів
        for (int i = 0; i < automaton->num_transitions; i++) {
            if (automaton->transitions[i].from_state == current_state) {
                int next_state = automaton->transitions[i].to_state;
                if (!visited[next_state]) {
                    queue[rear++] = next_state;
                    visited[next_state] = true;
                }
            }
        }
    }

    // Якщо не вдалося знайти шлях до цільового стану
    return false;
}


bool canReachFinalState(struct Automaton *automaton, int current) {
    // Масив для відстеження відвіданих станів
    bool visited[MAX_STATES] = {false};

    // Черга для обробки станів
    int queue[MAX_STATES];
    int front = 0, rear = 0;

    // Початковий стан відзначається як відвіданий і додається в чергу
    visited[current] = true;
    queue[rear++] = current;

    while (front < rear) {
        int current_state = queue[front++];

        // Перевірка, чи поточний стан є фінальним
        for (int i = 0; i < automaton->num_final_states; i++) {
            if (current_state == automaton->final_states[i]) {
                return true;
            }
        }

        // Перегляд усіх переходів з поточного стану
        for (int i = 0; i < automaton->num_transitions; i++) {
            struct Transition transition = automaton->transitions[i];
            if (transition.from_state == current_state && !visited[transition.to_state]) {
                visited[transition.to_state] = true;
                queue[rear++] = transition.to_state;
            }
        }
    }

    // Неможливо досягнути фінальний стан з поточного стану
    return false;
}


bool is_word_accepted(const struct Automaton automaton, const char* word) {
    int current_state = automaton.initial_state;
    for (int i = 0; word[i] != '\0'; i++) {
        current_state = get_transition_state(automaton, current_state, word[i]);
        printf("%d ", current_state);
        if (current_state == -1) {
            return false; // Word is not accepted
        }
    }
    return is_final_state(automaton, current_state);
}
bool is_word_accepted2(const struct Automaton automaton, const char* word, int* state) {
    int current_state = *state;
    //printf("\n is_word_accepted2 current_state: %d", current_state);
    for (int i = 0; word[i] != '\0'; i++) {
        current_state = get_transition_state(automaton, current_state, word[i]);
       // printf("\n is_word_accepted2 current_state: %d", current_state);
        if (current_state == -1) {
            return false; // Word is not accepted
        }
    }

    *state = current_state;
    return true;
}

void displayAutomaton(const struct Automaton automaton) {
    printf("Alphabet size: %d\n", automaton.alphabet_size);
    printf("Alphabet:\n");
    for(int i = 0; i < automaton.alphabet_size; i++){
         printf("%c ", automaton.alphabet[i]);
    }
    printf("\n");
    printf("Number of states: %d\n", automaton.num_states);
    printf("States:\n");
    for(int i = 0; i < automaton.num_states; i++){
        printf("%d ", automaton.states[i]);
    }
    printf("\n");
    printf("Initial state: %d\n", automaton.initial_state);
    printf("Number of final states: %d\n", automaton.num_final_states);

    printf("Final states: ");
    for (int i = 0; i < automaton.num_final_states; i++) {
        printf("%d ", automaton.final_states[i]);
    }
    printf("\n");

    printf("Transitions:\n");
    for (int i = 0; i < automaton.num_transitions; i++) {
        printf("%d --(%c)--> %d\n", automaton.transitions[i].from_state, automaton.transitions[i].input_symbol, automaton.transitions[i].to_state);
    }
}

int main() {
    char filename[] = "input3.txt";

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    struct Automaton automaton;
    automaton.num_transitions = 0;

    fscanf(file, "%d %d %d %d", &automaton.alphabet_size, &automaton.num_states, &automaton.initial_state, &automaton.num_final_states);

    for (int i = 0; i < automaton.num_final_states; i++) {
        fscanf(file, "%d", &automaton.final_states[i]);
    }

    while (fscanf(file, "%d %c %d", &automaton.transitions[automaton.num_transitions].from_state,
                  &automaton.transitions[automaton.num_transitions].input_symbol,
                  &automaton.transitions[automaton.num_transitions].to_state) == 3) {
        automaton.num_transitions++;
    }

    fclose(file);

    for(int i = 0; i < automaton.num_states; i++){
        automaton.states[i] = i;
    }

    for(int i = 0; i < automaton.alphabet_size; i++){
        automaton.alphabet[i]= 'a' + i;
    }

    displayAutomaton(automaton);

    char w0[100];
    printf("Enter w0: ");
    scanf("%s", w0);


    char symbol_to_check = w0[0];
    int result_states[MAX_STATES];
    int result_count;

    findStatesForSymbol(&automaton, symbol_to_check, result_states, &result_count);

    printf("States that read a character '%c':\n", symbol_to_check);
    for (int i = 0; i < result_count; i++) {
        printf("%d\n", result_states[i]);
    }

    printf("\n");
    bool check = false;
    for(int i = 0; i < result_count; i++){
        int current_state = result_states[i];
        printf("\ncurrent_state %d", result_states[i]);
        bool w0_accepted = is_word_accepted2(automaton, w0, &current_state);
        if(!w0_accepted){
            printf("\n is_word_accepted FALSE");
            continue;
        }
        printf("\n is_word_accepted TRUE");
        if(!canReachStateFromStart(&automaton, result_states[i])){
            printf("\n canReachStateFromStart FALSE");
            continue;
        }
        printf("\n canReachStateFromStart TRUE");
        if(canReachFinalState(&automaton, current_state)){
            check = true;
            printf("\n canReachFinalState TRUE");
            break;
        }
        printf("\n canReachFinalState FALSE");
    }
    if(check){
        printf("\n\nw1w0w2 is accepted\n");
    }
    else{
        printf("\n\nw1w0w2 isn't accepted\n");
    }

    return 0;
}
