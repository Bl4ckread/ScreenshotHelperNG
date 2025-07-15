#include "Settings.h"

void Settings::LoadData(std::string a_section, const CSimpleIniA& a_ini)
{ 

    detail::get_value(a_ini, wabbajack, a_section.c_str(), "Wabbajack");
}

bool Settings::GetWabbajack() const { return wabbajack; }

Settings::Settings() {
    const auto path = fmt::format("Data/SKSE/Plugins/{}.ini", SKSE::PluginDeclaration::GetSingleton()->GetName());

    CSimpleIniA ini;
    ini.SetUnicode();
    //ini.SetMultikey();

    ini.LoadFile(path.c_str());

    LoadData("General", ini);

    (void)ini.SaveFile(path.c_str());
}