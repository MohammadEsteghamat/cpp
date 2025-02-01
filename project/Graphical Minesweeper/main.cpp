#include <vector>
#include <ctime>
#include <windows.h>
#include <string>
using namespace std;

class CustomWindow {
public:
    CustomWindow(int x, int y);
    ~CustomWindow();

    void addRectangle(vector<vector<int>> xy_p);
    void addSymbol(vector<vector<int>> xy_c);
    void Execute();
    virtual void customFunction();
    struct Position {
        int X, Y, S;
    } mouse;
    char inputCH;
    bool isNewCh = false;
protected:
    HWND hwnd;
private:
    HINSTANCE hInstance;
    bool hasChanged = false;
    int X, Y;

    vector<vector<int>> xy_rectangles;
    vector<vector<int>> xy_symbols;

    void DrawRectangle(HDC hdc, int x, int y, int width, int height, COLORREF color);
    void DrawSymbol(HDC hdc, int x, int y, const char character, COLORREF textColor);
    void Render(HDC hdc);

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
};

void CustomWindow::customFunction() {
    // Override in derived classes
}

void CustomWindow::addRectangle(vector<vector<int>> xy_c) {
    xy_rectangles = xy_c;
    hasChanged = true;
}

void CustomWindow::addSymbol(vector<vector<int>> xy_c) {
    xy_symbols = xy_c;
    hasChanged = true;
}

void CustomWindow::Render(HDC hdc) {
    for (auto& i : xy_rectangles) {
        DrawRectangle(hdc, i[0], i[1], i[2], i[3], RGB(i[4], i[5], i[6]));
    }
    for (auto& i : xy_symbols) {
        DrawSymbol(hdc, i[0], i[1], char(i[2]), RGB(i[3], i[4], i[5]));
    }
}

CustomWindow::CustomWindow(int x, int y) : X(x), Y(y) {
    hInstance = GetModuleHandle(NULL);
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"CustomWindowClass";

    if (!RegisterClassW(&wc)) {
        exit(1);
    }

    hwnd = CreateWindowExW(
            0,
            wc.lpszClassName,
            L"Custom Window",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, X, Y,
            NULL, NULL, hInstance, this
    );

    if (!hwnd) {
        exit(1);
    }
}

CustomWindow::~CustomWindow() {
    if (hwnd) {
        DestroyWindow(hwnd);
    }
}

void CustomWindow::Execute() {
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void CustomWindow::DrawRectangle(HDC hdc, int x, int y, int width, int height, COLORREF color) {
    HBRUSH brush = CreateSolidBrush(color);
    SelectObject(hdc, brush);
    Rectangle(hdc, x, y, x + width, y + height);
    DeleteObject(brush);
}

void CustomWindow::DrawSymbol(HDC hdc, int x, int y, const char character, COLORREF textColor) {
    SetTextColor(hdc, textColor);
    TextOutA(hdc, x, y, &character, 1);
}

LRESULT CALLBACK CustomWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    CustomWindow* pThis = nullptr;

    if (msg == WM_NCCREATE) {
        pThis = static_cast<CustomWindow*>(reinterpret_cast<LPCREATESTRUCT>(lp)->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    } else {
        pThis = reinterpret_cast<CustomWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis) {
        if (pThis->hasChanged) {
            InvalidateRect(hwnd, NULL, TRUE);
            pThis->hasChanged = false;
        }
        switch (msg) {
            case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                pThis->Render(hdc);
                EndPaint(hwnd, &ps);
                break;
            }
            case WM_RBUTTONDOWN:
            case WM_LBUTTONDOWN: {
                int x = LOWORD(lp);
                int y = HIWORD(lp);
                pThis->mouse.X = x;
                pThis->mouse.Y = y;
                pThis->mouse.S = (msg == WM_LBUTTONDOWN);
                pThis->customFunction();
                break;
            }
            case WM_CHAR: {
                char typedChar = static_cast<char>(wp);
                pThis->inputCH = typedChar;
                pThis->isNewCh = true;
                pThis->customFunction();
                break;
            }
            case WM_DESTROY:
                PostQuitMessage(0);
                break;
            default:
                return DefWindowProc(hwnd, msg, wp, lp);
        }
    }

    return 0;
}

class Minesweeper{
protected:
    vector<vector<int>> grid;
    vector<vector<int>> revealed;
    vector<vector<int>> rect;

    int rowsm, colsm, mineCountm;

    void placeMines() {
        srand(time(0));
        int placedMines = 0;
        while (placedMines < mineCountm) {
            int x = rand() % rowsm;
            int y = rand() % colsm;
            if (grid[x][y] == 0) {
                ++placedMines;
                grid[x][y] = -1;
            }
        }
    }

    void calculateNeighborMines() {
        for (int i = 0; i < rowsm; ++i) {
            for (int j = 0; j < colsm; ++j) {
                if (grid[i][j] == -1) continue;
                int mineCount = 0;
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        int ni = i + dx;
                        int nj = j + dy;
                        if (ni >= 0 && ni < rowsm && nj >= 0 && nj < colsm && grid[ni][nj] == -1) {
                            ++mineCount;
                        }
                    }
                }
                grid[i][j] = mineCount;
            }
        }
    }

    void reveal(int x, int y) {
        if (x < 0 || x >= rowsm || y < 0 || y >= colsm || revealed[x][y]) return;
        revealed[x][y] = 1;
        if (grid[x][y] == 0) {
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    reveal(x + dx, y + dy);
                }
            }
        }
    }
};


class game : public CustomWindow , public Minesweeper{
private:
    int rows, cols, mineCount, minfind;
    vector<vector<int>> rect;
    int mode = 0;

    void printmenu() {
        rect.clear();
        rect.push_back({50, 40, 100, 50, 0, 255, 0});
        rect.push_back({160, 40, 100, 50, 0, 255, 0});
        rect.push_back({270, 40, 100, 50, 0, 255, 0});
        rect.push_back({380, 40, 100, 50, 0, 255, 0});
        rect.push_back({460, 200, 100, 50, 0, 255, 0});
        addRectangle(rect);

        vector<vector<int>> symbols;

        int x = 50, y = 24;


        for (char c : "rows") {
            symbols.push_back({x, y, c, 0, 0, 0});
            x += 10;
        }

        x = 160;

        for (char c : "cols") {
            symbols.push_back({x, y, c, 0, 0, 0});
            x += 10;
        }

        x = 270;

        for (char c : "min count") {
            symbols.push_back({x, y, c, 0, 0, 0});
            x += 10;
        }


        symbols.push_back({400, 50, 'e', 0, 0, 0});
        symbols.push_back({407, 50, 'n', 0, 0, 0});
        symbols.push_back({414, 50, 't', 0, 0, 0});
        symbols.push_back({418, 50, 'e', 0, 0, 0});
        symbols.push_back({426, 50, 'r', 0, 0, 0});


        symbols.push_back({496, 215, 'e', 0, 0, 0});
        symbols.push_back({504, 215, 'x', 0, 0, 0});
        symbols.push_back({512, 215, 'i', 0, 0, 0});
        symbols.push_back({516, 215, 't', 0, 0, 0});


        addSymbol(symbols);
    }
    void menu(){

        bool isfind = false;
        for (auto &Rect: rect) {
            int rectX = Rect[0];
            int rectY = Rect[1];
            int rectWidth = Rect[2];
            int rectHeight = Rect[3];

            if (mouse.X >= rectX && mouse.X < rectX + rectWidth && mouse.Y >= rectY && mouse.Y < rectY + rectHeight) {
                isfind = true;
                break;
            }
        }
        if (!isfind) {
            mouse.S = -1;
            return;
        }

        if (mouse.X >= 380 && mouse.X <= 480 && mouse.Y >= 40 && mouse.Y <= 90) { // Enter button
            SetWindowPos(hwnd, HWND_TOP, 0, 0, (cols+2) * 25 + 16, rows * 25 + 39, SWP_NOZORDER);
            cleer();
            mode = true;
            return;
        }

        if (mouse.X >= 460 && mouse.X <= 560 && mouse.Y >= 200 && mouse.Y <= 250) { // Exit button
            PostQuitMessage(0);
            return;
        }

        if (mouse.S == 1 && isNewCh) {
            isNewCh = false;

            static int input = 0;
            static int mousex = 0;
            if(mouse.X != mousex){
                input = 0;
                mousex = mouse.X;
            }

            if (inputCH >= '0' && inputCH <= '9') {
                input = input * 10 + (inputCH - '0');
            }

            vector<vector<int>> symbols;
            int x = 0, y = 0;

            if (mouse.X >= 50 && mouse.X <= 150 && mouse.Y >= 40 && mouse.Y <= 90) {
                rows = input;
            } else if (mouse.X >= 160 && mouse.X <= 260 && mouse.Y >= 40 && mouse.Y <= 90) {
                cols = input;
            } else if (mouse.X >= 270 && mouse.X <= 370 && mouse.Y >= 40 && mouse.Y <= 90) {
                mineCount = input;
            }

            string inputTextRows = to_string(rows);
            string inputTextCols = to_string(cols);
            string inputTextMin = to_string(mineCount);

            x = 80;
            y = 50;
            for (char c : inputTextRows) {
                symbols.push_back({x, y, c, 255, 0, 0});
                x += 7;
            }

            x = 180;
            y = 50;
            for (char c : inputTextCols) {
                symbols.push_back({x, y, c, 255, 0, 0});
                x += 7;
            }

            x = 280;
            y = 50;
            for (char c : inputTextMin) {
                symbols.push_back({x, y, c, 255, 0, 0});
                x += 7;
            }

            symbols.push_back({400, 50, 'e', 0, 0, 0});
            symbols.push_back({407, 50, 'n', 0, 0, 0});
            symbols.push_back({414, 50, 't', 0, 0, 0});
            symbols.push_back({418, 50, 'e', 0, 0, 0});
            symbols.push_back({426, 50, 'r', 0, 0, 0});

            symbols.push_back({496, 215, 'e', 0, 0, 0});
            symbols.push_back({504, 215, 'x', 0, 0, 0});
            symbols.push_back({512, 215, 'i', 0, 0, 0});
            symbols.push_back({516, 215, 't', 0, 0, 0});
            addSymbol(symbols);
        }
    }

    void printGrid() {
        rect.clear();
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int color = (revealed[i][j] == 1) ? RGB(255, 255, 255) : RGB(100, 100, 100);
                color = (revealed[i][j] == 2) ? RGB(255, 0, 0) : color;
                rect.push_back({j * 25, i * 25, 24, 24, GetRValue(color), GetGValue(color), GetBValue(color)});
            }
        }
        rect.push_back({cols * 25, 0, 50, 50, 155, 0, 0});
        addRectangle(rect);

        vector<vector<int>> symbols;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (revealed[i][j] == 1 && grid[i][j] > 0) {
                    symbols.push_back({j * 25 + 8, i * 25 + 5, grid[i][j] + '0', 0, 0, 0});
                }
            }
        }
        if(minfind > 9){
            symbols.push_back({cols * 25 + 17, 15, minfind / 10 + '0', 0, 0, 0});
            symbols.push_back({cols * 25 + 25, 15, minfind % 10 + '0', 0, 0, 0});
        }else{
            symbols.push_back({cols * 25 + 21, 15, minfind + '0', 0, 0, 0});
        }
        addSymbol(symbols);
    }

    void cleer(){
        vector<vector<int>> rect;
        addSymbol(rect);
        rect.push_back({0, 0, (cols+2) * 25 + 16, rows * 25 + 39, 255, 255, 255});
        addRectangle(rect);

    }

    void loss(){
        vector<vector<int>> rect;
        rect.push_back({0, 0, (cols+2) * 25 + 16, rows * 25 + 39, 255, 255, 255});
        addRectangle(rect);


        vector<vector<int>> symbols;
        int x{((cols+2) * 25 + 16) / 2.5}, y{(rows * 25 + 39) / 2.5};

        for (char c : "GAME OVER") {
            symbols.push_back({x, y, c, 0, 0, 0});
            x += 10;
        }
        addSymbol(symbols);
    }

    void win(){
        vector<vector<int>> rect;
        rect.push_back({0, 0, (cols+2) * 25 + 16, rows * 25 + 39, 255, 255, 255});
        addRectangle(rect);

        vector<vector<int>> symbols;
        int x{((cols+2) * 25 + 16) / 2.5}, y{(rows * 25 + 39) / 2.5};

        symbols.push_back({x, y, 'W', 0, 0, 0});
        x += 14;
        symbols.push_back({x, y, 'I', 0, 0, 0});
        x += 5;
        symbols.push_back({x, y, 'N', 0, 0, 0});


        addSymbol(symbols);
    }

    void gameMin() {
        bool isfind = false;
        for (auto& Rect : rect) {
            int rectX = Rect[0];
            int rectY = Rect[1];
            int rectWidth = Rect[2];
            int rectHeight = Rect[3];

            if (mouse.X >= rectX && mouse.X < rectX + rectWidth && mouse.Y >= rectY && mouse.Y < rectY + rectHeight) {
                mouse.X = rectX / 25;
                mouse.Y = rectY / 25;
                isfind = true;
            }

        }
        if(!isfind){
            return;
        }
        if (revealed[mouse.Y][mouse.X] == 0 && mouse.S == 1) {
            reveal(mouse.Y, mouse.X);
            if (grid[mouse.Y][mouse.X] == -1) {
                loss();
            } else {
                printGrid();
            }
        }
        else if (mouse.S == 0) {
            if(revealed[mouse.Y][mouse.X] == 0){
                --minfind;
                revealed[mouse.Y][mouse.X] = 2;
                if(grid[mouse.Y][mouse.X] == -1) {
                    --mineCount;
                    if (mineCount == 0) {
                        win();
                        return;
                    }
                }
                printGrid();
            }
            else if(revealed[mouse.Y][mouse.X] == 2){
                ++minfind;
                revealed[mouse.Y][mouse.X] = 0;
                if(grid[mouse.Y][mouse.X] == -1){
                    ++mineCount;
                }
                printGrid();
            }
    }
}

    void Minesweeper_init(int rows_, int cols_, int mineCount_)
{
    rowsm = rows_;
    colsm = cols_;
    mineCountm = mineCount_;
    minfind = mineCount_;

    grid.resize(rows, vector<int>(cols, 0));
    revealed.resize(rows, vector<int>(cols, 0));
    placeMines();
    calculateNeighborMines();
    printGrid();
}

public:
    game() : CustomWindow(600, 300), rows(0), cols(0), mineCount(0) { printmenu(); }

    void customFunction() override {
        if(!mode ){
            menu();
        } else{
            static bool First = true;
            if(First){
                Minesweeper_init(rows,cols,mineCount);
                First = false;
                return;
            }
            gameMin();
        }
    }



};

int main() {
    game ob;
    ob.Execute();
    return 0;
}
