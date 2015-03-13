SC2Ranks.com API cross-platform C++ Library
===========================================

Example
-------

```cpp
#include <iostream>
#include "sc2ranks.h"

int main()
{
        try
        {
                sc2ranks::appKey = "foobarlol.com";
                sc2ranks::map res = sc2ranks::get_base_char("us", "Vevix", 2127429);

                std::cout << "Name: " << res.get<std::string>("name") << "\n";
                std::cout << "Battle.net ID: " << res.get<int>("bnet_id") << "\n";
                std::cout << "Achievement Points: " << res.get<int>("achievement_points") << "\n";
        }
        catch (std::exception const& e)
        {
                std::cout << "Error: " << e.what() << "\n";
        }
}
```

Output
------

```
Name: Vevix
Battle.net ID: 2127429
Achievement Points: 1775
```

Libraries (Required)
--------------------
- boost	- http://www.boost.org
- cpp-netlib - http://cpp-netlib.github.com (Proposed to Boost)
- tinyjson - included in source (site down)
