#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <string>

std::map<std::string, std::string> m;
std::ofstream fout("tictactoe.cfg");

char WinningPlayer(std::string board) {
    if (board[0] == board[4] && board[4] == board[8] && board[4] != '-') return board[4];
    if (board[2] == board[4] && board[4] == board[6] && board[4] != '-') return board[4];
    for (int i = 0; i < 3; i++) {
        if (board[i] == board[3 + i] && board[i] == board[6 + i] && board[i] != '-') return board[i];
        if (board[3 * i] == board[3 * i + 1] && board[3 * i] == board[3 * i + 2] && board[3 * i] != '-') return board[3 * i];
    }
    for (char c : board) if (c == '-') return '-';
    return 'T';
}

int Negamax(std::string board, int player, int depth) {
    switch (WinningPlayer(board))
    {
    case 'T':
        m[board] = "Game tied!";
        return 0;
    case 'X': 
        m[board] = "X wins!";
        return player * 1000 / depth;
    case 'O':
        m[board] = "O wins!"; 
        return player * -1000 / depth;
    default:
        int best = -1000;
        std::string bestMove;
        for (int i = 0; i < 9; i++) {
            if (board[i] != '-') continue;
            board[i] = player > 0 ? 'X' : 'O';
            int value = -Negamax(board, -player, depth + 1);
            if (value > best) {
                best = value;
                bestMove = board;
            }
            board[i] = '-';
        }
        m[board] = bestMove;
        return best;
    }
}

char CurrentPlayer(std::string board) {
    int x = 0, o = 0;
    for (char c: board) {
        switch (c)
        {
        case 'X':
            x++;
            break;
        case 'O':
            o++;
            break;
        }
    }
    return "XO"[x > o];
}

void PrintBoard(std::string board) {
    fout << "echo;";
    fout << "echo " << board[0] << " | " << board[1] << " | " << board[2] << ";row;";
    fout << "echo " << board[3] << " | " << board[4] << " | " << board[5] << ";row;";
    fout << "echo " << board[6] << " | " << board[7] << " | " << board[8] << ';';
}

int main() {
    Negamax("---------", 1, 0);
    fout << "alias row echo --+---+--\n";
    fout << "alias reset \"alias 1;alias 2;alias 3;alias 4;alias 5;alias 6;alias 7;alias 8;alias 9\"\n";
    for (auto i : m) {
        fout << "alias " << i.first << " \"clear;";
        if (i.second.length() == 9) {
            std::string board = i.second;
            if (m[board].length() == 9) {
                std::string command[9];
                for (int j = 0; j < 9; j++) {
                    if (board[j] != '-') continue;
                    board[j] = CurrentPlayer(board);
                    command[j] = ' ' + board;
                    board[j] = '-';
                }
                for (int j = 0; j < 9; j++) fout << "alias " << j + 1 << command[j] << ';';
                PrintBoard(board);
                fout << "echo;echo select move;";
                for (int j = 0; j < 9; j++) {
                    if (j % 3 == 0) fout << "echo ";
                    if (command[j].length() > 0) fout << j + 1;
                    else fout << '*';
                    fout << ' ';
                    if (j % 3 == 2 && j != 8) fout << ';';
                }
            }
            else {
                PrintBoard(board);
                fout << "echo;echo " << m[board] << ";reset";
            }
        }
        else {
            PrintBoard(i.first);
            fout << "echo;echo " << i.second << ";reset";
        }
        fout << "\"\n";
    }
    fout << "clear\necho Do you want to go first? (y/n)\n";
    fout << "alias n \"alias y;alias n;---------\"\n";
    fout << "alias y \"alias y;alias n;clear;echo - | - | -;row;echo - | - | -;row;echo - | - | -;echo;";
    fout << "echo select move;echo 1 2 3;echo 4 5 6; echo 7 8 9;";
    fout << "alias 1 X--------;alias 2 -X-------;alias 3 --X------;";
    fout << "alias 4 ---X-----;alias 5 ----X----;alias 6 -----X---;";
    fout << "alias 7 ------X--;alias 8 -------X-;alias 9 --------X\"";
}