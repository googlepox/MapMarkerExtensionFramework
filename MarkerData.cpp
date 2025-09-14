#include "MarkerData.h"

namespace MapMarkerExtensionFramework
{
	MarkerData::MarkerData()
	{
		iconPath = "";
		ref = (*g_thePlayer);
	}

	MarkerData::MarkerData(const Input& a_input) :
		iconPath(a_input.iconPath),
		ref(a_input.ref)
	{
	}

	std::uint32_t MarkerData::GetFormID(const std::string& a_str)
	{
		constexpr auto lookup_formID = [](std::uint32_t a_refID, const std::string& modName) -> std::uint32_t
			{
				const auto modIdx = (*g_dataHandler)->GetModIndex(modName.c_str());
				return modIdx == 0xFF ? 0 : (a_refID & 0xFFFFFF) | modIdx << 24;
			};

		if (const auto splitID = string::split(a_str, "~"); splitID.size() == 2) {
			const auto  formID = string::to_num<std::uint32_t>(splitID[0], true);
			const auto& modName = splitID[1];
			return lookup_formID(formID, modName);
		}
		if (string::is_only_hex(a_str, true))
		{
			if (const auto form = LookupFormByID(string::to_num<std::uint32_t>(a_str, true))) {
				return form->refID;
			}
		}
		return 0;
	}

	void MarkerData::GetForms(const std::string& a_path, const std::string& a_str, std::function<void(std::uint32_t, MarkerData&)> a_func)
	{
		constexpr auto swap_empty = [](const UInt32& a_set) {
			if (const auto formID =(&a_set); formID) {
				return *formID == 0;
			}
		};

		const auto formPair = string::split(a_str, "|");

		if (const auto baseFormID = GetFormID(formPair[0]); baseFormID != 0) {
			TESForm* form = LookupFormByID(baseFormID);
			const Input input(
				formPair[1], form);
			MarkerData markerData(input);
			a_func(baseFormID, markerData);
		}
		else {
			_ERROR("\t\t\tfailed to process %s (BASE formID not found)", a_str.c_str());
		}
	}
}