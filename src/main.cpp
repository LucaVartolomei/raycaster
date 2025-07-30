#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

const int mapWidth=24;
const int mapHeight=24;
const int screenWidth=1280;
const int screenHeight=720;

int worldMap[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

bool canMove(int x, int y){
    if(x<0||x>mapWidth||y<0||y>mapHeight){
        return false;
    }
    return worldMap[x][y]==0;
}

sf::Vector2f rotate(sf::Vector2f vec, float val){
    return sf::Vector2f(
        vec.x * cos(val) - vec.y * sin(val),
        vec.x * sin(val) + vec.y * cos(val)
    );
}

int main(){

    sf::Vector2f pos(22, 12);
    sf::Vector2f dir(-1, 0);
    sf::Vector2f plane(0, 0.66);

    double time = 0;
    double oldTime = 0;

    sf::VertexArray lines(sf::PrimitiveType::Lines, 2*screenWidth);

    float moveSpeed = 5.0f;
    float rotationSpeed = 2.0f;

    sf::RenderWindow window(sf::VideoMode({screenWidth+1, screenHeight}), "Raycaster");
    window.setFramerateLimit(30);

    sf::Clock clock;

    while (window.isOpen())
    {
        int32_t dt_i = clock.restart().asMilliseconds();
        float dt = dt_i / 1000.0f;
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        //handle keyboard input

        float moveForward = 0.0f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
            moveForward = 1.0f;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
            moveForward = -1.0f;
        }

        if(moveForward != 0.0f){
            sf::Vector2f moveVec = dir * moveForward * moveSpeed * dt;
            if(canMove(pos.x + moveVec.x, pos.y)){
                pos.x += moveVec.x;
            }
            if(canMove(pos.x, pos.y + moveVec.y)){
                pos.y += moveVec.y;
            }
        }

        float rotateDir = 0.0f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
            rotateDir = 1.0f;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
            rotateDir = -1.0f;
        }

        if(rotateDir != 0.0f){
            float rotation = rotationSpeed * rotateDir * dt;
            dir = rotate(dir, rotation);
            plane = rotate(plane, rotation);
        }

        for(int x = 0; x < screenWidth; x++){

            double cameraX = 2.0f * x / screenWidth - 1.0f;
            sf::Vector2f rayDir(dir.x + plane.x * cameraX, 
                                dir.y + plane.y * cameraX);
            
            sf::Vector2i map((int)pos.x, (int)pos.y);
            
            double sideDistX;
            double sideDistY;

            double deltaDistX = (rayDir.x == 0 ? 1e30 : sqrt(1.0f + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x)));
            double deltaDistY = (rayDir.y == 0 ? 1e30 : sqrt(1.0f + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y)));
            double perpWallDist;

            int stepX;
            int stepY;

            int hit = 0; 
            int side;

            if(rayDir.x < 0.0f){
                stepX = -1;
                sideDistX = (pos.x - map.x) * deltaDistX;
            }
            else{
                stepX = 1;
                sideDistX = (map.x + 1.0 - pos.x) * deltaDistX;
            }

            if(rayDir.y < 0.0f){
                stepY = -1;
                sideDistY = (pos.y - map.y) * deltaDistY;
            }
            else{
                stepY = 1;
                sideDistY = (map.y + 1.0 - pos.y) * deltaDistY;
            }

            while(hit == 0){
                if(sideDistX < sideDistY){
                    sideDistX += deltaDistX;
                    map.x += stepX;
                    side = 0;
                    perpWallDist = (map.x - pos.x + (1-stepX) / 2) / rayDir.x;
                }
                else{
                    sideDistY += deltaDistY;
                    map.y += stepY;
                    side = 1;
                    perpWallDist = (map.y - pos.y + (1-stepY) / 2) / rayDir.y;
                }
                if(worldMap[map.x][map.y] > 0){
                    hit = 1;
                }
            }

            int lineHeight = (int)(screenHeight / perpWallDist);

            int drawStart = - lineHeight / 2 + screenHeight / 2;
            if(drawStart < 0) drawStart = 0;
            int drawEnd = lineHeight / 2 + screenHeight / 2;
            if(drawEnd>=screenHeight)drawEnd = screenHeight;

            sf::Color color;

            switch(worldMap[map.x][map.y]){
                case 1:color=sf::Color::Red;break;
                case 2:color=sf::Color::Green;break;
                case 3:color=sf::Color::Blue;break;
                case 4:color=sf::Color::White;break;
                default:color=sf::Color::Yellow;break;
            }

            if(side){
                color.r/=2;
                color.g/=2;
                color.b/=2;
            }
            
            lines[x*2].position = sf::Vector2f((float)x, (float)drawStart);
            lines[x*2].color = color;

            lines[x*2+1].position = sf::Vector2f((float)x, (float)drawEnd);
            lines[x*2+1].color = color;

        }

        window.clear();
        window.draw(lines);
        window.display();
    }
}