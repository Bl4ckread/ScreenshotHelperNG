#pragma once

class Settings
{
public:
    bool GetWabbajack() const;

    void LoadData(std::string a_type, const CSimpleIniA& a_ini);

    [[nodiscard]] static Settings* GetSingleton() {
        static Settings singleton;
        return std::addressof(singleton);
    }

private:
    struct detail {
        template <class T>
        static void get_value(const CSimpleIniA& a_ini, T& a_value, const char* a_section, const char* a_key) {
            if constexpr (std::is_same_v<T, bool>) {
                a_value = a_ini.GetBoolValue(a_section, a_key, a_value);
            } else if constexpr (std::is_integral_v<T>) {
                a_value = a_ini.GetLongValue(a_section, a_key, a_value);
            } else if constexpr (std::is_same_v<T, double>) {
                a_value = a_ini.GetDoubleValue(a_section, a_key, a_value);
            } else if constexpr (std::is_floating_point_v<T>) {
                a_value = static_cast<float>(a_ini.GetDoubleValue(a_section, a_key, a_value));
            } else {
                a_value = a_ini.GetValue(a_section, a_key, a_value.c_str());
            }
        }
    };

    bool wabbajack;

    Settings();
    Settings(const Settings&) = delete;
    Settings(Settings&&) = delete;

    ~Settings() = default;

    Settings& operator=(const Settings&) = delete;
    Settings& operator=(Settings&&) = delete;
};