#include "Hooks.h"
#include "Defs.h"
#include "Manager.h"

namespace MapMarkerExtensionFramework
{

	struct MapMarkerExtraDataImpl
	{
		static void __fastcall GetMapMarkerExtraDataHook(TESObjectREFR* a1, void* edx)
		{
			Manager::GetSingleton()->SetCurrentMarker((TESForm*) a1);
			ThisStdCall(originalAddress, a1);
		}
		static inline std::uint32_t originalAddress;

		static void Install()
		{
			originalAddress = 0x4D7730;
			WriteRelCall(0x5B9810, reinterpret_cast<std::uint32_t>(GetMapMarkerExtraDataHook));
			_MESSAGE("Installed GetMapMarkerExtraData hook");
		}
	};

	struct MapMarkerTypeImpl
	{
		static void __fastcall GetMapMarkerTypeHook(TESObjectREFR* a1, void* edx)
		{
			Manager::GetSingleton()->SetCurrentMarker((TESForm*) a1);
			ThisStdCall(originalAddress, a1);
			
		}
		static inline std::uint32_t originalAddress;

		static void Install()
		{
			originalAddress = 0x42B370;
			WriteRelCall(0x5B9832, reinterpret_cast<std::uint32_t>(GetMapMarkerTypeHook));
			_MESSAGE("Installed GetMapMarkerType hook");
		}
	};

	struct TileSetFloatMenuImpl
	{
		static void __fastcall TileSetFloatHook(Tile* tile, void* edx, DWORD* a2, float a3)
		{
			MarkerData markerData = Manager::GetSingleton()->GetMarkerData();
			if (markerData.iconPath != "") {
				a3 = 1.0;
				ThisStdCall(0x58CED0, tile, 0xFE6, markerData.iconPath.c_str());
			}
			ThisStdCall(originalAddress, tile, a2, a3);
		}
		static inline std::uint32_t originalAddress;

		static void Install()
		{
			originalAddress = 0x58CEB0;
			WriteRelCall(0x5B984A, reinterpret_cast<std::uint32_t>(TileSetFloatHook));
			_MESSAGE("Installed TileSetFloatMenu hook");
		}
	};

	struct TileSetFloatHUDImpl
	{
		static void __fastcall TileSetFloatHook(Tile* tile, void* edx, DWORD* a2, float a3)
		{
			MarkerData markerData = Manager::GetSingleton()->GetMarkerDataHUD();
			if (markerData.iconPath != "") {
				a3 = 1.0;
				ThisStdCall(0x58CED0, tile, 0xFE6, markerData.iconPath.c_str());
			}
			else {
				ThisStdCall(0x58CED0, tile, 0xFE6, "Menus\\icons\\map\\map_icons_all.dds");
			}
			ThisStdCall(originalAddress, tile, a2, a3);
		}
		static inline std::uint32_t originalAddress;

		static void Install()
		{
			originalAddress = 0x58CEB0;
			WriteRelCall(0x5A7292, reinterpret_cast<std::uint32_t>(TileSetFloatHook));
			_MESSAGE("Installed TileSetFloatHUD hook");
		}
	};

	struct GetModelPathImpl
	{
		static void __fastcall GetModelPathHook(TESModel* model, void* edx)
		{
			Manager::GetSingleton()->LoadFormsOnce();
			const char* path = model->nifPath.m_data;
			if (path != "") {
				Manager::GetSingleton()->SetCurrentMarkerName(path);
			}
			ThisStdCall(originalAddress, model);
		}
		static inline std::uint32_t originalAddress;

		static void Install()
		{
			originalAddress = 0x412AB0;
			WriteRelCall(0x5A722B, reinterpret_cast<std::uint32_t>(GetModelPathHook));
			_MESSAGE("Installed GetModelPath hook");
		}
	};

	void Install()
	{
		_MESSAGE("-HOOKS-");
		MapMarkerExtraDataImpl::Install();
		TileSetFloatMenuImpl::Install();
		TileSetFloatHUDImpl::Install();
		GetModelPathImpl::Install();
		_MESSAGE("Installed all hooks");

	}

	// Credits to lStewieAl
	[[nodiscard]] __declspec(noinline) UInt32 __stdcall DetourVtable(UInt32 addr, UInt32 dst)
	{
		UInt32 originalFunction = *(UInt32*)addr;
		SafeWrite32(addr, dst);
		return originalFunction;
	}
}