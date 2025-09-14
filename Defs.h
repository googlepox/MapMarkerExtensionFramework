#pragma once

//Standard
#include <variant>
#include <string>
#include <ranges>
#include <unordered_set>
#include <unordered_map>

//OBSE
#include "PluginAPI.h"
#include "GameAPI.h"
#include "GameData.h"
#include "GameForms.h"
#include "GameObjects.h"
#include "SafeWrite.h"
#include "ModTable.h"
#include "GameTiles.h"

//Map Marker Extended Framework
#include "lib/string.hpp"
#include "lib/rng.hpp"
#include "lib/distribution.hpp"
#include "lib/numeric.hpp"
#include "lib/simpleINI.hpp"
#include "lib/srell.hpp"

namespace string = clib_util::string;
namespace dist = clib_util::distribution;
namespace numeric = clib_util::numeric;

using FormIDStr = std::variant<std::uint32_t, std::string>;

template <class T>
using MarkerMap = std::unordered_map<FormIDStr, T>;
