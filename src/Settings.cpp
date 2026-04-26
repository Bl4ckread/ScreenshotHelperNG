#include "Settings.h"

void Settings::LoadData(std::string a_section, const CSimpleIniA& a_ini)
{ 

    detail::get_value(a_ini, wabbajack, a_section.c_str(), "Wabbajack");
}

void Settings::SaveData(CSimpleIniA& a_ini) 
{ 
    
    detail::set_value(a_ini, wabbajack, "General", "Wabbajack");
}

bool Settings::GetWabbajack() const { return wabbajack; }

Settings::Settings() {
    const auto path = fmt::format("Data/SKSE/Plugins/{}.ini", SKSE::PluginDeclaration::GetSingleton()->GetName());

    CSimpleIniA ini;
    ini.SetUnicode();
    //ini.SetMultikey();

    if (ini.LoadFile(path.c_str()) >= 0) {
        LoadData("General", ini);
    } else {
        SaveData(ini);
        (void)ini.SaveFile(path.c_str());
    }
}