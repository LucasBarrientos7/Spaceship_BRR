#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <list>
using namespace std;
#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
#define BARRA 32

void gotoxy(int x, int y)
{
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;

    SetConsoleCursorPosition(hCon, dwPos);
}

// Función para ocultar el cursor de la terminal
void OcultarCursor(){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 50;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hCon, &cci);
}

// Dibuja los limites del juego
void pintar_limites()
{
    for (int i = 2; i < 78; i++)
    {
        gotoxy(i, 3);
        printf("%c", 205);
        gotoxy(i, 33);
        printf("%c", 205);
    }
    for (int i = 4; i < 33; i++)
    {
        gotoxy(2, i);
        printf("%c", 186);
        gotoxy(77, i);
        printf("%c", 186);
    }
    gotoxy(2, 3);
    printf("%c", 201);
    gotoxy(2, 33);
    printf("%c", 200);
    gotoxy(77, 3);
    printf("%c", 187);
    gotoxy(77, 33);
    printf("%c", 188);
}

// Constructor de NAVE
class NAVE
{
    int x, y;
    int corazones;
    int vidas;

public:
    NAVE(int _x, int _y, int _corazones, int _vidas) : x(_x), y(_y), corazones(_corazones), vidas(_vidas) {}
    int X() { return x; }
    int Y() { return y; }
    int VIDAS() { return vidas; }
    void COR() { corazones--; }
    void pintar();
    void borrar();
    void mover();
    void pintar_corazones();
    void morir();
};

// Dibuja la nave en pantalla
void NAVE::pintar()
{
    gotoxy(x, y);
    printf("  %c", 30);
    gotoxy(x, y + 1);
    printf(" %c%c%c", 40, 207, 41);
    gotoxy(x, y + 2);
    printf("%c%c %c%c", 30, 190, 190, 30);
}

// Borra la nave de la pantalla
void NAVE::borrar()
{
    gotoxy(x, y);
    printf("        ");
    gotoxy(x, y + 1);
    printf("        ");
    gotoxy(x, y + 2);
    printf("        ");
}

// Mueve la nave en la pantalla utilizando las flechas de direccion del teclado
void NAVE::mover()
{
    if (_kbhit())
    {
        char tecla = _getch();
        borrar();
        if (tecla == IZQUIERDA && x > 3)
            x--;
        if (tecla == DERECHA && x + 6 < 77)
            x++;
        if (tecla == ARRIBA && y > 4)
            y--;
        if (tecla == ABAJO && y + 3 < 33)
            y++;
        if (tecla == 'e')
            corazones--;
        pintar();
        pintar_corazones();
    }
}

// Dibuja los corazones en pantalla
void NAVE::pintar_corazones()
{
    gotoxy(50, 2);
    printf("LIFE %d", vidas);
    gotoxy(64, 2);
    printf("Health ");
    gotoxy(70, 2);
    printf("      ");
    for (int i = 0; i < corazones; i++)
    {
        gotoxy(70 + i, 2);
        printf("%c", 3);
    }
}

// Cuando la cantidad de corazones llega a cero, dibuja la explosion de la nave
void NAVE::morir()
{
    if (corazones == 0)
    {
        borrar();
        gotoxy(x, y);
        printf("   **   ");
        gotoxy(x, y + 1);
        printf("  **   ");
        gotoxy(x, y + 2);
        printf("   **   ");
        Sleep(200);

        borrar();
        gotoxy(x, y);
        printf(" * ** *");
        gotoxy(x, y + 1);
        printf("  ** ");
        gotoxy(x, y + 2);
        printf(" * ** *");
        Sleep(200);
        borrar();

        vidas--;
        corazones = 3;
        pintar_corazones();
        pintar();
    }
}

// Constructor de Asteroide
class AST
{
    int x, y;

public:
    AST(int _x, int _y) : x(_x), y(_y) {}
    void pintar();
    void mover();
    void choque(NAVE &N);
    int X() { return x; }
    int Y() { return y; }
};

// Dibuja el asteroide en pantalla
void AST::pintar()
{
    gotoxy(x, y);
    printf("%c", 184);
}

// Mueve de manera descendente el asteroide por la pantalla
void AST::mover()
{
    gotoxy(x, y);
    printf(" ");
    y++;
    if (y > 32)
    {
        x = rand() % 71 + 4;
        y = 4;
    }
    pintar();
}

// Choque del Asteroide con la nave
void AST::choque(NAVE &N)
{
    if (x >= N.X() && x < N.X() + 6 && y >= N.Y() && y <= N.Y() + 2)
    {
        N.COR();
        N.borrar();
        N.pintar();
        N.pintar_corazones();
        x = rand() % 71 + 4;
        y = 4;
    }
}

//Constructor de la Bala
class BALA
{
    int x, y;

public:
    BALA(int _x, int _y) : x(_x), y(_y) {}
    int X() { return x; }
    int Y() { return y; }
    void mover();
    bool fuera();
};

// Mueve la pantalla de manera ascendente de la Bala
void BALA::mover()
{
    gotoxy(x, y);
    printf(" ");
    y--;
    gotoxy(x, y);
    printf("%c", 24);
}

// Verifica si la bala salio fuera de la pantalla
bool BALA::fuera()
{
    if (y == 4)
        return true;
    return false;
}

int main()
{
    OcultarCursor();
    pintar_limites();
    NAVE N(37, 30, 3, 3);
    N.pintar();
    N.pintar_corazones();

    list<AST *> A;
    list<AST *>::iterator itA;
    for (int i = 0; i < 5; i++)
    {
        A.push_back(new AST(rand() % 75 + 3, rand() % 5 + 4));
    }

    list<BALA *> B;
    list<BALA *>::iterator it;

    bool game_over = false;

    int puntos = 0;

    while (!game_over)
    {
        gotoxy(4,2); printf("SCORE: %d", puntos);
        if (_kbhit())
        {
            char tecla = _getch();
            if (tecla == BARRA)
                B.push_back(new BALA(N.X() + 2, N.Y() - 1));
        }
        // <-Correcion del error
        for (it = B.begin(); it != B.end();)
        {
            (*it)->mover();
            if ((*it)->fuera())
            {
                gotoxy((*it)->X(), (*it)->Y());
                printf(" ");
                delete (*it);
                it = B.erase(it);
            }
            else
                it++;
        }

        for (itA = A.begin(); itA != A.end(); itA++)
        {
            (*itA)->mover();
            (*itA)->choque(N);
        }

        for (itA = A.begin(); itA != A.end(); itA++)
        {
            for(it =B.begin(); it != B.end(); it++){
                if ((*itA)->X() == (*it)->X() && ( (*itA)->Y()+1 == (*it)->Y() || (*itA)->Y() == (*it)->Y() )){
                    gotoxy((*it)->X(), (*it)->Y()); printf(" ");
                    delete(*it);
                    it = B.erase(it);
                    A.push_back(new AST(rand()%74 + 3, 4));

                    gotoxy((*itA)->X(), (*itA)->Y()); printf(" ");
                    delete(*itA);
                    itA = A.erase(itA);

                    puntos +=5;
                }
                
            }
        }
        if (N.VIDAS() == 0)
        {
            game_over = true;
            N.borrar();
        }
        
        N.morir();
        N.mover();
        Sleep(30);

    }

    gotoxy(35,16); printf("GAME OVER");
    getch();
    return 0;
}
