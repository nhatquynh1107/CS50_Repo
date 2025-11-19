/*Ref:
1. https://youtu.be/qigPctCgR_k
2. https://theory.stanford.edu/~amitp/GameProgramming/AStarComparison.html#the-a-star-algorithm
*/

#include "frontend.hpp"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <chrono>
#include <windows.h>

using namespace std;

mt19937 rd(chrono::steady_clock::now().time_since_epoch().count());

int Rand(int l, int h){
    return uniform_int_distribution<int>(l, h)(rd);
}

int load_progress() {
    ifstream fin("progress.txt");
    int level = 0;
    if (fin.is_open()) fin >> level;
    return level;
}

void save_progress(int& level) {
    ofstream fout("progress.txt");
    if (fout.is_open()) fout << level;
}

void game_over(int state){
    if(state == 0){
        cout << "Game over!\nYou got caught";
    }
    else{
        cout << "Game over!\nYou got trapped";
    }
}

void game_win(){
    cout << "Congratulations! You have escaped the maze";
}

int manhattan_dist(int& player_r, int& player_c, int& mummy_r, int& mummy_c){
    return abs(mummy_r - player_r) + abs(mummy_c - player_c);
}

void player_move(const vector<vector<int>>& wall, char& input, int& player_r, int& player_c, int& nxt_player_r, int& nxt_player_c){
    if(input == 72 || input == 'w' || input == 'W'){
        if(!(wall[player_r][player_c] & 4)) nxt_player_r -= 1;
        else nxt_player_r = -1;
    }
    else if(input == 80 || input == 's' || input == 'S'){
        if(!(wall[player_r][player_c] & 8)) nxt_player_r += 1;
        else nxt_player_r = -1;
    }
    else if(input == 75 || input == 'a' || input == 'A'){
        if(!(wall[player_r][player_c] & 1)) nxt_player_c -= 1;
        else nxt_player_c = -1;
    }
    else if(input == 77 || input == 'd' || input == 'D'){
        if(!(wall[player_r][player_c] & 2)) nxt_player_c += 1;
        else nxt_player_c = -1;
    }
}

void visualise_game(sf::RenderWindow& window, const vector<vector<int>>& wall, int& player_r, int& player_c,
                    int mummy_r, int mummy_c, int mummy2_r, int mummy2_c, int scorpion_r, int scorpion_c, 
                    int trap_r, int trap_c,int win_r, int win_c){
    window.clear();
    draw_maze(window, wall);
    draw_entity(window, win_r, win_c, sf::Color::Green);
    draw_entity(window, player_r, player_c, sf::Color::Blue);
    if(scorpion_r != -1) draw_entity(window, scorpion_r, scorpion_c, sf::Color::Yellow);
    if(mummy_r != -1) draw_entity(window, mummy_r, mummy_c, sf::Color::Red);
    if(mummy2_r != -1) draw_entity(window, mummy2_r, mummy2_c, sf::Color::Red);
    if(trap_r != -1) draw_trap(window, trap_r, trap_c, sf::Color::Magenta);
    window.display();
}

int level_0(sf::RenderWindow& window){
    // ref: https://postimg.cc/w3fkDvww
    vector<string> maze = {
        "......",
        "....P.",
        "......",
        "......",
        "......",
        ".M....",
    };

    vector<vector<int>> wall = {
        {13, 4, 14, 5, 12, 6},
        {5, 8, 14, 11, 5, 10},
        {3, 5, 14, 5, 2, 7},
        {9, 0, 4, 2, 11, 3},
        {5, 8, 0, 0, 6, 3},
        {9, 12, 8, 8, 8, 10}
    };

    int n_row = maze.size();
    int n_col = maze[0].size();
    int player_r = -1, player_c = -1;
    int mummy_r = -1, mummy_c = -1;
    int win_r = 5, win_c = 0;

    // scan map
    for(int r = 0; r < n_row; r++){
        for(int c = 0; c < n_col; c++){
            if(maze[r][c] == 'P'){
                player_r = r; player_c = c;
            }
            else if(maze[r][c] == 'M'){
                mummy_r = r; mummy_c = c;
            }
        }
    }

    bool caught = false, win = false;
    bool exist = (mummy_r != -1);

    vector<pair<int,int>> player, mummy;
    player.push_back({player_r, player_c});
    mummy.push_back({mummy_r, mummy_c});

    // create SFML window ONCE
    int mummy2_r = -1, mummy2_c = -1;
    int scorpion_r = -1, scorpion_c = -1;
    int trap_r = -1, trap_c = -1;
    
    while(window.isOpen()){
        
        visualise_game(window, wall, player_r, player_c, mummy_r, mummy_c, -1, -1, -1, -1, -1, -1, win_r, win_c);

        draw_hud(window, player.size() - 1, (mummy_r != -1) + (mummy2_r != -1), (scorpion_r != -1));

        if(caught){
            game_over(0);
            return 0;
        }
        if(win){
            game_win();
            return 1;
        }

        char input = '0';

        // SFML event & keyboard handling
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
                return -1;
            }

            if(event.type == sf::Event::KeyPressed){
                switch(event.key.code){
                    case sf::Keyboard::W: input = 'w'; break;
                    case sf::Keyboard::A: input = 'a'; break;
                    case sf::Keyboard::S: input = 's'; break;
                    case sf::Keyboard::D: input = 'd'; break;
                    case sf::Keyboard::Space: input = ' '; break;
                    case sf::Keyboard::R: input = 'r'; break;
                    case sf::Keyboard::M: input = 'm'; break;
                    case sf::Keyboard::Q: input = 'q'; break;
                }
            }
        }

        if(input == '0') continue; // no key pressed, skip loop

        // Quit
        if(input == 'q' || input == 'Q'){
            window.close();
            return -1;
        }

        // redo
        if(input == 'r' || input == 'R'){
            if(player.size() == 1) continue;
            // go back
            player.pop_back();
            mummy.pop_back();
            player_r = player.back().first;
            player_c = player.back().second;
            mummy_r  = mummy.back().first;
            mummy_c  = mummy.back().second;

            continue;
        }

        // reset
        if(input == 'm' || input == 'M'){

            // return to first stored location
            player_r = player.front().first;
            player_c = player.front().second;
            mummy_r  = mummy.front().first;
            mummy_c  = mummy.front().second;
            player.clear();
            mummy.clear();

            // re-create history
            player.push_back({player_r, player_c});
            mummy.push_back({mummy_r, mummy_c});
            continue;
        }

        // Player movement
        int nxt_r = player_r, nxt_c = player_c;
        player_move(wall, input, player_r, player_c, nxt_r, nxt_c);
        caught = false;

        if(nxt_r != -1 && nxt_c != -1){
            // collide with mummy
            if(nxt_r == mummy_r && nxt_c == mummy_c){
                caught = true;;
                break;
            }

            // reach exit
            if(nxt_r == win_r && nxt_c == win_c){
                win = true;
            }

            maze[player_r][player_c] = '.';
            maze[nxt_r][nxt_c] = 'P';
            player_r = nxt_r; player_c = nxt_c;
            player.push_back({player_r, player_c});

            // mummy move (2 steps random)
            int dr[4] = {0, 0, -1, 1};
            int dc[4] = {-1, 1, 0, 0};
            for(int step = 0; step < 2; step++){
                vector<int> valid;
                for(int d = 0; d < 4; d++){
                    if(!(wall[mummy_r][mummy_c] & (1<<d)))
                        valid.push_back(d);
                }
                if(valid.empty()) break;
                int d = valid[Rand(0, valid.size()-1)];
                mummy_r += dr[d];
                mummy_c += dc[d];
                if(mummy_r == player_r && mummy_c == player_c){
                    caught = true;
                    break;
                }
                visualise_game(window, wall, player_r, player_c, mummy_r, mummy_c, -1, -1, -1, -1, -1, -1, win_r, win_c);

                sf::sleep(sf::milliseconds(120));
            }
            mummy.push_back({mummy_r,mummy_c});
        }
    }
    return 1;
}

void white_mummy_move(const vector<vector<int>>& wall, int& mummy_r, int& mummy_c,
                      bool& caught, int& player_r, int& player_c,
                      int& mummy2_r, int& mummy2_c,
                      int& scorpion_r, int& scorpion_c, int& trap_r, int& trap_c,
                      sf::RenderWindow& window, int win_r, int win_c){

    int white_dr[4] = {0, 0, -1, 1};
    int white_dc[4] = {-1, 1, 0, 0};

    for(int step = 1; step <= 2 && !caught && mummy_r != -1; ++step){
        int nx_dir = -1;
        int cur_dist = manhattan_dist(player_r, player_c, mummy_r, mummy_c);

        // Luon uu tien di chuyen ngang
        for(int direction = 0; direction < 4; ++direction){
            int nxt_mummy_r = mummy_r + white_dr[direction];
            int nxt_mummy_c = mummy_c + white_dc[direction];
            if(!(wall[mummy_r][mummy_c] & (1 << direction))){
                int nxt_dist = manhattan_dist(player_r, player_c, nxt_mummy_r, nxt_mummy_c);
                if(nxt_dist < cur_dist){
                    cur_dist = nxt_dist;
                    nx_dir = direction;
                }
            }
        }

        // Neu co huong di (khong bi chan boi tuong)
        if(nx_dir != -1){
            mummy_r += white_dr[nx_dir];
            mummy_c += white_dc[nx_dir];

            if(mummy_r == player_r && mummy_c == player_c){
                caught = true;
            }

            // Mummy di chuyển trúng mummy2 -> mummy bị tiêu diệt
            if(mummy_r == mummy2_r && mummy_c == mummy2_c){
                mummy_r = -1;
            }
            // Mummy di chuyển trúng scorpion -> scorpion bị tiêu diệt
            else if(mummy_r == scorpion_r && mummy_c == scorpion_c){
                scorpion_r = -1;
            }
            visualise_game(window, wall, player_r, player_c, mummy_r, mummy_c, mummy2_r, mummy2_c, scorpion_r, scorpion_c, trap_r, trap_c, win_r, win_c);

            sf::sleep(sf::milliseconds(120));
        }
    }
}

void white_scorpion_move(const vector<vector<int>>& wall, int& scorpion_r, int& scorpion_c,
                        bool& caught, int& player_r, int& player_c,
                        int& mummy_r, int& mummy_c,
                        int& mummy2_r, int& mummy2_c, int& trap_r, int& trap_c,
                        sf::RenderWindow& window, int win_r, int win_c){
    int white_dr[4] = {0, 0, -1, 1};
    int white_dc[4] = {-1, 1, 0, 0};

    int nx_dir = -1;
    int cur_dist = manhattan_dist(player_r, player_c, scorpion_r, scorpion_c);

    // Luon uu tien di chuyen ngang
    for(int direction = 0; direction < 4; ++direction){
        int nxt_scorpion_r = scorpion_r + white_dr[direction];
        int nxt_scorpion_c = scorpion_c + white_dc[direction];
        if(!(wall[scorpion_r][scorpion_c] & (1 << direction))){
            int nxt_dist = manhattan_dist(player_r, player_c, nxt_scorpion_r, nxt_scorpion_c);
            if(nxt_dist < cur_dist){
                cur_dist = nxt_dist;
                nx_dir = direction;
            }
        }
    }

    // Neu co huong di (khong bi chan boi tuong)
    if(nx_dir != -1){
        scorpion_r += white_dr[nx_dir];
        scorpion_c += white_dc[nx_dir];

        if(scorpion_r == player_r && scorpion_c == player_c){
            caught = true;
        }

        // Mummy di chuyển trúng mummy2 -> mummy bị tiêu diệt
        if(scorpion_r == mummy_r && scorpion_c == mummy_c){
            scorpion_r = -1;
        }
        // Mummy di chuyển trúng scorpion -> scorpion bị tiêu diệt
        else if(scorpion_r == mummy2_r && scorpion_c == mummy2_c){
            scorpion_r = -1;
        }
        visualise_game(window, wall, player_r, player_c, mummy_r, mummy_c, mummy2_r, mummy2_c, scorpion_r, scorpion_c, trap_r, trap_c, win_r, win_c);

        sf::sleep(sf::milliseconds(120));
    }            
}

int run_level(vector<string>& maze, vector<vector<int>>& wall, sf::RenderWindow& window, int n_row, int n_col, int win_r, int win_c){
    int player_r = -1, player_c = -1;
    int mummy_r = -1, mummy_c = -1;
    int mummy2_r = -1, mummy2_c = -1;
    int trap_r = -1, trap_c = -1;
    int scorpion_r = -1, scorpion_c = -1;

    // Scan maze for initial positions
    for(int row = 0; row < n_row; ++row){
        for(int col = 0; col < n_col; ++col){
            if(maze[row][col] == 'M'){
                if(mummy_r == -1) mummy_r = row, mummy_c = col;
                else mummy2_r = row, mummy2_c = col;
            }
            else if(maze[row][col] == 'P'){
                player_r = row, player_c = col;
            }
            else if(maze[row][col] == 'T'){
                trap_r = row, trap_c = col;
            }
            else if(maze[row][col] == 'S'){
                scorpion_r = row, scorpion_c = col;
            }
        }
    }

    bool caught = false, win = false, trap = false;
    vector<pair<int, int>> player, mummy, mummy2, scorpion;
    player.push_back({player_r, player_c});
    mummy.push_back({mummy_r, mummy_c});
    mummy2.push_back({mummy2_r, mummy2_c});
    scorpion.push_back({scorpion_r, scorpion_c});
    
    while(window.isOpen()){
        visualise_game(window, wall, player_r, player_c, mummy_r, mummy_c, mummy2_r, mummy2_c, scorpion_r, scorpion_c, trap_r, trap_c, win_r, win_c);
        draw_hud(window, player.size() - 1, (mummy_r != -1) + (mummy2_r != -1), (scorpion_r != -1));

        if(trap){
            game_over(1);
            return 0;
        }
        if(caught){
            game_over(0);
            return 0;
        }
        if(win){
            game_win();
            return 1;
        }

        char input = '0';

        // SFML keyboard input
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
                return -1;
            }

            if(event.type == sf::Event::KeyPressed){
                switch(event.key.code){
                    case sf::Keyboard::W: input = 'w'; break;
                    case sf::Keyboard::A: input = 'a'; break;
                    case sf::Keyboard::S: input = 's'; break;
                    case sf::Keyboard::D: input = 'd'; break;
                    case sf::Keyboard::Space: input = ' '; break;
                    case sf::Keyboard::R: input = 'r'; break;
                    case sf::Keyboard::M: input = 'm'; break;
                    case sf::Keyboard::Q: input = 'q'; break;
                }
            }
        }

        if(input == '0') continue; // no key pressed, skip loop

        // Quit
        if(input == 'q' || input == 'Q'){
            window.close();
            return -1;
        }

        // Redo
        if(input == 'r' || input == 'R'){
            if (player.size() == 1) continue;
            player.pop_back();
            mummy.pop_back();
            mummy2.pop_back();
            scorpion.pop_back();
            player_r = player.back().first, player_c = player.back().second;
            mummy_r = mummy.back().first, mummy_c = mummy.back().second;
            mummy2_r = mummy2.back().first, mummy2_c = mummy2.back().second;
            scorpion_r = scorpion.back().first, scorpion_c = scorpion.back().second;

            continue;
        }

        // Reset
        if(input == 'm' || input == 'M'){
            player_r = player.front().first, player_c = player.front().second;
            mummy_r = mummy.front().first, mummy_c = mummy.front().second;
            mummy2_r = mummy2.front().first, mummy2_c = mummy2.front().second;
            scorpion_r = scorpion.front().first, scorpion_c = scorpion.front().second;

            player.clear(), mummy.clear(), mummy2.clear(), scorpion.clear();
            player.push_back({player_r, player_c});
            mummy.push_back({mummy_r, mummy_c});
            mummy2.push_back({mummy2_r, mummy2_c});
            scorpion.push_back({scorpion_r, scorpion_c});
            continue;
        }

        // Player move
        int nxt_player_r = player_r, nxt_player_c = player_c;
        player_move(wall, input, player_r, player_c, nxt_player_r, nxt_player_c);
        caught = false;

        if(nxt_player_r != -1 && nxt_player_c != -1){
            if(nxt_player_r == trap_r && nxt_player_c == trap_c){
                trap = true;
                continue;
            }
            if((nxt_player_r == mummy_r && nxt_player_c == mummy_c) ||
               (nxt_player_r == mummy2_r && nxt_player_c == mummy2_c) ||
               (nxt_player_r == scorpion_r && nxt_player_c == scorpion_c)){
                caught = true;
                break;
            }
            if(nxt_player_r == win_r && nxt_player_c == win_c){
                win = true;
            }

            player_r = nxt_player_r, player_c = nxt_player_c;
            player.push_back({player_r, player_c});

            if(mummy_r != -1){
                white_mummy_move(wall, mummy_r, mummy_c, caught, player_r, player_c,
                                 mummy2_r, mummy2_c, scorpion_r, scorpion_c, trap_r, trap_c, window, win_r, win_c);
            }
            mummy.push_back({mummy_r, mummy_c});

            if(mummy2_r != -1){
                white_mummy_move(wall, mummy2_r, mummy2_c, caught, player_r, player_c,
                                 mummy_r, mummy_c, scorpion_r, scorpion_c, trap_r, trap_c, window, win_r, win_c);
            }
            mummy2.push_back({mummy2_r, mummy2_c});

            if(scorpion_r != -1){
                white_scorpion_move(wall, scorpion_r, scorpion_c, caught, player_r, player_c,
                                    mummy_r, mummy_c, mummy2_r, mummy2_c, trap_r, trap_c, window, win_r, win_c);
            }
            scorpion.push_back({scorpion_r, scorpion_c});
        }
    }
    return 1;
}

int main(){
    // ref maze: https://postimg.cc/XGWs1z5c (player voi mummy lech 1 o)
    vector<string> maze_6_1 = {
        "......",
        "......",
        ".....T",
        ".....P",
        ".....M",
        "......",
    };
    vector<vector<int>> wall_6_1 = {
        {5, 4, 12, 12, 4, 6},
        {11, 11, 5, 4, 8, 2},
        {5, 14, 1, 0, 4, 2},
        {1, 12, 0, 0, 8, 2},
        {1, 6, 1, 2, 5, 2},
        {11, 9, 8, 8, 8, 10}
    };

    // ref maze: https://postimg.cc/HVp8V5q4
    vector<string> maze_6_2 = {
        "......",
        "......",
        "......",
        "M....P",
        "......",
        "......",
    };
    vector<vector<int>> wall_6_2 = {
        {7, 13, 4, 12, 4, 6},
        {1, 14, 1, 14, 1, 2},
        {1, 6, 1, 12, 2, 3},
        {1, 8, 8, 6, 9, 2},
        {1, 4, 14, 1, 14, 3},
        {9, 8, 14, 9, 12, 10}
    };

    // ref maze (8x8): https://postimg.cc/bGrYr3Qk/b9f376e3
    vector<string> maze_8_1 = {
        "..S.....",
        "........",
        "........",
        "........",
        ".......M",
        "........",
        "........",
        "......P.",
    };
    vector<vector<int>> wall_8_1 = {
        {5, 4, 4, 4, 4, 4, 4, 14},
        {9, 8, 0, 8, 2, 1, 0, 6},
        {7, 13, 0, 4, 0, 2, 1, 2},
        {3, 7, 1, 2, 1, 0, 8, 2},
        {9, 2, 1, 2, 1, 0, 4, 2},
        {13, 0, 0, 2, 9, 0, 2, 11},
        {5, 0, 10, 9, 6, 3, 11, 7},
        {11, 9, 12, 12, 8, 8, 12, 10}
    };

    sf::RenderWindow window(sf::VideoMode(640,640), "Mummy Game");

    enum GameState {
        MENU, PLAYING, GAME_OVER, GAME_WIN, NEXT_LEVEL
    };
    GameState state = MENU;
    int cur_level = 0;
    while(window.isOpen()){

        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        
        if(state == MENU){
            draw_menu(window);
            if(handle_menu_input(window)) state = PLAYING;
        }
        else if(state == PLAYING){
            int result;
            if(cur_level == 0) result = level_0(window);
            else if(cur_level == 1) result = run_level(maze_6_1, wall_6_1, window, 6, 6, 5, 3);
            else if(cur_level == 2) result = run_level(maze_6_2, wall_6_2, window, 6, 6, 0, 0);
            else if(cur_level == 3) result = run_level(maze_8_1, wall_8_1, window, 8, 8, 0, 0);
            
            if(result == 1){
                save_progress(cur_level);
                if(cur_level == 3){
                    state = GAME_WIN;
                }
                else cur_level++, state = NEXT_LEVEL;
            }
            else if(result == 0)
                state = GAME_OVER;
        }
        else if(state == GAME_OVER){
            draw_game_over(window);
            if(handle_game_over_input(window)) state = MENU;
        }
        else if(state == GAME_WIN){
            draw_game_win(window);
            if(handle_game_win_input(window)) state = MENU;
        }
        else if(state == NEXT_LEVEL){
            draw_next_level_screen(window);
            if(handle_next_level_input(window)) state = PLAYING;
        }

        window.display();
    }
    return 0;
}
