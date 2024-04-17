#include <unistd.h>
#include <string>
#include "optional"
#include "sheep_basic.h"
using namespace std;

class Config
{
public:
    optional <sheep_basic::Error> Parse(int argc,char ** argv);
    string mount_dir;
    string command;
};