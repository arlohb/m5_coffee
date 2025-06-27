#include "Utils.h"

namespace __detail {

std::string getMethodName(const char* prettyFunctionC) {
    std::string_view prettyFunction(prettyFunctionC);
    size_t bracket = prettyFunction.rfind("(");
    size_t space = prettyFunction.rfind(" ", bracket) + 1;
    prettyFunction = prettyFunction.substr(space, bracket - space);

    std::string tag;

    /* If there is still a '(', this is a lambda */
    bracket = prettyFunction.find('(');
    if (bracket != std::string_view::npos) {
        prettyFunction = prettyFunction.substr(0, bracket);
        tag = std::string(prettyFunction) + "::lambda";
    } else {
        tag = prettyFunction;
    }
    
    return tag;
}

}
