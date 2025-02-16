#include <windows.h>
#include <cmath>
#include <iostream>
class EventHandler {
public:
    virtual ~EventHandler() = default;                 // Virtual destructor to prevent memory issues
    virtual void onMouseMove(int x, int y) = 0;        // Handle mouse movement event
    virtual void onLeftClick() = 0;                    // Handle left mouse button click
    virtual void onLeftRelease() = 0;                  // Handle left mouse button release
    virtual void onRightClick() = 0;                   // Handle right mouse button click
    virtual void onRightRelease() = 0;                 // Handle right mouse button release
    virtual void onMiddleClick() = 0;                  // Handle middle mouse button click
    virtual void onMiddleRelease() = 0;                // Handle middle mouse button release
    virtual void onMouseScroll(int delta) = 0;         // Handle mouse wheel scroll
    virtual void onKeyPress(wchar_t keyCode) = 0;      // Handle key press event
    virtual void onKeyRelease(wchar_t keyCode) = 0;    // Handle key release event
    virtual void onKeyCharInput(wchar_t keyCode) = 0 ; // Handle  character input
    virtual void onAltKeyPress(wchar_t keyCode) = 0;   // Handle Alt + key press event
    virtual void onAltRelease(wchar_t charCode) = 0;   // For character input like letters and numbers
    virtual void onSysCharInput(wchar_t charCode) = 0; // Handle system character input
};



class Window : public EventHandler{
public:
    Window(const char* windowTitle, int width, int height, bool resizable) {
        // Getting the instance handle
        hInstance = GetModuleHandle(NULL);
        this->windowTitle = windowTitle;
        this->width = width;
        this->height = height;
        this->resizable = resizable;

        // Registering the window class
        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc; // Message processing function
        wc.hInstance = hInstance;    // Instance handle
        wc.lpszClassName = className; // Window class name
        RegisterClass(&wc); // Registering the class in the system

        // If the window is resizable, keep the WS_SIZEBOX style
        DWORD dwStyle = WS_OVERLAPPEDWINDOW;
        if (!resizable) {
            dwStyle &= ~WS_SIZEBOX;  // Removing WS_SIZEBOX style to disable resizing
        }

        // Creating the window using the registered class information
        hwnd = CreateWindowEx(
                0,
                className,       // Window class name
                windowTitle,     // Window title
                dwStyle,         // Using the specified style
                CW_USEDEFAULT,   // Default horizontal position
                CW_USEDEFAULT,   // Default vertical position
                width,           // Window width
                height,          // Window height
                NULL,            // Parent window (no parent in this case)
                NULL,            // No menu
                hInstance,       // Instance handle
                this             // Window object information (to access in message processing)
        );
    }

    // Show the window
    void Show(int nCmdShow) {
        ShowWindow(hwnd, nCmdShow); // Show the window
    }

    // Message processing loop
    void Run() {
        MSG msg = {}; // Message structure
        // Receive and process messages until the window is closed
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);  // Translate messages
            DispatchMessage(&msg);   // Dispatch messages to the processor
        }
    }

    // Function to resize and change the window's text
    void ResizeAndChangeText(int newWidth, int newHeight, const char* newTitle) {
        // Resize the window
        SetWindowPos(hwnd, NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE);

        // Change the window's text
        SetWindowText(hwnd, newTitle);
    }

    void DrawPixel(int x, int y, COLORREF color) {
        HDC hdc = GetDC(hwnd); // Get access to the window's graphics context
        SetPixel(hdc, x, y, color); // Light up the pixel with the specified color
        ReleaseDC(hwnd, hdc); // Release the graphics context
    }

    void DrawLine(int x1, int y1, int x2, int y2, COLORREF color) {
        HDC hdc = GetDC(hwnd); // Get access to the window's graphics context

        // Set pen color
        HPEN hPen = CreatePen(PS_SOLID, 1, color); // Create a pen with the desired color
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen); // Select the new pen

        // Set the start point and draw the line
        MoveToEx(hdc, x1, y1, NULL); // Set the start point
        LineTo(hdc, x2, y2); // Draw the line to the second point

        // Release resources
        SelectObject(hdc, hOldPen); // Restore the old pen
        DeleteObject(hPen); // Delete the new pen
        ReleaseDC(hwnd, hdc); // Release the graphics context
    }

    void DrawRectangle(int topLeftX, int topLeftY, int width, int height, COLORREF fillColor, bool filled) {
        HDC hdc = GetDC(hwnd); // Get access to the window's graphics context

        // Create a pen for the rectangle's border with the color passed in
        HPEN hPen = CreatePen(PS_SOLID, 1, fillColor);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        if (filled) {
            // If the rectangle should be filled, create a brush for filling with the specified color
            HBRUSH hBrush = CreateSolidBrush(fillColor); // Use the input color for filling
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            // Draw the rectangle and fill it
            Rectangle(hdc, topLeftX, topLeftY, topLeftX + width, topLeftY + height);

            // Release the brush resources
            SelectObject(hdc, hOldBrush);
            DeleteObject(hBrush);
        } else {
            // If the rectangle is empty, only draw the border
            // Draw the rectangle without filling it
            Rectangle(hdc, topLeftX, topLeftY, topLeftX + width, topLeftY + height);
        }

        // Release pen resources
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
        ReleaseDC(hwnd, hdc);
    }

    void DrawCircle(int centerX, int centerY, int radius, COLORREF color, bool filled) {
        HDC hdc = GetDC(hwnd); // Get access to the window's graphics context

        // Create a pen for the circle's border with the color passed in
        HPEN hPen = CreatePen(PS_SOLID, 1, color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        if (filled) {
            // If the circle should be filled, create a brush for filling with the specified color
            HBRUSH hBrush = CreateSolidBrush(color); // Use the input color for filling
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            // Draw the circle and fill it
            Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);

            // Release the brush resources
            SelectObject(hdc, hOldBrush);
            DeleteObject(hBrush);
        } else {
            // If the circle is empty, only draw the border
            Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
        }

        // Release pen resources
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
        ReleaseDC(hwnd, hdc);
    }

    void DrawArc(int centerX, int centerY, int radius, int startAngle, int endAngle, COLORREF color) {
        HDC hdc = GetDC(hwnd); // Get access to the window's graphics context

        // Create a pen for the arc's border with the color passed in
        HPEN hPen = CreatePen(PS_SOLID, 1, color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        // Calculate the coordinates of the arc's start point (based on the start angle)
        int startX = centerX + radius * cos(startAngle * 3.14159 / -180.0); // Convert degrees to radians
        int startY = centerY + radius * sin(startAngle * 3.14159 / -180.0);

        // Calculate the coordinates of the arc's end point (based on the end angle)
        int endX = centerX + radius * cos(endAngle * 3.14159 / -180.0);
        int endY = centerY + radius * sin(endAngle * 3.14159 / -180.0);

        // Draw the arc (a portion of the circle)
        Arc(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius,
            startX, startY, endX, endY);

        // Release resources
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
        ReleaseDC(hwnd, hdc);
    }

    void ClearScreen(COLORREF bgColor) {
        HDC hdc = GetDC(hwnd); // Get access to the window's graphics context

        // Create a brush for the background
        HBRUSH hBrush = CreateSolidBrush(bgColor); // Use the desired background color
        RECT rect;
        GetClientRect(hwnd, &rect); // Get the window dimensions

        // Fill the screen with the background color
        FillRect(hdc, &rect, hBrush);

        // Release resources
        DeleteObject(hBrush);
        ReleaseDC(hwnd, hdc);
    }

    void Drawtext(int x, int y, const char* text, COLORREF textColor, int fontSize) {
        HDC hdc = GetDC(hwnd); // Get access to the window's graphics context

        // Set the pen color for the text
        SetTextColor(hdc, textColor);

        // Set the font with the desired size
        HFONT hFont = CreateFont(
                fontSize,          // Font size
                0,                 // Font width (0 means use the default width)
                0,                 // Font angle
                0,                 // Font angle
                FW_NORMAL,         // Font weight
                FALSE,             // Italic font
                FALSE,             // Underline font
                FALSE,             // Strikeout font
                ANSI_CHARSET,      // Character set
                OUT_DEFAULT_PRECIS, // Output precision
                CLIP_DEFAULT_PRECIS,// Clipping precision
                DEFAULT_QUALITY,   // Default quality
                DEFAULT_PITCH,     // Default pitch
                "Arial"            // Font name
        );
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

        // Set the background mode to transparent
        SetBkMode(hdc, TRANSPARENT);

        // Output the text at the specified coordinates
        TextOut(hdc, x, y, text, strlen(text));

        // Release resources
        SelectObject(hdc, hOldFont); // Restore the old font
        DeleteObject(hFont);         // Delete the new font
        ReleaseDC(hwnd, hdc);        // Release the graphics context
    }

    void onMouseMove(int x, int y) override {
        ClearScreen(RGB(255,255,255));
        std::string message = "Mouse moved to (" + std::to_string(x) + ", " + std::to_string(y) + ")";
        Drawtext(10, 10, message.c_str(), RGB(0, 0, 0), 20);
    }

    void onLeftClick() override {
        ClearScreen(RGB(255,255,255));
        std::string message = "Left mouse button clicked.";
        Drawtext(10, 30, message.c_str(), RGB(0, 0, 0), 20);
    }

    void onLeftRelease() override {
        ClearScreen(RGB(255,255,255));
        std::string message = "Left mouse button released.";
        Drawtext(10, 50, message.c_str(), RGB(0, 0, 0), 20);
    }

    void onRightClick() override {
        ClearScreen(RGB(255,255,255));
        std::string message = "Right mouse button clicked.";
        Drawtext(10, 70, message.c_str(), RGB(0, 0, 0), 20);
    }

    void onRightRelease() override {
        ClearScreen(RGB(255,255,255));
        std::string message = "Right mouse button released.";
        Drawtext(10, 90, message.c_str(), RGB(0, 0, 0), 20);
    }

    void onMiddleClick() override {
        ClearScreen(RGB(255,255,255));
        std::string message = "Middle mouse button clicked.";
        Drawtext(10, 110, message.c_str(), RGB(0, 0, 0), 20);
    }

    void onMiddleRelease() override {
        ClearScreen(RGB(255,255,255));
        std::string message = "Middle mouse button released.";
        Drawtext(10, 130, message.c_str(), RGB(0, 0, 0), 20);
    }

    void onMouseScroll(int delta) override {
        ClearScreen(RGB(255,255,255));
        std::string message = "Mouse wheel scrolled with delta: " + std::to_string(delta);
        Drawtext(10, 150, message.c_str(), RGB(0, 0, 0), 20);
    }

    void onKeyPress(wchar_t keyCode) override {
        ClearScreen(RGB(255,255,255));
        std::wstring message = L"Key pressed: " + std::wstring(1, keyCode);
        Drawtext(10, 170, std::string(message.begin(), message.end()).c_str(), RGB(0, 0, 0), 20);
    }

    void onKeyRelease(wchar_t keyCode) override {
        ClearScreen(RGB(255,255,255));
        std::wstring message = L"Key released: " + std::wstring(1, keyCode);
        Drawtext(10, 190, std::string(message.begin(), message.end()).c_str(), RGB(0, 0, 0), 20);
    }

    void onKeyCharInput(wchar_t keyCode) override {
        ClearScreen(RGB(255,255,255));
        std::wstring message = L"Character input: " + std::wstring(1, keyCode);
        Drawtext(10, 210, std::string(message.begin(), message.end()).c_str(), RGB(0, 0, 0), 20);
    }

    void onAltKeyPress(wchar_t keyCode) override {
        ClearScreen(RGB(255,255,255));
        std::wstring message = L"Alt key pressed with key: " + std::wstring(1, keyCode);
        Drawtext(10, 230, std::string(message.begin(), message.end()).c_str(), RGB(0, 0, 0), 20);
    }

    void onAltRelease(wchar_t charCode) override {
        ClearScreen(RGB(255,255,255));
        std::wstring message = L"Alt key released with char: " + std::wstring(1, charCode);
        Drawtext(10, 250, std::string(message.begin(), message.end()).c_str(), RGB(0, 0, 0), 20);
    }

    void onSysCharInput(wchar_t charCode) override {
        ClearScreen(RGB(255,255,255));
        std::wstring message = L"System character input: " + std::wstring(1, charCode);
        Drawtext(10, 270, std::string(message.begin(), message.end()).c_str(), RGB(0, 0, 0), 20);
    }

private:
    const char* className = "WindowClass"; // Window class name
    const char* windowTitle;               // Window title
    int width, height;                     // Window dimensions
    bool resizable;                        // Whether the window is resizable
    HINSTANCE hInstance;                   // Instance handle
    HWND hwnd;                             // Window handle


    // Window message processing function
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        Window* pThis;
        if (uMsg == WM_NCCREATE) {
            pThis = (Window*)((CREATESTRUCT*)lParam)->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        } else {
            pThis = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        // If the window object is initialized, process the messages
        if (pThis) {
            switch (uMsg) {
                case WM_DESTROY:
                    PostQuitMessage(0);
                    return 0;

                // Handle mouse movement event
                case WM_MOUSEMOVE: {
                    pThis->onMouseMove(LOWORD(lParam), HIWORD(lParam));
                    break;
                }

                // Handle left mouse button click
                case WM_LBUTTONDOWN: {
                    pThis->onLeftClick();
                    break;
                }

                // Handle left mouse button release
                case WM_LBUTTONUP: {
                    pThis->onLeftRelease();
                    break;
                }

                // Handle right mouse button click
                case WM_RBUTTONDOWN: {
                    pThis->onRightClick();
                    break;
                }

                // Handle right mouse button release
                case WM_RBUTTONUP: {
                    pThis->onRightRelease();
                    break;
                }

                // Handle middle mouse button click
                case WM_MBUTTONDOWN: {
                    pThis->onMiddleClick();
                    break;
                }

                // Handle middle mouse button release
                case WM_MBUTTONUP: {
                    pThis->onMiddleRelease();
                    break;
                }

                // Handle mouse wheel scroll
                case WM_MOUSEWHEEL: {
                    pThis->onMouseScroll(GET_WHEEL_DELTA_WPARAM(wParam));
                    break;
                }

                // Handle key press event
                case WM_KEYDOWN:
                {
                    pThis->onKeyPress(wParam);
                    break;
                }

                // Handle key release event
                case WM_KEYUP:
                {
                    pThis->onKeyRelease(wParam);
                    break;
                }

                // Handle character input
                case WM_CHAR:
                {
                    pThis->onKeyCharInput((wchar_t)wParam);
                    break;
                }

                // Handle system key press (e.g., Alt key)
                case WM_SYSKEYDOWN:
                {
                    pThis->onAltKeyPress((wchar_t)wParam);
                    break;
                }

                // Handle system key release
                case WM_SYSKEYUP:
                {
                    pThis->onAltRelease((wchar_t)wParam);
                    break;
                }

                // Handle system character input
                case WM_SYSCHAR:
                {
                    pThis->onSysCharInput((wchar_t)wParam);
                    break;
                }


            }
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
};

int main() {
    Window window("Simple Window", 800, 600, false); // Create a window instance
    window.Show(SW_SHOW); // Show the window
    window.Run(); // Start the message loop
    return 0;
}
