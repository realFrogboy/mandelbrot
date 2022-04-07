#include <SFML/Graphics.hpp>
#include <emmintrin.h>
#include <immintrin.h>

const float len    = 800.f    , wig      = 600.f;
const float dx     = 2.f / len, dy       = 2.f / wig;
const float ROI_X  = -1.325f  , ROI_Y    = 0;
const float offset = 10.f     , ShOffset = 100.f;

const int   nMax   = 256;

const __m256 r2Max = _mm256_set_ps(100.f, 100.f, 100.f, 100.f, 100.f, 100.f, 100.f, 100.f);
const __m256 off   = _mm256_set_ps(  7.f,   6.f,   5.f,   4.f,   3.f,   2.f,   1.f,   0.f);
const __m256 nmax  = _mm256_set_ps( nMax,  nMax,  nMax,  nMax,  nMax,  nMax,  nMax,  nMax);

int main() {
    sf::RenderWindow window(sf::VideoMode(len, wig), "Mandelbrot");

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
                        xC -= dx * ((sf::Keyboard::LShift) ? ShOffset : offset);
                        break;
                    
                    case sf::Keyboard::Right:
                        xC += dx * ((sf::Keyboard::LShift) ? ShOffset : offset);
                        break; 

                    case sf::Keyboard::Down:
                        yC += dx * ((sf::Keyboard::LShift) ? ShOffset : offset);
                        break;

                    case sf::Keyboard::Up:
                        yC -= dx * ((sf::Keyboard::LShift) ? ShOffset : offset);
                        break;

                    case sf::Keyboard::A:
                        scale += dx * ((sf::Keyboard::LShift) ? ShOffset : offset);
                        break;

                    case sf::Keyboard::Z:
                        scale -= dx * ((sf::Keyboard::LShift) ? ShOffset : offset);
                }
            }
        }
        int ix = 0, iy = 0;

        for (; iy < 600; iy++) { 
            float x0 = (-400.f * dx + ROI_X + xC) * scale, 
                  y0 = ((iy - 300.f) * dy + ROI_Y + yC) * scale;
            
            for (ix = 0; ix < 800; ix += 8, x0 += dx * 8) {
                __m256 displ = _mm256_mul_ps (off, _mm256_set_ps(dx, dx, dx, dx, dx, dx, dx, dx));

                __m256 X0 = _mm256_add_ps (_mm256_set_ps(x0, x0, x0, x0, x0, x0, x0, x0), displ);
                __m256 Y0 =             _mm256_set_ps(y0, y0, y0, y0, y0, y0, y0, y0);

                __m256 X  = X0, Y = Y0;
                
                __m256i N = _mm256_setzero_si256();

                for (int n = 0; n < nMax; n++) {
                    __m256 x2 = _mm256_mul_ps (X, X),
                           y2 = _mm256_mul_ps (Y, Y);
                           
                    __m256 r2 = _mm256_add_ps (x2, y2);

                    __m256 cmp = _mm256_cmp_ps (r2, r2Max, 2);
                    int mask   = _mm256_movemask_ps (cmp);
                    if (!mask) break;

                    N = _mm256_sub_epi32 (N, _mm256_castps_si256 (cmp));

                    __m256 xy = _mm256_mul_ps (X, Y);

                    X = _mm256_add_ps (_mm256_sub_ps (x2, y2), X0);
                    Y = _mm256_add_ps (_mm256_add_ps (xy, xy), Y0);
                }
                sf::VertexArray points(sf::Points, 8);

                for (int displ = 0; displ < 8; displ++) {
                    int* pn = (int*) &N;

                    points[displ].position = sf::Vector2f(ix + displ, iy);
                    (pn[displ] == nMax) ? points[displ].color = sf::Color::Black : points[displ].color = sf::Color::Cyan;
                }
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