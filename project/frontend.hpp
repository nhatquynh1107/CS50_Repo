#ifndef FRONTEND_HPP
#define FRONTEND_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

void draw_maze(sf::RenderWindow& window, const std::vector<std::vector<int>>& wall);
void draw_entity(sf::RenderWindow& window, int r, int c, sf::Color color);
void draw_trap(sf::RenderWindow& window, int r, int c, sf::Color color);

void draw_menu(sf::RenderWindow& window);
bool handle_menu_input(sf::RenderWindow& window);

void draw_game_over(sf::RenderWindow& window);
bool handle_game_over_input(sf::RenderWindow& window);

void draw_game_win(sf::RenderWindow& window);
bool handle_game_win_input(sf::RenderWindow& window);

void draw_next_level_screen(sf::RenderWindow& window);
bool handle_next_level_input(sf::RenderWindow& window);
void draw_hud(sf::RenderWindow &window, int moves, int mummy_alive, int scorpion_alive);

#endif
