#pragma once

namespace MapMarkerExtensionFramework
{
	[[nodiscard]] UInt32 __stdcall DetourVtable(UInt32 addr, UInt32 dst);

	void Install();
}