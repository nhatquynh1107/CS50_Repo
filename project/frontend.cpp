#include "frontend.hpp"

#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>
#include <string>

using namespace std;

void draw_maze(sf::RenderWindow& window,
               const vector<vector<int>>& wall)
{
    int rows = wall.size();
    int cols = wall[0].size();
    const float TILE = 64;
    sf::RectangleShape cell(sf::Vector2f(TILE, TILE));
    cell.setFillColor(sf::Color(60, 60, 60)); // floor

    // wall line
    sf::RectangleShape line;
    line.setFillColor(sf::Color::White);

    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < cols; c++)
        {
            // draw tile
            float x = c * TILE;
            float y = r * TILE;
            cell.setPosition(x, y);
            window.draw(cell);

            int mask = wall[r][c];

            // draw wall top
            if(mask & 4)
            {
                line.setSize(sf::Vector2f(TILE, 3));
                line.setPosition(x, y);
                window.draw(line);
            }

            // draw wall bottom
            if(mask & 8)
            {
                line.setSize(sf::Vector2f(TILE, 3));
                line.setPosition(x, y + TILE - 3);
                window.draw(line);
            }

            // draw wall left
            if(mask & 1)
            {
                line.setSize(sf::Vector2f(3, TILE));
                line.setPosition(x, y);
                window.draw(line);
            }

            // draw wall right
            if(mask & 2)
            {
                line.setSize(sf::Vector2f(3, TILE));
                line.setPosition(x + TILE - 3, y);
                window.draw(line);
            }
        }
    }
}

void draw_entity(sf::RenderWindow& window, int r, int c, sf::Color color)
{
    const float TILE = 64;
    sf::CircleShape shape(TILE/3);
    shape.setFillColor(color);
    shape.setPosition(c * TILE + TILE/6, r * TILE + TILE/6);
    window.draw(shape);
}

void draw_trap(sf::RenderWindow& window, int r, int c, sf::Color color)
{
    if(r == -1) return;
    const float TILE = 64;
    sf::CircleShape shape(TILE/5);
    shape.setFillColor(color);
    shape.setPosition(c * TILE + TILE/10, r * TILE + TILE/10);
    window.draw(shape);
}

void draw_menu(sf::RenderWindow &window)
{
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text title;
    title.setFont(font);
    title.setString("Beauty and flipyflop");
    title.setCharacterSize(48);
    title.setFillColor(sf::Color::White);
    title.setPosition(80, 40);

    // Player Controls
    sf::Text controls;
    controls.setFont(font);
    controls.setCharacterSize(24);
    controls.setFillColor(sf::Color::White);
    controls.setString(
        "Move (W/A/S/D). Press SPACE to stay still.\n"
        "Press 'R' to redo, 'M' to reset level, 'Q' to quit.\n"
    );
    controls.setPosition(60, 140);

    // MOB movement info
    sf::Text info;
    info.setFont(font);
    info.setCharacterSize(24);
    info.setFillColor(sf::Color::Yellow);
    info.setString(
        "Mummy: moves 2 tiles toward you every turn.\n"
        "Scorpion: moves 1 tile toward you every turn.\n"
    );
    info.setPosition(60, 220);

    // Start Prompt
    sf::Text start;
    start.setFont(font);
    start.setCharacterSize(30);
    start.setFillColor(sf::Color::Cyan);
    start.setString("Press ENTER to start");
    start.setPosition(100, 330);

    // Project Info
    sf::Text project_info;
    project_info.setFont(font);
    project_info.setCharacterSize(20);
    project_info.setFillColor(sf::Color::Green);
    project_info.setString(
        "Project Title: Beauty and flipyflop\n"
        "Name: Nhat Quynh\n"
        "GitHub Username: nhatquynh1107\n"
        "edX Username: nhatquynh1107\n"
        "City & Country: Ho Chi Minh City, Vietnam\n"
        "Date: [November 14, 2025]"
    );
    project_info.setPosition(60, 400);

    // Drawing everything
    window.draw(title);
    window.draw(controls);
    window.draw(info);
    window.draw(start);
    window.draw(project_info);
}

bool handle_menu_input(sf::RenderWindow &window)
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            window.close();
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Enter)
                return true;
        }
    }
    return false;
}


void draw_game_over(sf::RenderWindow& window)
{
    sf::Font font;
    if(!font.loadFromFile("arial.ttf"))
        return;

    sf::Text title("GAME OVER", font, 70);
    title.setFillColor(sf::Color::Red);
    title.setPosition(100, 150);

    sf::Text subtitle("Press ENTER to return to menu", font, 30);
    subtitle.setFillColor(sf::Color::White);
    subtitle.setPosition(110, 300);

    window.draw(title);
    window.draw(subtitle);
}

bool handle_game_over_input(sf::RenderWindow& window)
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window.close();
            return false;
        }

        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Enter)
            {
                return true; // back to menu
            }
        }
    }
    return false;
}

void draw_game_win(sf::RenderWindow& window)
{
    sf::Font font;
    if(!font.loadFromFile("arial.ttf"))
        return;

    sf::Text title("YOU WIN!", font, 70);
    title.setFillColor(sf::Color::Green);
    title.setPosition(140, 150);

    sf::Text subtitle("Press ENTER to return to menu", font, 30);
    subtitle.setFillColor(sf::Color::White);
    subtitle.setPosition(110, 300);

    window.draw(title);
    window.draw(subtitle);
}

bool handle_game_win_input(sf::RenderWindow& window)
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window.close();
            return false;
        }

        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Enter)
            {
                return true; // back to menu
            }
        }
    }
    return false;
}

void draw_next_level_screen(sf::RenderWindow &window) {
    sf::Font font;
    if(!font.loadFromFile("arial.ttf"))
        return;

    window.clear(sf::Color::Black);

    sf::Text title;
    title.setFont(font);
    title.setString("You have escaped the maze!\nLet's move to the next one!");
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::White);
    title.setPosition(70, 150);

    sf::Text small;
    small.setFont(font);
    small.setString("Press ENTER to continue");
    small.setCharacterSize(20);
    small.setFillColor(sf::Color::Green);
    small.setPosition(150, 300);

    window.draw(title);
    window.draw(small);
}

bool handle_next_level_input(sf::RenderWindow &window) {
    sf::Event event;
    while(window.pollEvent(event)) {

        if(event.type == sf::Event::Closed) {
            window.close();
        }

        if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::Enter) {
                return true;
            }
        }
    }
    return false;
}

void draw_hud(sf::RenderWindow &window, 
               int moves, int mummy_alive, int scorpion_alive)
{
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text hud;
    hud.setFont(font);
    hud.setCharacterSize(22);
    hud.setFillColor(sf::Color::White);

    hud.setString(
        "Moves: " + std::to_string(moves) +
        "  Mummies alive: " + std::to_string(mummy_alive) +
        "  Scorpions alive: " + std::to_string(scorpion_alive)
    );

    hud.setPosition(40, 10);

    window.draw(hud);
}



