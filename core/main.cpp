#include "common.h"
#include "host.h"

const string _version = "0.9.0";
const int startTime = time(0);
string bot_path = "bot";

int main(int argc, char* argv[])
{
	if(argc == 2) bot_path = argv[1]; // TODO
	return host::runApplication();
}
