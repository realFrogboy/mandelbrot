#include <SFML/Graphics.hpp>

const int nMax  = 256;
const int r2Max = 100;
const float dx  = 2.f / 800.f;
const float dy  = 2.f / 600.f;

const float ROI_X = -1.325f,
            ROI_Y = 0;

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mandelbrot");

    sf::Clock clock;
    float lastTime = 0;

    float xC = 0.f, yC = 0.f, scale = 1.f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed) {
                switch(event.key.code) {
                    case sf::Keyboard::Left:
                        xC -= dx * ((sf::Keyboard::LShift) ? 100.f : 10.f);
                        break;
                    
                    case sf::Keyboard::Right:
                        xC += dx * ((sf::Keyboard::LShift) ? 100.f : 10.f);
                        break; 

                    case sf::Keyboard::Down:
                        yC += dx * ((sf::Keyboard::LShift) ? 100.f : 10.f);
                        break;

                    case sf::Keyboard::Up:
                        yC -= dx * ((sf::Keyboard::LShift) ? 100.f : 10.f);
                        break;

                    case sf::Keyboard::A:
                        scale += dx * ((sf::Keyboard::LShift) ? 100.f : 10.f);
                        break;

                    case sf::Keyboard::Z:
                        scale -= dx * ((sf::Keyboard::LShift) ? 100.f : 10.f);
                }
            }
        }

        int ix = 0, iy = 0;
        
        for (; iy < 600; iy++) {
            float   x0 = (-400.f * dx + ROI_X + xC) * scale, 
                    y0 = ((iy - 300.f) * dy + ROI_Y + yC) * scale;

            for (ix = 0; ix < 800; ix++, x0 += dx) {
                float X = x0, Y = y0;

                int N = 0;
                for (; N < nMax; N++) {
                    float x2 = X*X,
                          y2 = Y*Y,
                          xy = X*Y;

                    float r2 = x2 + y2;
                       
                    if (r2 >= r2Max) break;
                        
                    X = x2 - y2 + x0,
                    Y = xy + xy + y0;
                }

                sf::VertexArray points(sf::Points, 1);
                points[0].position = sf::Vector2f(ix, iy);
                
                (N == nMax) ? points[0].color = sf::Color::Black : points[0].color = sf::Color::Cyan;

                window.draw(points);
            }
        }

        float curTime = clock.restart().asSeconds();
        float fps = 1.f / curTime;
        lastTime = curTime;

        char sfps[8] = {};
        sprintf(sfps, "%7.5lf", fps);

        sf::Font font;
        font.loadFromFile("arial.ttf");

        sf::String str;
        str = sfps;
        sf::Text text(str, font);
        text.setCharacterSize(50);
        text.setFillColor(sf::Color::Red);

        window.draw(text);
        window.display();
    }
}