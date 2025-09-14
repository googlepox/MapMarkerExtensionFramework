#pragma once
#include "Defs.h"
#include "MarkerData.h"

namespace MapMarkerExtensionFramework
{

	struct ConditionalInput
	{
		ConditionalInput(TESObjectREFR* a_ref, TESForm* a_form) :
			ref(a_ref),
			base(a_form),
			currentCell(a_ref->parentCell)
		{
		}

		[[nodiscard]] bool IsValid(const UInt32& a_data) const;

		// members
		TESObjectREFR* ref;
		TESForm* base;
		TESObjectCELL* currentCell;
	};

	class Manager
	{
	public:
		static Manager* GetSingleton()
		{
			static Manager singleton;
			return std::addressof(singleton);
		}

		void SetCurrentMarker(TESForm* marker);
		void SetCurrentMarkerName(const char* marker);
		MarkerData GetMarkerData();
		MarkerData GetMarkerDataHUD();
		TESForm* GetCurrentMarker();
		std::string GetCurrentMarkerName();

		void LoadFormsOnce();


		// members
		MarkerMap<MarkerData>         markerForms{};
		std::vector<UINT32>         mapMarkers{};

	private:
		Manager() = default;
		~Manager() = default;

		Manager(const Manager&) = delete;
		Manager(Manager&&) = delete;
		Manager& operator=(const Manager&) = delete;
		Manager& operator=(Manager&&) = delete;

		void LoadForms();

		MarkerMap<MarkerData>& get_form_map(const std::string& a_str);
		static void           get_forms(const std::string& a_path, const std::string& a_str, MarkerMap<MarkerData>& a_map);

		// members
		TESForm* currentMarker;
		const char* currentMarkerName;
		MarkerData emptyMarkerData;

		bool           hasConflicts{ false };
		std::once_flag init{};
	};
}