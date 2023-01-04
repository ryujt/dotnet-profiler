// dllmain.h : Declaration of module class.

class CdotnetprofilerModule : public ATL::CAtlDllModuleT< CdotnetprofilerModule >
{
public :
	DECLARE_LIBID(LIBID_DotNetProfilerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DOTNETPROFILER, "{5bbe4e9a-cb29-4446-a431-33c2c8604778}")
};

extern class CdotnetprofilerModule _AtlModule;
