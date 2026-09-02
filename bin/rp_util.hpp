#include "realpaver/Interval.hpp"
#include "realpaver/Param.hpp"
#include <string>
#include <vector>

#define BLACK(s) "\033[30m" << s << "\033[39m"
#define RED(s) "\033[31m" << s << "\033[39m"
#define GREEN(s) "\033[32m" << s << "\033[39m"
#define ORANGE(s) "\033[33m" << s << "\033[39m"
#define BLUE(s) "\033[34m" << s << "\033[39m"
#define MAGENTA(s) "\033[35m" << s << "\033[39m"
#define CYAN(s) "\033[36m" << s << "\033[39m"
#define GRAY(s) "\033[37m" << s << "\033[39m"

// processes the arguments on the command line
bool processArgs(int argc, char **argv, std::string &pbFilename, std::string &solFilename,
                 std::string &paramFilename, std::string &logFilename,
                 std::vector<std::string> &options, realpaver::Interval &objBounds,
                 std::string &msgError);

// parses the filename
void parseFilename(const std::string &filename, std::string &path, std::string &base,
                   std::string &ext);

// processes the options to assign parameters
bool processOptions(realpaver::Params &params, std::vector<std::string> &options,
                    std::string &msgError);

// replaces a tilde by the home directory in a filename.
bool expandTildeInPath(const std::string &path, std::string &res, std::string &msgError);

// inserts points at the end of a std::string
std::string WP(const std::string &s, int n);
