#include "platform/win32/Application-win32.h"

#if TARGET_PLATFORM == PLATFORM_WIN32

NS_BEGIN

Application* Application::sm_pSharedApplication = 0;

Application::Application()
: _instance(nullptr)
{
    _instance    = GetModuleHandle(nullptr);
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
    initGLContextAttrs();

    if (!applicationDidFinishLaunching())
    {
        return 1;
    }

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

    return nullptr;
}

Application::Platform Application::getTargetPlatform()
{
    return Platform::OS_WINDOWS;
}

bool Application::openURL(const std::string &url)
{

    return false;
}

NS_END

#endif