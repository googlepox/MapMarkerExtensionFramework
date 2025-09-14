#pragma once

#include "Defs.h"

namespace MapMarkerExtensionFramework
{
	class MarkerData
	{
	public:
		struct Input
		{
			std::string iconPath;
			TESForm* ref;
		};

		MarkerData();
		explicit MarkerData(const Input& a_input);

		[[nodiscard]] static std::uint32_t GetFormID(const std::string& a_str);

		static void GetForms(const std::string& a_path, const std::string& a_str, std::function<void(std::uint32_t, MarkerData&)> a_func);

		std::string iconPath{};
		TESForm* ref;
	};
}