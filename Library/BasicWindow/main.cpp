#include <windows.h>
#include <cmath>
#include <iostream>

class Window {
public:
    Window(const char* windowTitle, int width, int height, bool resizable) {
        hInstance = GetModuleHandle(NULL);
        this->windowTitle = windowTitle;
        this->width = width;
        this->height = height;
        this->resizable = resizable;

        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = className;
        RegisterClass(&wc);

        DWORD dwStyle = WS_OVERLAPPEDWINDOW;
        if (!resizable) {
            dwStyle &= ~WS_SIZEBOX;
        }

        hwnd = CreateWindowEx(
                0,
                className,
                windowTitle,
                dwStyle,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                width,
                height,
                NULL,
                NULL,
                hInstance,
                this
        );
    }

    void Show(int nCmdShow) {
        ShowWindow(hwnd, nCmdShow);
    }

    void Run() {
        MSG msg = {};
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void ResizeAndChangeText(int newWidth, int newHeight, const char* newTitle) {
        SetWindowPos(hwnd, NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE);
        SetWindowText(hwnd, newTitle);
    }

    void DrawPixel(int x, int y, COLORREF color) {
        HDC hdc = GetDC(hwnd);
        SetPixel(hdc, x, y, color);
        ReleaseDC(hwnd, hdc);
    }

    void DrawLine(int x1, int y1, int x2, int y2, COLORREF color) {
        HDC hdc = GetDC(hwnd);
        HPEN hPen = CreatePen(PS_SOLID, 1, color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, x1, y1, NULL);
        LineTo(hdc, x2, y2);
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
        ReleaseDC(hwnd, hdc);
    }

    void DrawRectangle(int topLeftX, int topLeftY, int width, int height, COLORREF fillColor, bool filled) {
        HDC hdc = GetDC(hwnd);
        HPEN hPen = CreatePen(PS_SOLID, 1, fillColor);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        if (filled) {
            HBRUSH hBrush = CreateSolidBrush(fillColor);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
            Rectangle(hdc, topLeftX, topLeftY, topLeftX + width, topLeftY + height);
            SelectObject(hdc, hOldBrush);
            DeleteObject(hBrush);
        } else {
            Rectangle(hdc, topLeftX, topLeftY, topLeftX + width, topLeftY + height);
        }

        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
        ReleaseDC(hwnd, hdc);
    }

    void DrawCircle(int centerX, int centerY, int radius, COLORREF color, bool filled) {
        HDC hdc = GetDC(hwnd);
        HPEN hPen = CreatePen(PS_SOLID, 1, color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        if (filled) {
            HBRUSH hBrush = CreateSolidBrush(color);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
            Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
            SelectObject(hdc, hOldBrush);
            DeleteObject(hBrush);
        } else {
            Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
        }

        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
        ReleaseDC(hwnd, hdc);
    }

    void DrawArc(int centerX, int centerY, int radius, int startAngle, int endAngle, COLORREF color) {
        HDC hdc = GetDC(hwnd);
        HPEN hPen = CreatePen(PS_SOLID, 1, color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        int startX = centerX + radius * cos(startAngle * 3.14159 / -180.0);
        int startY = centerY + radius * sin(startAngle * 3.14159 / -180.0);
        int endX = centerX + radius * cos(endAngle * 3.14159 / -180.0);
        int endY = centerY + radius * sin(endAngle * 3.14159 / -180.0);

        Arc(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius,
            startX, startY, endX, endY);

        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
        ReleaseDC(hwnd, hdc);
    }

    void ClearScreen(COLORREF bgColor) {
        HDC hdc = GetDC(hwnd);
        HBRUSH hBrush = CreateSolidBrush(bgColor);
        RECT rect;
        GetClientRect(hwnd, &rect);
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);
        ReleaseDC(hwnd, hdc);
    }

    void Drawtext(int x, int y, const char* text, COLORREF textColor, int fontSize) {
        HDC hdc = GetDC(hwnd);
        SetTextColor(hdc, textColor);

        HFONT hFont = CreateFont(
                fontSize,
                0,
                0,
                0,
                FW_NORMAL,
                FALSE,
                FALSE,
                FALSE,
                ANSI_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                DEFAULT_PITCH,
                "Arial"
        );
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
        SetBkMode(hdc, TRANSPARENT);
        TextOut(hdc, x, y, text, strlen(text));
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
        ReleaseDC(hwnd, hdc);
    }

private:
    const char* className = "WindowClass";
    const char* windowTitle;
    int width, height;
    bool resizable;
    HINSTANCE hInstance;
    HWND hwnd;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        Window* pThis = nullptr;

        if (uMsg == WM_CREATE) {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (Window*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        } else {
            pThis = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        if (pThis) {
            switch (uMsg) {
                case WM_DESTROY:
                    PostQuitMessage(0);
                    return 0;

                case WM_MOUSEMOVE: {
                    int x = LOWORD(lParam);
                    int y = HIWORD(lParam);
                    std::cout << "Mouse moved to (" << x << ", " << y << ")\n";
                    break;
                }

                case WM_LBUTTONDOWN: {
                    int x = LOWORD(lParam);
                    int y = HIWORD(lParam);
                    std::cout << "Left mouse button clicked at (" << x << ", " << y << ")\n";
                    break;
                }

                case WM_LBUTTONUP: {
                    int x = LOWORD(lParam);
                    int y = HIWORD(lParam);
                    std::cout << "Left mouse button released at (" << x << ", " << y << ")\n";
                    break;
                }

                case WM_RBUTTONDOWN: {
                    int x = LOWORD(lParam);
                    int y = HIWORD(lParam);
                    std::cout << "Right mouse button clicked at (" << x << ", " << y << ")\n";
                    break;
                }

                case WM_RBUTTONUP: {
                    int x = LOWORD(lParam);
                    int y = HIWORD(lParam);
                    std::cout << "Right mouse button released at (" << x << ", " << y << ")\n";
                    break;
                }

                case WM_MBUTTONDOWN: {
                    int x = LOWORD(lParam);
                    int y = HIWORD(lParam);
                    std::cout << "Middle mouse button clicked at (" << x << ", " << y << ")\n";
                    break;
                }

                case WM_MBUTTONUP: {
                    int x = LOWORD(lParam);
                    int y = HIWORD(lParam);
                    std::cout << "Middle mouse button released at (" << x << ", " << y << ")\n";
                    break;
                }

                case WM_MOUSEWHEEL: {
                    static int zDelta = 0;
                    zDelta += GET_WHEEL_DELTA_WPARAM(wParam);
                    if (zDelta > 0) {
                        std::cout << "Mouse wheel scrolled up " << zDelta << "\n";
                    } else {
                        std::cout << "Mouse wheel scrolled down " << zDelta << "\n";
                    }
                    break;
                }
            }
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
};

int main() {
    Window window("mohammad", 500, 300, false);
    window.Show(SW_SHOWNORMAL);
    Sleep(1000);
    window.ResizeAndChangeText(800, 600, "New Title");
    window.DrawArc(200, 200, 50, 40, 90, RGB(255, 0, 0));
    Sleep(1000);
    window.ClearScreen(RGB(255, 0, 255));
    window.Run();
    return 0;
}
