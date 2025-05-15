#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

//--------------------------------------------------------------
//	Base class
//--------------------------------------------------------------

class BaseException : public std::exception
{
public:
    explicit  BaseException(const std::string& message) :_message(message) {}
    const char* what() const noexcept override { return _message.c_str(); }

protected:
    std::string _message;
};

//--------------------------------------------------------------
//	Specific exceptions
//--------------------------------------------------------------

// The window is not initialized
class WindowNotInitializedException : public BaseException
{
public:
    explicit WindowNotInitializedException(const std::string& context = "")
        : BaseException("[Window Error] " + context + "Window is not initialized!")
    {
    }
};

// File upload error (texture, font, etc.)
class FileLoadException : public BaseException
{
public:
    explicit FileLoadException(const std::string& filepath)
        : BaseException("[File Error] Failed to load file: '" + filepath + "'")
    {
    }
};

// Font-related error
class FontException : public BaseException
{
public:
    explicit FontException(const std::string& reason)
        : BaseException("[Font Error] " + reason)
    {
    }
};

// Error in the button operation
class ButtonException : public BaseException
{
public:
    explicit ButtonException(const std::string& reason)
        : BaseException("[Button Error] " + reason)
    {
    }
};


#endif //EXCEPTIONS_HPP