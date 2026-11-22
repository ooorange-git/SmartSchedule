#ifndef SYSTEMAPI_H
#define SYSTEMAPI_H

#include<windows.h>
#include<dwmapi.h>
#include<QWidget>
#include<QFile>
#include<QTextStream>
#include<QCoreApplication>

enum WINDOWCOMPOSITIONATTRIB {
    WCA_ACCENT_POLICY=19,
};

enum ACCENT_STATE {
    ACCENT_DISABLED = 0,
    ACCENT_ENABLE_GRADIENT = 1,
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
    ACCENT_ENABLE_BLURBEHIND = 3,          // 经典毛玻璃
    ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,   // Win10 1803+ 亚克力效果
    ACCENT_ENABLE_HOSTBACKDROP = 5         // 更现代的云母效果
};

typedef struct _ACCENT_POLICY {
    ACCENT_STATE AccentState;
    DWORD        AccentFlags;
    DWORD        GradientColor;
    DWORD        AnimationId;
} ACCENT_POLICY, *PACCENT_POLICY;

typedef struct _WINDOWCOMPOSITIONATTRIBDATA {
    WINDOWCOMPOSITIONATTRIB Attrib;
    PVOID                   pvData;
    UINT                    cbData;
} WINDOWCOMPOSITIONATTRIBDATA, *PWINDOWCOMPOSITIONATTRIBDATA;

typedef BOOL (WINAPI *pfnSetWindowCompositionAttribute)(
    HWND hwnd,
    const WINDOWCOMPOSITIONATTRIBDATA* pwcad
);


namespace SystemAPI{

    inline bool enableBlurBehindWindow(QWidget *w) {
        HWND hwnd = reinterpret_cast<HWND>(w->winId());
        if (!hwnd) return false;

        DWM_BLURBEHIND blurBehind = {0,0,0,0};
        blurBehind.dwFlags = DWM_BB_ENABLE;
        blurBehind.fEnable = true;
        blurBehind.hRgnBlur = NULL;

        HRESULT hr = DwmEnableBlurBehindWindow(hwnd, &blurBehind);
        return SUCCEEDED(hr);
    };

    inline bool setWindowCompositionAttribute(QWidget *w){
        HWND hwnd = reinterpret_cast<HWND>(w->winId());
        HMODULE hUser32 = GetModuleHandleW(L"user32.dll");
        if (!hUser32) {
            return 0;
        }
        auto SetWindowCompositionAttribute = reinterpret_cast<pfnSetWindowCompositionAttribute>(GetProcAddress(hUser32,"SetWindowCompositionAttribute"));
        if(!SetWindowCompositionAttribute){
            return 0;
        }

        ACCENT_POLICY ac;
        ac.AnimationId = 0;
        ac.AccentState = ACCENT_ENABLE_ACRYLICBLURBEHIND;
        ac.AccentFlags = 0x20 | 0x40 | 0x80 | 0x100;
        ac.GradientColor = 0x20202020;

        WINDOWCOMPOSITIONATTRIBDATA wcd;
        wcd.Attrib=WCA_ACCENT_POLICY;
        wcd.cbData=sizeof(ac);
        wcd.pvData=&ac;

        return SetWindowCompositionAttribute(hwnd,&wcd);
    };

    inline bool extendFrameIntoClientArea(QWidget *w){
        HWND hwnd = reinterpret_cast<HWND>(w->winId());
        MARGINS margins = {-1,-1,-1,-1};
        HRESULT hr = DwmExtendFrameIntoClientArea(hwnd, &margins);
        return SUCCEEDED(hr);
    }

    inline bool writeFile(QString path,QString content){
        QFile file(QCoreApplication::applicationDirPath()+path);
        bool result = file.open(QIODevice::WriteOnly | QIODevice::Text);
        if(result){
            QTextStream out(&file);
            out<<content;
            out.flush();
            file.close();
        }
        return result;
    }

    //格式“/xxx/xx”
    inline QString readFile(QString path,bool *ok=nullptr){
        QFile file(QCoreApplication::applicationDirPath()+path);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream out(&file);
            QString content = out.readAll();
            file.close();
            if(ok){
                *ok = 1;
            }
            return content.trimmed();
        }else{
            if(ok){
                *ok = 0;
            }
            return QString();
        }
    }

    inline bool is_exist(QString path){
        return QFile::exists(QCoreApplication::applicationDirPath()+path);
    }
};

#endif // SYSTEMAPI_H
