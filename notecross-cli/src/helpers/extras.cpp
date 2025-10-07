#include <cctype>
#include <cstdlib>

bool isInteger(const char* s)
{
    if (s == nullptr || *s == '\0')
        return false;
    if (!std::isdigit(*s) && *s != '-' && *s != '+')
        return false;

    char* p;
    std::strtol(s, &p, 10);
    return (*p == '\0');
}
