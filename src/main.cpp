#include <SFML/Graphics.hpp>
#include <cmath>

const int mapWidth=24;
const int mapHeight=24;
const int screenWidth=640;
const int screenHeight=480;

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

int main(){

    sf::Vector2f pos(22, 12);
    sf::Vector2f dir(-1, 0);
    sf::Vector2f plane(0, 0.66);

    double time = 0;
    double oldTime = 0;

    sf::RenderWindow window(sf::VideoMode({screenWidth, screenHeight}), "Raycaster");

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        for(int x = 0; x < screenWidth; x++){

            double cameraX = 2.0 * x / screenWidth - 1;
            sf::Vector2f rayDir(dir.x + plane.x * cameraX, 
                                dir.y + plane.y * cameraX);
            
            sf::Vector2i map((int)pos.x, (int)pos.y);
            
            double sideDistX;
            double sideDistY;

            double deltaDistX = (rayDir.x == 0 ? 1e30 : sqrt(1 + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x)));
            double deltaDistY = (rayDir.y == 0 ? 1e30 : sqrt(1 + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y)));
            double perpWallDist;

            int stepX;
            int stepY;

            int hit = 0; 
            int side;

            if(rayDir.x < 0){
                stepX = -1;
                sideDistX = (pos.x - map.x) * deltaDistX;
            }
            else{
                stepX = 1;
                sideDistX = (map.x + 1.0 - pos.x) * deltaDistX;
            }

            if(rayDir.y < 0){
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
                    perpWallDist = (map.x - pos.x + (1-stepX) / 2);
                }
                else{
                    sideDistY += deltaDistY;
                    map.y += stepY;
                    side = 1;
                    perpWallDist = (map.y - pos.y + (1-stepY) / 2);
                }
                if(worldMap[map.x][map.y] != 0){
                    hit = 1;
                }
            }
        }

        window.clear();
        
        window.display();
    }
}