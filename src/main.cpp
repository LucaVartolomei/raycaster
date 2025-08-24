#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

const int mapWidth=24;
const int mapHeight=24;

const int screenWidth=1280;
const int screenHeight=720;

const int texWidth = 64;
const int texHeight = 64;
const int texture_size = 512;
const int texture_wall_size = 64;

int worldMap[mapWidth][mapHeight]=
{
  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
  {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
  {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
  {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
  {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
  {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
  {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};

bool canMove(sf::Vector2f position, sf::Vector2f size){
    sf::Vector2i upper_left(position - size / 2.0f);
    sf::Vector2i lower_right(position + size / 2.0f);
    if(upper_left.x < 0 || upper_left.y < 0 || lower_right.x >= mapWidth || lower_right.y >= mapHeight){
        return false;
    }
    for(int x = upper_left.x; x <= lower_right.x; x++){
        for(int y = upper_left.y; y <= lower_right.y; y++){
            if(worldMap[x][y] != 0){
                return false;
            }
        }
    }
    return true;
}

sf::Vector2f rotate(sf::Vector2f vec, float val){
    return sf::Vector2f(
        vec.x * cos(val) - vec.y * sin(val),
        vec.x * sin(val) + vec.y * cos(val)
    );
}

int main(){

    sf::Texture texture;
    if(!texture.loadFromFile("Z:/Programming Projects/raycaster/data/wolftextures.png")){
        std::cout << "Cannot open texture from file!\n";
        return EXIT_FAILURE;
    }

    sf::Image floorTex;
    sf::Image ceilingTex;

    if(!floorTex.loadFromFile("Z:/Programming Projects/raycaster/data/greystone.png")){
        std::cout << "Cannot open texture from file!\n";
        return EXIT_FAILURE;
    }

    if(!ceilingTex.loadFromFile("Z:/Programming Projects/raycaster/data/wood.png")){
        std::cout << "Cannot open texture from file!\n";
        return EXIT_FAILURE;
    }

    sf::RenderStates state(&texture);

    sf::Vector2f pos(22, 11.5);
    sf::Vector2f dir(-1, 0);
    sf::Vector2f plane(0, 0.66);

    float size_f = 0.375f;
    sf::Vector2f size(size_f, size_f);

    double time = 0;
    double oldTime = 0;

    sf::VertexArray lines(sf::PrimitiveType::Lines, 2*screenWidth);

    sf::VertexArray pixelArray(sf::PrimitiveType::Points, screenHeight * screenWidth);

    float moveSpeed = 5.0f;
    float rotationSpeed = 2.0f;

    sf::RenderWindow window(sf::VideoMode({screenWidth+1, screenHeight}), "Raycaster");
    window.setFramerateLimit(30);

    sf::Clock clock;

    sf::Texture aux;

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
            if(canMove(sf::Vector2f(pos.x + moveVec.x, pos.y), size)){
                pos.x += moveVec.x;
            }
            if(canMove(sf::Vector2f(pos.x, pos.y + moveVec.y), size)){
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

        sf::Image screenBuffer(sf::Vector2u(screenWidth, screenHeight), sf::Color::Black);

        for(int y = 0; y < screenHeight; y++){
            sf::Vector2f rayDir0 = dir - plane;
            sf::Vector2f rayDir1 = dir + plane;

            int p = y - screenHeight / 2;

            float posZ = 0.30f * screenHeight;

            float rowDistance = posZ / p;

            float floorStepX = rowDistance * (rayDir1.x - rayDir0.x) / screenHeight;
            float floorStepY = rowDistance * (rayDir1.y - rayDir0.y) / screenHeight;
            rowDistance=rowDistance*1.78;
            sf::Vector2f floor = pos + rowDistance * rayDir0;

            for(int x = 0; x < screenWidth; x++){
                sf::Vector2i cell((int)floor.x, (int)floor.y);

                int tx = (int)(texWidth * (floor.x - cell.x)) & (texWidth - 1);
                int ty = (int)(texHeight * (floor.y - cell.y)) & (texHeight - 1);

                floor.x += floorStepX;
                floor.y += floorStepY;

                sf::Color floorColor = floorTex.getPixel(sf::Vector2u(tx, ty));
                floorColor.r /= 2, floorColor.g /= 2, floorColor.b /= 2;
                int index = y * screenWidth + x;
                pixelArray[index].position = sf::Vector2f(x, y);
                pixelArray[index].color = floorColor;

                sf::Color ceilingColor = ceilingTex.getPixel(sf::Vector2u(tx, ty));
                ceilingColor.r /= 2, ceilingColor.g /= 2, ceilingColor.b /= 2;
                index = (screenHeight-y-1) * screenWidth + x;
                pixelArray[index].position = sf::Vector2f(x, screenHeight-y-1);
                pixelArray[index].color = ceilingColor;

                screenBuffer.setPixel(sf::Vector2u(x, y), floorColor);
                screenBuffer.setPixel(sf::Vector2u(x, screenHeight-y-1), ceilingColor);
            }
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

            int texNum = worldMap[map.x][map.y] - 1;

            sf::Vector2i textureCoords(
                    texNum * texture_wall_size % texture_size,
                    texNum * texture_wall_size / texture_size * texture_wall_size
            );

            double wallX;
            if(side == 0){
                wallX = pos.y + perpWallDist * rayDir.y;
            }
            else{
                wallX = pos.x + perpWallDist * rayDir.x;
            }
            wallX = wallX - floor(wallX);

            int texX = (int)(wallX * (double)texture_wall_size);
            if((side == 0 && rayDir.x > 0) || (side == 1 && rayDir.y < 0)){
                texX = texture_wall_size - texX - 1;
            }
            textureCoords.x += texX;

            sf::Color color = sf::Color::White;

            if(side){
                color.r /= 2;
                color.g /= 2;
                color.b /= 2;
            }
            
            lines[x * 2].position = sf::Vector2f((float)x, (float)drawStart);
            lines[x * 2].color = color;
            lines[x * 2].texCoords = sf::Vector2f((float)textureCoords.x, (float)textureCoords.y);

            lines[x * 2 + 1].position = sf::Vector2f((float)x, (float)drawEnd);
            lines[x * 2 + 1].color = color;
            lines[x * 2 + 1].texCoords = sf::Vector2f((float)textureCoords.x, (float)textureCoords.y + texture_wall_size - 1);

        }

        sf::Texture tx;
        tx.loadFromImage(screenBuffer);
        
        sf::Sprite auxx(tx);

        window.clear();
        //window.draw(auxx);
        window.draw(pixelArray);
        window.draw(lines, state);
        window.display();
    }
}