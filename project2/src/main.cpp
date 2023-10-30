#include "lib/app/launcher.hpp"
#include "lib/traits/app_traits.hpp"
#include <iostream>

using namespace app;
using namespace traits;
int main(int argc, char *argv[]) { return launch<AppTraits>(argc, argv); }
