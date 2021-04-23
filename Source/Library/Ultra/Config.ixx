module;

#include <string>

export module Ultra.Config;

using std::string;

///
/// @brief Application Configuration
/// @note under construction!
/// 

export namespace Ultra {

class Config {
public:
    Config() = delete;
    ~Config() = delete;

private:
    string mConfigFile = "nul";
};

}
