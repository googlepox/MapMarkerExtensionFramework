#include "Manager.h"
#include <MarkerData.h>

namespace MapMarkerExtensionFramework
{
	MarkerMap<MarkerData>& Manager::get_form_map(const std::string& a_str)
	{
		return markerForms;
	}

	void Manager::get_forms(const std::string& a_path, const std::string& a_str, MarkerMap<MarkerData>& a_map)
	{
		return MarkerData::GetForms(a_path, a_str, [&](RE::FormID a_baseID, const MarkerData& a_markerData) {
			a_map[a_baseID] = a_markerData;
			});
	}

	bool ConditionalInput::IsValid(const UInt32& a_data) const
	{
		if (const auto form = LookupFormByID(a_data)) {
			return ref == form;
		}
		return false;
	}

	void Manager::LoadFormsOnce()
	{
		std::call_once(init, [this] {
			LoadForms();
			emptyMarkerData = MarkerData();
			});
	}

	void Manager::LoadForms()
	{
		_MESSAGE("-INI-");

		const std::filesystem::path bosFolder{ R"(Data\MapMarkers)" };
		if (!exists(bosFolder)) {
			_WARNING("MMEF folder not found...");
			return;
		}

		const auto configs = dist::get_configs(R"(Data\MapMarkers)");

		if (configs.empty()) {
			_WARNING("No .ini files were found in Data\\MapMarkers folder, aborting...");
			return;
		}

		_MESSAGE("%u matching inis found", configs.size());

		for (auto& path : configs) {
			_MESSAGE("\tINI : %s", path.c_str());

			CSimpleIniA ini;
			ini.SetUnicode();
			ini.SetMultiKey();
			ini.SetAllowKeyOnly();

			if (const auto rc = ini.LoadFile(path.c_str()); rc < 0) {
				_ERROR("\tcouldn't read INI");
				continue;
			}

			CSimpleIniA::TNamesDepend sections;
			ini.GetAllSections(sections);
			sections.sort(CSimpleIniA::Entry::LoadOrder());

			for (auto& [section, comment, keyOrder] : sections) {
				_MESSAGE("\t\treading [%s]", section);

				CSimpleIniA::TNamesDepend values;
				ini.GetAllKeys(section, values);
				values.sort(CSimpleIniA::Entry::LoadOrder());

				if (!values.empty()) {
					_MESSAGE("\t\t\t%u map markers found", values.size());
					auto& map = get_form_map(section);
					for (const auto& key : values) {
						get_forms(path, key.pItem, map);
					}
				}
			}
		}

		_MESSAGE("-RESULT-");

		_MESSAGE("%u map markers processed", markerForms.size());

		_MESSAGE("-END-");
	}

	TESForm* Manager::GetCurrentMarker()
	{
		return currentMarker;
	}

	std::string Manager::GetCurrentMarkerName()
	{
		return currentMarkerName;
	}

	void Manager::SetCurrentMarker(TESForm* marker)
	{
		currentMarker = marker;
	}

	void Manager::SetCurrentMarkerName(const char* marker)
	{
		currentMarkerName = marker;
	}

	MarkerData Manager::GetMarkerData()
	{

		if (const auto it = markerForms.find(currentMarker->refID); it != markerForms.end())
		{
			return it->second;
		}
		else
		{
			return emptyMarkerData;
		}
	}

	MarkerData Manager::GetMarkerDataHUD()
	{
		for (const auto& pair : markerForms) {
			if (!pair.second.ref) {
				continue;
			}

			if (pair.second.ref->GetFullName() && pair.second.ref->GetFullName()->name.m_data == currentMarkerName) {
				return pair.second;
			}
		}
		return emptyMarkerData;
	}
}