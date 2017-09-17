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
### Output Demo
```C++
#include "clitl/clitl.hpp"

#include <thread>

int main()
{
    clitl::streambuf obuf;
    clitl::ostream cliout(&obuf);

    clitl::rect<clitl::coord_t> rec(std::pair<clitl::coord_t, clitl::coord_t>(3, 2),
        std::pair<clitl::coord_t, clitl::coord_t>(8, 4), clitl::color::CYAN);

    cliout << clitl::pre_process;
    cliout << rec;
    std::this_thread::sleep_for(std::chrono::seconds(8));
    cliout << clitl::post_process;
}
```
Output:
```

  ██████
  ██████
  ██████
```

### Input Demo
```C++
#include "../clitl.hpp"

#include <iostream>
#include <thread>

int main()
{
    clitl::streambuf ibuf;
    clitl::istream cliin(&ibuf);

    char i[2];
    
    for (;;) {
        cliin >> i; /*
                    i[0]: Key Input or EOF
                    i[1]: NULL
                    */
        std::cout << i[0] << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(64));
    }
}
```
Output:
```

o

k
k


y
```

## Instruction
### Namespace
This template library uses namespace `clitl`.
### Class Specification

