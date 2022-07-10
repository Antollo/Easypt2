#include  <stdexcept>
#include  <sstream>
#include <iomanip>
#include "stringUtils.h"

std::string parseString(const std::string &source)
{
    std::string ret;
    size_t length = source.size() - 1;
    ret.reserve(length);
    for (size_t i = 1; i < length; i++)
    {
        if (source[i] != '\\')
            ret.push_back(source[i]);
        else if (i + 1 < length)
            switch (source[++i])
            {
            case '"':
                ret.push_back('"');
                break;
            case '\\':
                ret.push_back('\\');
                break;
            case 'a':
                ret.push_back('\a');
                break;
            case 'b':
                ret.push_back('\b');
                break;
            case 'f':
                ret.push_back('\f');
                break;
            case 'n':
                ret.push_back('\n');
                break;
            case 'r':
                ret.push_back('\r');
                break;
            case 't':
                ret.push_back('\t');
                break;
            case 'v':
                ret.push_back('\v');
                break;
            case '\n':
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            {
                if (i + 2 > length)
                    throw std::runtime_error("wrong escape sequence on position " + std::to_string(i) + " in \"" + source + "\"");
                char temp = source[i+3];
                char *tempPtr = const_cast<char*>(&source[i + 3]);
                *tempPtr = '\0';
                ret.push_back(std::strtol(&source[i], nullptr, 10));
                *tempPtr = temp;
                i += 2;
                break;
            }
            case 'x':
            {
                if (i + 2 > length)
                    throw std::runtime_error("wrong escape sequence on position " + std::to_string(i) + " in \"" + source + "\"");
                char temp = source[i+3];
                char *tempPtr = const_cast<char*>(&source[i + 3]);
                *tempPtr = '\0';
                ret.push_back(std::strtol(&source[i]+1, nullptr, 16));
                *tempPtr = temp;
                i += 2;
                break;
            }
            }
        else
            throw std::runtime_error("wrong escape sequence on position " + std::to_string(i) + " in \"" + source + "\"");
    }
    return ret;
}

std::string escapeString(const std::string &str)
{

    std::string result;
    result.reserve(str.size() + 2);

    result.push_back('"');
    for (char c : str)
    {
        if (' ' <= c and c <= '~' and c != '\\' and c != '"')
        {
            result.push_back(c);
        }
        else
        {
            result.push_back('\\');
            switch (c)
            {
            case '"':
                result.push_back('"');
                break;
            case '\\':
                result.push_back('\\');
                break;
            case '\a':
                result.push_back('a');
                break;
            case '\b':
                result.push_back('b');
                break;
            case '\f':
                result.push_back('f');
                break;
            case '\n':
                result.push_back('n');
                break;
            case '\r':
                result.push_back('r');
                break;
            case '\t':
                result.push_back('t');
                break;
            case '\v':
                result.push_back('v');
                break;
            default:
            {
                std::stringstream s;
                s << 'x' << std::setfill('0') << std::setw(2) << std::hex << (int)(c & 0xff);
                result.append(s.str());
                break;
            }
            }
        }
    }
    result.push_back('"');
    return result;
}