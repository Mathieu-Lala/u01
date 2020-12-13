#include <iostream>
#include "Object.hpp"

int main()
{
    std::cout << "App is in version: " << VER << "\n";

    Object{}.foo();

    // if VER == 1 impl::v1::Object{}.foo();
    // if VER == 2 impl::v2::Object{}.foo();
    // ...
}
