#include <PGN/Platform/Thread/sleep.h>
#include <unistd.h>

void pgn::sleep(unsigned milliSeconds)
{
    usleep(milliSeconds * 1000);
}
