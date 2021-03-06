/* Copyright 2012-2014 Neko. */

/* WIN32 provides win32 platform input and output interface. */

#include "../../device/device.h"

#include "win32con.h"
#include "win32app.h"
#include "win32.h"

static UCHAR CodeMap[][8]= {
    /* {SINGLE��ASCII��+SHIFT��ASCII��+CTRL��ASCII��+ALT��ASCII} */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x1B, 0x01, 0x1B, 0x01, 0x1B, 0x01, 0x00},
    {0x02, 0x31, 0x02, 0x21, 0x00, 0x00, 0x78, 0x00},
    {0x03, 0x32, 0x03, 0x40, 0x03, 0x00, 0x79, 0x00},
    {0x04, 0x33, 0x04, 0x23, 0x00, 0x00, 0x7A, 0x00},
    {0x05, 0x34, 0x05, 0x24, 0x00, 0x00, 0x7B, 0x00},
    {0x06, 0x35, 0x06, 0x25, 0x00, 0x00, 0x7C, 0x00},
    {0x07, 0x36, 0x07, 0x5E, 0x07, 0x1E, 0x7D, 0x00},
    {0x08, 0x37, 0x08, 0x26, 0x00, 0x00, 0x7E, 0x00},
    {0x09, 0x38, 0x09, 0x2A, 0x00, 0x00, 0x7F, 0x00},
    {0x0A, 0x39, 0x0A, 0x28, 0x00, 0x00, 0x80, 0x00},
    {0x0B, 0x30, 0x0B, 0x29, 0x00, 0x00, 0x81, 0x00},
    {0x0C, 0x2D, 0x0C, 0x5F, 0x0C, 0x1F, 0x82, 0x00},
    {0x0D, 0x3D, 0x0D, 0x2B, 0x00, 0x00, 0x83, 0x00},
    {0x0E, 0x08, 0x0E, 0x08, 0x0E, 0x7F, 0x0E, 0x00},
    {0x0F, 0x09, 0x0F, 0x00, 0x94, 0x00, 0xA5, 0x00},
    {0x10, 0x71, 0x10, 0x51, 0x10, 0x11, 0x10, 0x00},
    {0x11, 0x77, 0x11, 0x57, 0x11, 0x17, 0x11, 0x00},
    {0x12, 0x65, 0x12, 0x45, 0x12, 0x05, 0x12, 0x00},
    {0x13, 0x72, 0x13, 0x52, 0x13, 0x12, 0x13, 0x00},
    {0x14, 0x74, 0x14, 0x54, 0x14, 0x14, 0x14, 0x00},
    {0x15, 0x79, 0x15, 0x59, 0x15, 0x19, 0x15, 0x00},
    {0x16, 0x75, 0x16, 0x55, 0x16, 0x15, 0x16, 0x00},
    {0x17, 0x69, 0x17, 0x49, 0x17, 0x09, 0x17, 0x00},
    {0x18, 0x6F, 0x18, 0x4F, 0x18, 0x0F, 0x18, 0x00},
    {0x19, 0x70, 0x19, 0x50, 0x19, 0x10, 0x19, 0x00},
    {0x1A, 0x5B, 0x1A, 0x7B, 0x1A, 0x1B, 0x1A, 0x00},
    {0x1B, 0x5D, 0x1B, 0x7D, 0x1B, 0x1D, 0x1B, 0x00},
    {0x1C, 0x0D, 0x1C, 0x0D, 0x1C, 0x0A, 0x1C, 0x00},
    {0x1D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x1E, 0x61, 0x1E, 0x41, 0x1E, 0x01, 0x1E, 0x00},
    {0x1F, 0x73, 0x1F, 0x53, 0x1F, 0x13, 0x1F, 0x00},
    {0x20, 0x64, 0x20, 0x44, 0x20, 0x04, 0x20, 0x00},
    {0x21, 0x66, 0x21, 0x46, 0x21, 0x06, 0x21, 0x00},
    {0x22, 0x67, 0x22, 0x47, 0x22, 0x07, 0x22, 0x00},
    {0x23, 0x68, 0x23, 0x48, 0x23, 0x08, 0x23, 0x00},
    {0x24, 0x6A, 0x24, 0x4A, 0x24, 0x0A, 0x24, 0x00},
    {0x25, 0x6B, 0x25, 0x4B, 0x25, 0x0B, 0x25, 0x00},
    {0x26, 0x6C, 0x26, 0x4C, 0x26, 0x0C, 0x26, 0x00},
    {0x27, 0x3B, 0x27, 0x3A, 0x00, 0x00, 0x27, 0x00},
    {0x28, 0x27, 0x28, 0x22, 0x00, 0x00, 0x28, 0x00},
    {0x29, 0x60, 0x29, 0x7E, 0x00, 0x00, 0x29, 0x00},
    {0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x2b, 0x5C, 0x2b, 0x7C, 0x2B, 0x1C, 0x2b, 0x00},
    {0x2C, 0x7A, 0x2C, 0x5A, 0x2C, 0x1A, 0x2C, 0x00},
    {0x2D, 0x78, 0x2D, 0x58, 0x2D, 0x18, 0x2D, 0x00},
    {0x2E, 0x63, 0x2E, 0x43, 0x2E, 0x03, 0x2E, 0x00},
    {0x2F, 0x76, 0x2F, 0x56, 0x2F, 0x16, 0x2F, 0x00},
    {0x30, 0x62, 0x30, 0x42, 0x30, 0x02, 0x30, 0x00},
    {0x31, 0x6E, 0x31, 0x4E, 0x31, 0x0E, 0x31, 0x00},
    {0x32, 0x6D, 0x32, 0x4D, 0x32, 0x0D, 0x32, 0x00},
    {0x33, 0x2C, 0x33, 0x3C, 0x00, 0x00, 0x33, 0x00},
    {0x34, 0x2E, 0x34, 0x3E, 0x00, 0x00, 0x34, 0x00},
    {0x35, 0x2F, 0x35, 0x3F, 0x00, 0x00, 0x35, 0x00},
    {0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x37, 0x2A, 0x37, 0x2A, 0x96, 0x00, 0x37, 0x00},
    {0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x39, 0x20, 0x39, 0x20, 0x39, 0x20, 0x39, 0x20},
    {0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x3B, 0x00, 0x54, 0x00, 0x5E, 0x00, 0x68, 0x00},
    {0x3C, 0x00, 0x55, 0x00, 0x5F, 0x00, 0x69, 0x00},
    {0x3D, 0x00, 0x56, 0x00, 0x60, 0x00, 0x6A, 0x00},
    {0x3E, 0x00, 0x57, 0x00, 0x61, 0x00, 0x6B, 0x00},
    {0x3F, 0x00, 0x58, 0x00, 0x62, 0x00, 0x6C, 0x00},
    {0x40, 0x00, 0x59, 0x00, 0x63, 0x00, 0x6D, 0x00},
    {0x41, 0x00, 0x5A, 0x00, 0x64, 0x00, 0x6E, 0x00},
    {0x42, 0x00, 0x5B, 0x00, 0x65, 0x00, 0x6F, 0x00},
    {0x43, 0x00, 0x5C, 0x00, 0x66, 0x00, 0x70, 0x00},
    {0x44, 0x00, 0x5D, 0x00, 0x67, 0x00, 0x71, 0x00},
    {0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x47, 0x00, 0x47, 0x37, 0x77, 0x00, 0x00, 0x07},
    {0x48, 0x00, 0x48, 0x38, 0x8D, 0x00, 0x00, 0x08},
    {0x49, 0x00, 0x49, 0x39, 0x84, 0x00, 0x00, 0x09},
    {0x4A, 0x2D, 0x4A, 0x2D, 0x8E, 0x00, 0x4A, 0x00},
    {0x4B, 0x00, 0x4B, 0x34, 0x73, 0x00, 0x00, 0x04},
    {0x4C, 0x00, 0x4C, 0x35, 0x8F, 0x00, 0x00, 0x05},
    {0x4D, 0x00, 0x4D, 0x36, 0x74, 0x00, 0x00, 0x06},
    {0x4E, 0x2B, 0x4E, 0x2B, 0x90, 0x00, 0x4E, 0x00},
    {0x4F, 0x00, 0x4F, 0x31, 0x75, 0x00, 0x00, 0x01},
    {0x50, 0x00, 0x50, 0x32, 0x91, 0x00, 0x00, 0x02},
    {0x51, 0x00, 0x51, 0x33, 0x76, 0x00, 0x00, 0x03},
    {0x52, 0x00, 0x52, 0x30, 0x92, 0x00, 0x00, 0x00},
    {0x53, 0x00, 0x53, 0x2E, 0x93, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x57, 0x00, 0x00, 0x00, 0x57, 0x00, 0x00, 0x00},
    {0x58, 0x00, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00}
};

static UCHAR MoveKeyCode[][8] = {
    {0x47, 0x00, 0x47, 0xE0, 0x77, 0xE0, 0x97, 0x00},
    {0x48, 0x00, 0x48, 0xE0, 0x8D, 0xE0, 0x98, 0x00},
    {0x49, 0x00, 0x49, 0xE0, 0x84, 0xE0, 0x99, 0x00},
    {0x4A, 0x00, 0x4A, 0xE0, 0x00, 0xE0, 0x9A, 0x00},
    {0x4B, 0x00, 0x4B, 0xE0, 0x73, 0xE0, 0x9B, 0x00},
    {0x4C, 0x00, 0x4C, 0xE0, 0x00, 0xE0, 0x9C, 0x00},
    {0x4D, 0x00, 0x4D, 0xE0, 0x74, 0xE0, 0x9D, 0x00},
    {0x4E, 0x00, 0x4E, 0xE0, 0x00, 0xE0, 0x9E, 0x00},
    {0x4F, 0x00, 0x4F, 0xE0, 0x75, 0xE0, 0x9F, 0x00},
    {0x50, 0x00, 0x50, 0xE0, 0x91, 0xE0, 0xA0, 0x00},
    {0x51, 0x00, 0x51, 0xE0, 0x76, 0xE0, 0xA1, 0x00},
    {0x52, 0x00, 0x52, 0xE0, 0x92, 0xE0, 0xA2, 0x00},
    {0x53, 0x00, 0x53, 0xE0, 0x93, 0xE0, 0xA3, 0x00}
};

VOID win32KeyboardMakeStatus() {
    if (GetAsyncKeyState(VK_RSHIFT) & 0x8000) {
        deviceConnectKeyboardSetFlag0RightShift();
    } else {
        deviceConnectKeyboardClrFlag0RightShift();
    }
    if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) {
        deviceConnectKeyboardSetFlag0LeftShift();
    } else {
        deviceConnectKeyboardClrFlag0LeftShift();
    }
    if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
        deviceConnectKeyboardSetFlag0Ctrl();
    } else {
        deviceConnectKeyboardClrFlag0Ctrl();
    }
    if (GetAsyncKeyState(VK_MENU) & 0x8000) {
        deviceConnectKeyboardSetFlag0Alt();
    } else {
        deviceConnectKeyboardClrFlag0Alt();
    }
    if (GetAsyncKeyState(VK_SCROLL) & 0x8000) {
        deviceConnectKeyboardSetFlag1ScrLck();
    } else {
        deviceConnectKeyboardClrFlag1ScrLck();
    }
    if (GetAsyncKeyState(VK_NUMLOCK) & 0x8000) {
        deviceConnectKeyboardSetFlag1NumLck();
    } else {
        deviceConnectKeyboardClrFlag1NumLck();
    }
    if (GetAsyncKeyState(VK_CAPITAL) & 0x8000) {
        deviceConnectKeyboardSetFlag1CapLck();
    } else {
        deviceConnectKeyboardClrFlag1CapLck();
    }
    if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
        deviceConnectKeyboardSetFlag1Insert();
    } else {
        deviceConnectKeyboardClrFlag1Insert();
    }

    if (GetKeyState(VK_SCROLL) & 0x0001) {
        deviceConnectKeyboardSetFlag0ScrLck();
    } else {
        deviceConnectKeyboardClrFlag0ScrLck();
    }
    if (GetKeyState(VK_NUMLOCK) & 0x0001) {
        deviceConnectKeyboardSetFlag0NumLck();
    } else {
        deviceConnectKeyboardClrFlag0NumLck();
    }
    if (GetKeyState(VK_CAPITAL) & 0x0001) {
        deviceConnectKeyboardSetFlag0CapLck();
    } else {
        deviceConnectKeyboardClrFlag0CapLck();
    }
    if (GetKeyState(VK_INSERT) & 0x0001) {
        deviceConnectKeyboardSetFlag0Insert();
    } else {
        deviceConnectKeyboardClrFlag0Insert();
    }
    if (GetKeyState(VK_PAUSE) & 0x0001) {
        deviceConnectKeyboardSetFlag1Pause();
    } else {
        deviceConnectKeyboardClrFlag1Pause();
    }
}

VOID win32KeyboardMakeKey(UCHAR scanCode, UCHAR virtualKey) {
    UCHAR ascii = 0x00;
    USHORT code = 0x0000;
    switch (virtualKey) {
    case VK_CAPITAL:
    case VK_NUMLOCK:
    case VK_SCROLL:
    case VK_INSERT:
    case VK_SHIFT:
    case VK_MENU:
    case VK_CONTROL:
    case VK_PAUSE:
        win32KeyboardMakeStatus();
        code = ((USHORT) scanCode << 8);
        break;
    case VK_UP:
    case VK_DOWN:
    case VK_LEFT:
    case VK_RIGHT:
    case VK_HOME:
    case VK_DELETE:
    case VK_END:
    case VK_PRIOR:
    case VK_NEXT:
        if (deviceConnectKeyboardGetFlag0Alt()) {
            code = MoveKeyCode[scanCode - 0x47][7];
        } else if (deviceConnectKeyboardGetFlag0Ctrl()) {
            code = MoveKeyCode[scanCode - 0x47][5];
        } else if (deviceConnectKeyboardGetFlag0Shift()) {
            code = MoveKeyCode[scanCode - 0x47][3];
        } else {
            code = MoveKeyCode[scanCode - 0x47][1];
        }
        code &= 0x00ff;
        code |= ((USHORT)scanCode << 8);
        break;
    case VK_F9:
        deviceStop();
    default:
        if (deviceConnectKeyboardGetFlag0Alt()) {
            code = CodeMap[scanCode][7];
        } else if (deviceConnectKeyboardGetFlag0Ctrl()) {
            code = CodeMap[scanCode][5];
        } else if (deviceConnectKeyboardGetFlag0Shift()) {
            code = CodeMap[scanCode][3];
        } else {
            if (virtualKey > 0x40 && virtualKey < 0x5b) {
                /* Caps Lock Active */
                if (deviceConnectKeyboardGetFlag0CapsLock() ==
                        deviceConnectKeyboardGetFlag0Shift())
                    code = CodeMap[scanCode][1];
                else
                    code = CodeMap[scanCode][3];
            } else if (scanCode > 0x46 && scanCode < 0x54) {
                /* Num Lock Active */
                if (virtualKey < 0x60)
                    /* Arrow Keys */
                    code = 0x00;
                else {
                    if (deviceConnectKeyboardGetFlag0NumLock() &&
                            deviceConnectKeyboardGetFlag0Shift())
                        code = CodeMap[scanCode][3];
                    else
                        code = CodeMap[scanCode][1];
                }
            } else {
                if (deviceConnectKeyboardGetFlag0Shift())
                    code = CodeMap[scanCode][3];
                else
                    code = CodeMap[scanCode][1];
            }
        }
        /* correct scanCode */
        if (deviceConnectKeyboardGetFlag0Alt()) {
            scanCode = CodeMap[scanCode][6];
        } else if (deviceConnectKeyboardGetFlag0Ctrl()) {
            scanCode = CodeMap[scanCode][4];
        } else if (deviceConnectKeyboardGetFlag0Shift()) {
            scanCode = CodeMap[scanCode][2];
        } else {
            scanCode = CodeMap[scanCode][0];
        }
        code &= 0x00ff;
        code |= ((USHORT) scanCode << 8);
        break;
    }
    deviceConnectKeyboardRecvKeyPress(code);
}

VOID win32DisplaySetScreen(BOOL flagWindow) {
    if (flagWindow) {
        win32appDisplaySetScreen();
    } else {
        win32conDisplaySetScreen();
    }
}

VOID win32DisplayPaint(BOOL flagWindow) {
    if (flagWindow) {
        win32appDisplayPaint();
    } else {
        win32conDisplayPaint();
    }
}

VOID win32StartMachine(BOOL flagWindow) {
    if (flagWindow) {
        win32appStartMachine();
    } else {
        win32conStartMachine();
    }
}
