#ifndef __DEV_TOOLS_CPP_LIBRARIES_UTILS_OS_TOOLS_H__
#define __DEV_TOOLS_CPP_LIBRARIES_UTILS_OS_TOOLS_H__

#include <string>

namespace OS {
    std::string Join(const std::string& path1, const std::string& path2);

    std::string Basename(const std::string& path);
    std::string Dirname(const std::string& path);
};

#endif
