#include "platform/win32/Application-win32.h"

#if TARGET_PLATFORM == PLATFORM_WIN32

#include "base/Director.h"

NS_BEGIN

Application* Application::sm_pSharedApplication = 0;

Application::Application()
: _instance(nullptr)
{
    _instance = GetModuleHandle(nullptr);
    _animationInterval.QuadPart = 0;
    ASSERT(!sm_pSharedApplication);
    sm_pSharedApplication = this;
}

Application::~Application()
{
    ASSERT(sm_pSharedApplication != nullptr);
    sm_pSharedApplication = nullptr;
}

int Application::run()
{
    // Main message loop:
    LARGE_INTEGER nLast;
    LARGE_INTEGER nNow;

    QueryPerformanceCounter(&nLast);

    initGLContextAttrs();

    // Initialize instance and simple2d.
    if (!applicationDidFinishLaunching())
    {
        return 1;
    }

    auto director = Director::getInstance();

    auto glview = director->getOpenGLView();

    glview->retain();

    glview->swapBuffers();

    while (!glview->windowShouldClose())
    {
        QueryPerformanceCounter(&nNow);

        if (nNow.QuadPart - nLast.QuadPart > _animationInterval.QuadPart)
        {
            nLast.QuadPart = nNow.QuadPart - (nNow.QuadPart % _animationInterval.QuadPart);

             director->mainLoop();
             glview->pollEvents();
        }
        else
        {
            Sleep(1);
        }
    }

    if (glview->isOpenGLReady())
    {
       director->end();
       director->mainLoop();
       director = nullptr; 
    }

    glview->release();
    return 0;
}

void Application::setAnimationInterval(double interval)
{
    LARGE_INTEGER nFreq;
    QueryPerformanceFrequency(&nFreq);
    _animationInterval.QuadPart = (LONGLONG)(interval * nFreq.QuadPart);
}

//---------------------------------------------------------------
//
// static function
//
//---------------------------------------------------------------

Application* Application::getInstance()
{
    ASSERT(sm_pSharedApplication);
    return sm_pSharedApplication;
}

LanguageType Application::getCurrentLanguage()
{
    LanguageType ret = LanguageType::ENGLISH;

    LCID localeID = GetUserDefaultLCID();
    unsigned short primaryLanguageID = localeID & 0xFF;

    switch (primaryLanguageID)
    {
    case LANG_CHINESE:
        ret = LanguageType::CHINESE;
        break;
    case LANG_ENGLISH:
        ret = LanguageType::ENGLISH;
        break;
    case LANG_FRENCH:
        ret = LanguageType::FRENCH;
        break;
    case LANG_ITALIAN:
        ret = LanguageType::ITALIAN;
        break;
    case LANG_GERMAN:
        ret = LanguageType::GERMAN;
        break;
    case LANG_SPANISH:
        ret = LanguageType::SPANISH;
        break;
    case LANG_DUTCH:
        ret = LanguageType::DUTCH;
        break;
    case LANG_RUSSIAN:
        ret = LanguageType::RUSSIAN;
        break;
    case LANG_KOREAN:
        ret = LanguageType::KOREAN;
        break;
    case LANG_JAPANESE:
        ret = LanguageType::JAPANESE;
        break;
    case LANG_HUNGARIAN:
        ret = LanguageType::HUNGARIAN;
        break;
    case LANG_PORTUGUESE:
        ret = LanguageType::PORTUGUESE;
        break;
    case LANG_ARABIC:
        ret = LanguageType::ARABIC;
        break;
    case LANG_NORWEGIAN:
        ret = LanguageType::NORWEGIAN;
        break;
    case LANG_POLISH:
        ret = LanguageType::POLISH;
        break;
    case LANG_TURKISH:
        ret = LanguageType::TURKISH;
        break;
    case LANG_UKRAINIAN:
        ret = LanguageType::UKRAINIAN;
        break;
    case LANG_ROMANIAN:
        ret = LanguageType::ROMANIAN;
        break;
    case LANG_BULGARIAN:
        ret = LanguageType::BULGARIAN;
        break;
    }
    return ret;
}

const char* Application::getCurrentLanguageCode()
{
    LANGID lid = GetUserDefaultUILanguage();
    const LCID locale_id = MAKELCID(lid, SORT_DEFAULT);
    static char code[3] = { 0 };
    GetLocaleInfoA(locale_id, LOCALE_SISO639LANGNAME, code, sizeof(code));
    code[2] = '\0';
    return code;
}

Application::Platform Application::getTargetPlatform()
{
    return Platform::OS_WINDOWS;
}

bool Application::openURL(const std::string &url)
{
    WCHAR *temp = new WCHAR[url.size() + 1];
    int wchars_num = MultiByteToWideChar(CP_UTF8, 0, url.c_str(), url.size() + 1, temp, url.size() + 1);
    HINSTANCE r = ShellExecuteW(NULL, L"open", temp, NULL, NULL, SW_SHOWNORMAL);
    delete[] temp;
    return (size_t)r > 32;
}

NS_END

#endif