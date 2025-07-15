#include "ScreenshotHandler.h"
#include "WStringUtil.h"
#include <Shlobj_core.h>
#include "Settings.h"

void ScreenshotHandler::ResetName()
{
	filename = WStringUtil::ConvertStringToWString("");
}

void ScreenshotHandler::Add(const char* a_str)
{
	filename += WStringUtil::ConvertStringToWString(a_str).c_str();
}

void ScreenshotHandler::AddSanitized(const char* a_str)
{
	WCHAR buffer[MAX_PATH];
	wcscpy_s(buffer, WStringUtil::ConvertStringToWString(a_str).c_str());
	PathCleanupSpec(NULL, buffer);
	filename += buffer;
}

std::string ScreenshotHandler::FilenameToString()
{
	return WStringUtil::ConvertWStringToString(filename);
}


const char* ScreenshotHandler::GetCurrentLocationName(RE::Actor* a_actor)
{
	for (auto location = a_actor->GetCurrentLocation(); location; location = location->parentLoc) {
		auto locationName = location->GetName();
		if (locationName && strlen(locationName))
			return locationName;
	}

	for (auto worldspace = a_actor->GetWorldspace(); worldspace; worldspace = worldspace->parentWorld) {
		auto worldspaceName = worldspace->GetName();
		if (worldspaceName && strlen(worldspaceName))
			return worldspaceName;
	}

	return nullptr;
}


void ScreenshotHandler::ModifyFileName(char* dest)
{
	auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
	auto Calendar = RE::Calendar::GetSingleton();

	if (PlayerCharacter && Calendar) {
		ResetName();
        Settings* settings = Settings::GetSingleton();
        if (settings->GetWabbajack()) {
            Add("[NoDelete] Screenshots\\");
        } else {
            Add("Screenshots\\");
        }
		if (PlayerCharacter->GetName() && PlayerCharacter->GetRace() && PlayerCharacter->GetRace()->GetName()) {
			AddSanitized(PlayerCharacter->GetName());
			Add(" - ");
			AddSanitized(PlayerCharacter->GetRace()->GetName());
			Add("\\");
		} else {
			logger::error("Failed to access player information");
		}

		if (!std::filesystem::exists(filename))
			std::filesystem::create_directories(filename);

		auto currentLocationName = GetCurrentLocationName(PlayerCharacter);
		if (currentLocationName) {
			AddSanitized(currentLocationName);
			Add(" - ");
		} else {
			logger::warn("Failed to find a location name");
		}

		char timedate[504]; // This matches what game code does
		Calendar->GetTimeDateString(timedate, 0x200u, 1);
		Add(timedate);  // We can't sanitize due to special character handling

		auto tempFilename = WStringUtil::ReplaceWCSWithPattern(filename, L":", L"꞉");

		if (std::filesystem::exists(tempFilename + WStringUtil::ConvertStringToWString(".png").c_str())) {
			int fixName = 1;
			while (std::filesystem::exists(tempFilename + WStringUtil::ConvertStringToWString(" - " + std::to_string(fixName) + ".png").c_str()))
				fixName++;
			Add(" - ");
			Add(std::to_string(fixName).c_str());
		}

		Add(".png");

		auto finalFilename = WStringUtil::ReplaceWCSWithPattern(filename, L":", L"꞉");
		if ((finalFilename).size() <= (sizeof(wchar_t) * FILENAME_WCHAR_MAX_LENGTH)) {
			if (filename.size() <= PRINT_CHAR_MAX_LENGTH) {
				strcpy_s(dest, PRINT_CHAR_MAX_LENGTH, WStringUtil::ConvertWStringToString(filename).c_str());
			} else {
				strcpy_s(dest, PRINT_CHAR_MAX_LENGTH, "Screenshots\\");
				logger::info("File name was too large to print ingame");
			}
			filename = finalFilename;
		} else {
			logger::error("File name was too large to be renamed");
		}

	}
}


const wchar_t* ScreenshotHandler::FinalFileName()
{
	logger::info(FMT_STRING("Attempting to create save file at {}"sv), FilenameToString());
	return filename.c_str();
}
