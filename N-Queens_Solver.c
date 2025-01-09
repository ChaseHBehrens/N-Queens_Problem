#include <stdio.h>
#include <stdint.h>

static inline void FLIP_BIT (int* array, int n) {
    array[n / 32] ^= (1ULL << (n % 32));
}
static inline int CHECK_BIT (int* array, int n) {
    return array[n / 32] & (1ULL << (n % 32));
}

/**
 * @brief holds board data
 */
typedef struct {
    int number_of_queens; /**the number of queens on the board*/
    int locations[100]; /**the location of queens index is rank int is file from 1 to n*/
    int occupied_rank[4]; /**the current occupied ranks*/
    int occupied_file[4]; /**the current occupied file*/
    int occupied_diag1[8]; /**the current occupied diag1*/
    int occupied_diag2[8]; /**the current occupied diag2*/
} Board;

/**
 * @brief displays a board position
 * @param board the board being displayed
 * @param n the size of the nxn board
 */
int display_board (Board* board, int n) {
    for (int rank = 0; rank < n; rank ++) {
        if (board->locations[rank] == 0) {
            for (int i = 0; i < n; i ++) {printf(" .");}
            printf("\n");
            continue;
        }
        for (int file = 1; file <= n; file ++) {
            if (board->locations[rank] == file) {printf(" Q");}
            else {printf(" .");}
        }
        printf("\n");
    }
    return 0;
}

/**
 * @brief adds a queen to a board
 * @param board the board being updated
 * @param square the square to add the queen to
 * @param n the size of the nxn board
 * @return 1 if sucessfuly added otherwise 0
 */
int add_queen (Board* board, int square, int n) {
    int rank = square / n;
    if (CHECK_BIT(board->occupied_rank, rank)) {return 0;}
    int file = square % n;
    if (CHECK_BIT(board->occupied_file, file)) {return 0;}
    int diag1 = rank + file;
    if (CHECK_BIT(board->occupied_diag1, diag1)) {return 0;}
    int diag2 = rank + (n - 1 - file);
    if (CHECK_BIT(board->occupied_diag2, diag2)) {return 0;}
    board->locations[rank] = file + 1;
    board->number_of_queens ++;
    FLIP_BIT(board->occupied_rank, rank);
    FLIP_BIT(board->occupied_file, file);
    FLIP_BIT(board->occupied_diag1, diag1);
    FLIP_BIT(board->occupied_diag2, diag2);
    return 1;
}

/**
 * @brief removes the queen on the highest rank from a legal board
 * @param board the board being updated
 * @param n the size of the nxn board
 */
int remove_last_queen (Board* board, int n) {
    board->number_of_queens --;
    int rank = board->number_of_queens;
    int file = board->locations[rank] - 1;
    int diag1 = rank + file;
    int diag2 = rank + (n - 1 - file);
    FLIP_BIT(board->occupied_rank, rank);
    FLIP_BIT(board->occupied_file, file);
    FLIP_BIT(board->occupied_diag1, diag1);
    FLIP_BIT(board->occupied_diag2, diag2);
    board->locations[rank] = 0;
    return 0;
}

/**
 * @brief calculates if a board position has no queens attacking each other
 * @param board the board being analized
 * @param n the size of the nxn board
 * @return 1 if the position is legal otherwise 0
 */
int is_legal_position (int locations[], int n) {
    Board board = {0};
    for (int i = 0; i < n; i ++) {
        if (!add_queen(&board, (i * n) + (locations[i] - 1), n)) {return 0;}
    }
    return 1;
}

/**
 * @brief calculates the next legal board position
 * @param board the board being analized
 * @param n the size of the nxn board
 * @return the next legal board or the board given if there is no next legal board
 */
Board next_legal_position (Board board, int n) {
    if (!is_legal_position (board.locations, n)) {return board;}
    for (int rank = 0; rank < n; rank ++) {
        for (int file = 0; file < n; file ++) {
            if (add_queen(&board, (rank * n) + file, n)) {return board;}
        }
    }
    return board;
}

/**
 * @brief finds the first possible solution to the n-queens problem
 * @param board the board being analized
 * @param n the size of the nxn board
 * @return the first board solution found
 */
Board find_first_solution (int n) {
    Board board = {0};
    int last_queen_file = 0;
    while (1) {
        start:
        for (int file = last_queen_file; file < n; file ++) {
            if (add_queen(&board, (board.number_of_queens * n) + file, n)) {
                if (board.number_of_queens == n) {return board;}
                last_queen_file = 0;
                goto start;
            }
        }
        last_queen_file = board.locations[board.number_of_queens - 1];
        remove_last_queen(&board, n);
        if (board.locations[0] == 0 && last_queen_file == n) {return board;}
    }
}

/**
 * @brief finds the first possible solution to the n-queens problem
 * @param board the board being analized
 * @param n the size of the nxn board
 * @return the total number of possible solutions
 */
int count_solutions (int n) {
    Board board = {0};
    int soultion_count = 0;
    int last_queen_file = 0;
    while (1) {
        start:
        for (int file = last_queen_file; file < n; file ++) {
            if (add_queen(&board, (board.number_of_queens * n) + file, n)) {
                if (board.number_of_queens == n) {soultion_count ++;}
                last_queen_file = 0;
                goto start;
            }
        }
        last_queen_file = board.locations[board.number_of_queens - 1];
        remove_last_queen(&board, n);
        if (board.locations[0] == 0 && last_queen_file == n) {return soultion_count;}
    }
}

/**
 * @brief gets an iniger choice input from the user
 * @param prompt the question asked to the user
 * @param minimum the minimum allowed intiger input
 * @param maximum the maximum allowed intiger input
 * @return the intiger value entered by the user or -1 to quit
 */
int get_intiger_input (char* prompt, int minimum, int maximum) {
    printf("%s\nn = ", prompt);
    int n;
    char input[100];
    while (1) {
        if (fgets (input, sizeof(input), stdin)) {
            if (input[0] == '\n') {return -1;}
            if (sscanf (input, "%d", &n) != 1) {
                printf ("Error, enter an integer.\nn = ");
            } else if (n < minimum || n > maximum) {
                printf ("Error, enter an intiger within the range %i to %i.\nn = ", minimum, maximum);
            } else {
                return n;
            }
        }
    }
}

/**
 * @brief continuosly prompts the user for an n value and then calculates and displays 
 * the fist solution and the total number of solutions for that n value.
 */
int main () {
    while (1) {
        int n = get_intiger_input("Enter a value of n or press enter to quit.", 4, 100);
        if (n == -1) {break;}
        Board board = find_first_solution(n);
        printf ("The First solution found for %i-Queens Problem is\n", n);
        display_board(&board, n);
        if (n > 20) {printf("Number of solutions unavailable for n > 20.\n"); continue;}
        printf ("There are %i solutions to the %i-Queens Problem.\n", count_solutions(n), n);
    }
    return 0;
}