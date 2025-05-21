#include <windows.h>
#include <cmath>
#include <iostream>



class Window{
public:

    struct UserInputState_ {
        // Mouse state
        bool leftMousePressed = false;
        bool rightMousePressed = false;
        bool middleMousePressed = false; // Track middle mouse button state
        int mouseX = 0;
        int mouseY = 0;
        bool mouseMoved = false; // True when the mouse position changes
        bool leftMouseReleased = false; // Track left mouse release
        bool rightMouseReleased = false; // Track right mouse release
        bool middleMouseReleased = false; // Track middle mouse release
        int mouseScrollDelta = 0; // Track mouse scroll delta (positive for up, negative for down)
        // Keyboard state
        char characterInput = ' ';
        bool keyShiftPressed = false;
        bool keyCtrlPressed = false;
        bool keySpacePressed = false;
        bool keyEscPressed = false;
        bool keyEnterPressed = false; // Enter key
        bool keyTabPressed = false;   // Tab key
        bool keyBackspacePressed = false; // Backspace key
        bool keyAltPressed = false;   // Alt key
        bool keyCapsLockPressed = false; // Caps Lock key
        int keyFPressed = 0;    // F1 to F12 key
        bool isKeyReleased = false; // Track if any key was released
    } userInputState;

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

private:
// Mouse Move
    void onMouseMove(int x, int y) {
        userInputState.mouseMoved = true;
        userInputState.mouseX = x;
        userInputState.mouseY = y;
    }

// Left Mouse Click
    void onLeftClick() {
        userInputState.leftMousePressed = true;
        userInputState.leftMouseReleased = false; // Reset release status when clicked
    }

// Left Mouse Release
    void onLeftRelease() {
        userInputState.leftMousePressed = false;
        userInputState.leftMouseReleased = true; // Mark as released
    }

// Right Mouse Click
    void onRightClick() {
        userInputState.rightMousePressed = true;
        userInputState.rightMouseReleased = false; // Reset release status when clicked
    }

// Right Mouse Release
    void onRightRelease() {
        userInputState.rightMousePressed = false;
        userInputState.rightMouseReleased = true; // Mark as released
    }

// Middle Mouse Click
    void onMiddleClick() {
        userInputState.middleMousePressed = true;
        userInputState.middleMouseReleased = false; // Reset release status when clicked

    }

// Middle Mouse Release
    void onMiddleRelease() {
        userInputState.middleMousePressed = false;
        userInputState.middleMouseReleased = true; // Mark middle mouse as released

    }

// Mouse Scroll
    void onMouseScroll(int delta) {
        // Update the mouse scroll delta
        userInputState.mouseScrollDelta += delta;
    }

    void onKeyPress(wchar_t keyCode) {
        userInputState.isKeyReleased = false; // Key is pressed
        switch (keyCode) {
            case VK_SHIFT:      userInputState.keyShiftPressed = true; break;
            case VK_CONTROL:    userInputState.keyCtrlPressed = true; break;
            case VK_SPACE:      userInputState.keySpacePressed = true; break;
            case VK_ESCAPE:     userInputState.keyEscPressed = true; break;
            case VK_RETURN:     userInputState.keyEnterPressed = true; break;
            case VK_TAB:        userInputState.keyTabPressed = true; break;
            case VK_BACK:       userInputState.keyBackspacePressed = true; break;
            case VK_MENU:       userInputState.keyAltPressed = true; break;
            case VK_CAPITAL:    userInputState.keyCapsLockPressed = true; break;
            default:
                if (keyCode >= VK_F1 && keyCode <= VK_F12) {
                    userInputState.keyFPressed = keyCode - VK_F1 + 1;  // F1 to F12 keys
                }
                break;
        }
    }

    void onKeyRelease(wchar_t keyCode) {
        userInputState.isKeyReleased = true; // Key is released
        switch (keyCode) {
            case VK_SHIFT:      userInputState.keyShiftPressed = false; break;
            case VK_CONTROL:    userInputState.keyCtrlPressed = false; break;
            case VK_SPACE:      userInputState.keySpacePressed = false; break;
            case VK_ESCAPE:     userInputState.keyEscPressed = false; break;
            case VK_RETURN:     userInputState.keyEnterPressed = false; break;
            case VK_TAB:        userInputState.keyTabPressed = false; break;
            case VK_BACK:       userInputState.keyBackspacePressed = false; break;
            case VK_MENU:       userInputState.keyAltPressed = false; break;
            case VK_CAPITAL:    userInputState.keyCapsLockPressed = false; break;
            default:
                if (keyCode >= VK_F1 && keyCode <= VK_F12) {
                    userInputState.keyFPressed = 0;  // Reset F1 to F12 keys
                }
                break;
        }
    }

    void onKeyCharInput(wchar_t keyCode) {
        userInputState.characterInput = keyCode; // Save character input
    }

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

            }
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
};

int main() {
    Window window("mamd", 800, 600, false); // Create a window instance
    window.Show(SW_SHOW); // Show the window
    window.Run(); // Start the message loop
    return 0;
}
