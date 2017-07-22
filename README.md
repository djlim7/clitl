> Notice: This libaray is under development. Specific parts can be changed.

# CLITL
CLITL is a cross-platform C++ template library.

## Support
It works with Windows Command Prompt and Bash.

## Install
Add this repository as a submodule at your project's repository.
```
$ git submodule add https://github.com/pauis/clitl.git
```

## Demo
```C++
#include "clitl/clitl.hpp"

int main()
{
    clitl::outbuf obuf;
    clitl::ostream cliout(&obuf);

    clitl::rect<clitl::coord_t> rec(std::pair<clitl::coord_t, clitl::coord_t>(2, 1),
        std::pair<clitl::coord_t, clitl::coord_t>(7, 3), clitl::color::CYAN);

    cliout << clitl::pre_process;
    cliout << rec;
    cliout << clitl::post_process;
}
```
```
Output:

  @@@@@@
  @@@@@@
  @@@@@@
```

## Instruction
### Namespace
This template library uses namespace `clitl`.
### Class specification

