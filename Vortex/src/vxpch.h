#pragma once

#include <functional>
#include <algorithm>
#include <execution>
#include <optional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <memory>
#include <random>
#include <ranges>
#include <cmath>
#include <any>

#include <thread>
#include <mutex>
#include <atomic>
#include <future>

#include <type_traits>
#include <typeinfo>
#include <iomanip>
#include <locale>
#include <limits>

#include <initializer_list>
#include <unordered_map>
#include <unordered_set>
#include <string_view>
#include <valarray>
#include <string>
#include <vector>
#include <bitset>
#include <array>
#include <stack>
#include <deque>
#include <tuple>
#include <list>
#include <map>
#include <set>

#include "Vortex/Core/Log.h"
#include "Vortex/Debug/Instrumentor.h"

#ifdef VX_PLATFORM_WINDOWS
	#include <Windows.h>
#endif // VX_PLATFORM_WINDOWS
