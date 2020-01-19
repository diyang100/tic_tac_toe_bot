#include <iostream>
#include <vector>
using namespace std;

bool check_won(vector<vector<char> > graph){
    for (int i = 0; i < 3; i++){
        if (graph[i][0] == graph[i][1] && graph[i][0] == graph[i][2] && graph[i][0] != '-'){
            return true;
        } else if (graph[0][i] == graph[1][i] && graph[0][i] == graph[2][i] && graph[0][i] != '-'){
            return true;
        }
    }
    if (graph[0][0] == graph [1][1] && graph[1][1] == graph[2][2] && graph[0][0] != '-'){
        return true;
    } else return (graph[0][2] == graph[1][1] && graph[0][2] == graph[2][0] && graph[0][2] != '-');
}

vector<int> can_win(vector<vector<char> > graph, char move){
    vector<int> win_move(2, -1);
    for (int row = 0; row < 3; row++){
        for (int col = 0; col < 3; col++){
            if (graph[row][col] != '-'){
                continue;
            } else {
                graph[row][col] = move;
                if (check_won(graph)){
                    win_move[0] = row;
                    win_move[1] = col;
                    return win_move;
                } else {
                    graph[row][col] = '-';
                }
            }
        }
    }
    return win_move;
}
int char_count(vector<vector<char> > graph, char character){
    int count = 0;
    for (auto row : graph){
        for (auto col: row){
            if (col == character){
                count++;
            }
        }
    }
    return count;
}
vector<vector<char> > random_move(vector<vector<char> > graph, char move){
    for (int row = 0; row < 3; row++){
        for (int col = 0; col < 3; col++){
            if (graph[row][col] == '-'){
                graph[row][col] = move;
                return graph;
            }
        }
    }
}
vector<vector<char> > bot_solve(vector<vector<char> > graph, char move){

    if (char_count(graph, '-') <= 6){
        vector<int> no_soln(2, -1);
        vector<int> move_pos = can_win(graph, move);
        vector<int> player_win_pos;
        if (move == 'X'){
            player_win_pos = can_win(graph, 'O');
        } else {
            player_win_pos = can_win(graph, 'X');
        }

        if (move_pos != no_soln){
            graph[move_pos[0]][move_pos[1]] = move;
            return graph;
        } else if (player_win_pos != no_soln){
            graph[player_win_pos[0]][player_win_pos[1]] = move;
            return graph;
        }
    }


    if (char_count(graph, '-') == 9){
        graph[0][0] = 'X';
    } else if (char_count(graph, '-') == 8 && graph[1][1] == '-'){
        graph[1][1] = 'O';
    } else if (char_count(graph, '-') == 8){
        graph[0][0] = 'O';
    } else if (char_count(graph, '-') == 7 && graph[1][1] == '-'){
        if (graph[0][2] == '-' && graph[0][1] != 'O'){
            graph[0][2] = 'X';
        } else {
            graph[2][0] = 'X';
        }
    } else if (char_count(graph, '-') == 7 && graph[1][1] == 'O'){
        graph[2][2] = 'X';
    } else if (char_count(graph, '-') == 6){
        if ((graph[0][0] == 'X' && graph[2][2] == 'X') || (graph[0][2] == 'X' && graph[2][0] == 'X')){
            graph[0][1] = 'O';
        } else {
            graph = random_move(graph, move);
        }
    } else if (char_count(graph, '-') == 5 && graph[1][1] == 'O') {
        graph[2][2] = 'X';
    } else if (char_count(graph, '-') == 5 && graph[0][1] == 'O' && graph[2][2] == '-') {
        graph[2][2] = 'X';
    } else if (char_count(graph, '-') == 5 && graph[0][2] == '-'){
        graph[0][2] = 'X';
    } else if (char_count(graph, '-') == 5){
        graph[2][0] = 'X';
    } else {
        graph = random_move(graph, move);
    }
    return graph;
}

bool incomplete_board(vector<vector<char> > graph){
    for (auto row : graph){
        for (auto col : row){
            if (col == '-'){
                return true;
            }
        }
    }
    return false;
}

vector<vector<char> > makeGraph(){
    vector<vector<char> > graph;
    for (int i = 0; i < 3; i++) {
        vector<char> row;
        for(int n = 0; n < 3; n++) {
            row.push_back('-');
        }
        graph.push_back(row);
    }
    return graph;
}

void print_graph(vector<vector<char> > graph){
    for (auto row : graph){
        for (auto col : row){
            cout << col << " ";
        }
        cout << endl;
    }
}

int main() {
    bool keep_playing = true;
    while(keep_playing) {
        vector<vector<char> > graph = makeGraph();

        int turn_choice = 0;
        char player_turn;
        char computer_turn;
        cout << "Will you go first or second? (input 1 or 2)" << endl;
        cin >> turn_choice;

        //can check if not 1 or 2 here
        if (turn_choice == 1) {
            player_turn = 'X';
            computer_turn = 'O';
        } else {
            player_turn = 'O';
            computer_turn = 'X';
            graph = bot_solve(graph, computer_turn);
        }
        print_graph(graph);

        vector<int> pos(2, 0);
        while (incomplete_board(graph)) {
            cout << "Enter a row number and col number (r c) or type -1 to concede:" << endl;
            cin >> pos[0];
            if (pos[0] == -1) {
                cout << "Player conceded!" << endl;
                break;
            }
            cin >> pos[1];
            // can check if row and column numbers are in range here
            if (graph[pos[0]][pos[1]] != '-') {
                cout << "That square is already taken!" << endl;
                continue;
            } else {
                graph[pos[0]][pos[1]] = player_turn;
            }
            print_graph(graph);
            if (!incomplete_board(graph)){
                break;
            }
            graph = bot_solve(graph, computer_turn);
            cout << "Computer plays: " << endl;
            print_graph(graph);
            if (check_won(graph)) {
                cout << "You lost!" << endl;
                break;
            }
        }
        if (!incomplete_board(graph)){
            cout << "Game Tied!" << endl;
        }
        cout << "Play again?" << endl;
        cin >> keep_playing;
    }
}
