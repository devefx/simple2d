#include "platform/linux/Application-linux.h"

#if TARGET_PLATFORM == PLATFORM_LINUX

#include <unistd.h>
#include <sys/time.h>
#include <string>
#include "base/Director.h"

NS_BEGIN

Application* Application::sm_pSharedApplication = 0;

static long getCurrentMillSecond() {
    long lLastTime;
    struct timeval stCurrentTime;

    gettimeofday(&stCurrentTime, NULL);
    lLastTime = stCurrentTime.tv_sec * 1000 + stCurrentTime.tv_usec * 0.001; //millseconds
    return lLastTime;
}

Application::Application()
: _animationInterval(1.0f / 60.0f * 1000.0f)
{
    ASSERT(!sm_pSharedApplication);
    sm_pSharedApplication = this;
}

Application::~Application()
{
    ASSERT(this == sm_pSharedApplication);
    sm_pSharedApplication = NULL;
}

int Application::run()
{
    long lastTime = 0L;
    long curTime = 0L;

    initGLContextAttrs();
    // Initialize instance and simple2d.
    if (! applicationDidFinishLaunching())
    {
        return 0;
    }

    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    glview->retain();

    glview->swapBuffers();

    while (!glview->windowShouldClose())
    {
        lastTime = getCurrentMillSecond();

        director->mainLoop();
        glview->pollEvents();

        curTime = getCurrentMillSecond();
        if (curTime - lastTime < _animationInterval)
        {
            usleep((_animationInterval - curTime + lastTime)*1000);
        }
    }

    if (glview->isOpenGLReady())
    {
        director->end();
        director->mainLoop();
        director = nullptr;
    }

    glview->release();
    return EXIT_SUCCESS;
}

void Application::setAnimationInterval(double interval)
{
    _animationInterval = interval * 1000.0f;
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
    char *pLanguageName = getenv("LANG");
    LanguageType ret = LanguageType::ENGLISH;
    if (!pLanguageName)
    {
        return LanguageType::ENGLISH;
    }
    strtok(pLanguageName, "_");
    if (!pLanguageName)
    {
        return LanguageType::ENGLISH;
    }

    if (0 == strcmp("zh", pLanguageName))
    {
        ret = LanguageType::CHINESE;
    }
    else if (0 == strcmp("en", pLanguageName))
    {
        ret = LanguageType::ENGLISH;
    }
    else if (0 == strcmp("fr", pLanguageName))
    {
        ret = LanguageType::FRENCH;
    }
    else if (0 == strcmp("it", pLanguageName))
    {
        ret = LanguageType::ITALIAN;
    }
    else if (0 == strcmp("de", pLanguageName))
    {
        ret = LanguageType::GERMAN;
    }
    else if (0 == strcmp("es", pLanguageName))
    {
        ret = LanguageType::SPANISH;
    }
    else if (0 == strcmp("nl", pLanguageName))
    {
        ret = LanguageType::DUTCH;
    }
    else if (0 == strcmp("ru", pLanguageName))
    {
        ret = LanguageType::RUSSIAN;
    }
    else if (0 == strcmp("ko", pLanguageName))
    {
        ret = LanguageType::KOREAN;
    }
    else if (0 == strcmp("ja", pLanguageName))
    {
        ret = LanguageType::JAPANESE;
    }
    else if (0 == strcmp("hu", pLanguageName))
    {
        ret = LanguageType::HUNGARIAN;
    }
    else if (0 == strcmp("pt", pLanguageName))
    {
        ret = LanguageType::PORTUGUESE;
    }
    else if (0 == strcmp("ar", pLanguageName))
    {
        ret = LanguageType::ARABIC;
    }
    else if (0 == strcmp("nb", pLanguageName))
    {
        ret = LanguageType::NORWEGIAN;
    }
    else if (0 == strcmp("pl", pLanguageName))
    {
        ret = LanguageType::POLISH;
    }
    else if (0 == strcmp("tr", pLanguageName))
    {
        ret = LanguageType::TURKISH;
    }
    else if (0 == strcmp("uk", pLanguageName))
    {
        ret = LanguageType::UKRAINIAN;
    }
    else if (0 == strcmp("ro", pLanguageName))
    {
        ret = LanguageType::ROMANIAN;
    }
    else if (0 == strcmp("bg", pLanguageName))
    {
        ret = LanguageType::BULGARIAN;
    }
    return ret;
}

const char * Application::getCurrentLanguageCode()
{
    static char code[3]={0};
    char *pLanguageName = getenv("LANG");
    if (!pLanguageName)
        return "en";
    strtok(pLanguageName, "_");
    if (!pLanguageName)
        return "en";
    strncpy(code,pLanguageName,2);
    code[2]='\0';
    return code;
}

Application::Platform Application::getTargetPlatform()
{
    return Platform::OS_LINUX;
}

bool Application::openURL(const std::string &url)
{
    std::string op = std::string("open ").append(url);
    return system(op.c_str()) != -1;
}

NS_END

#endif
