#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include "KenzGUI/CallGUI_.h"
#include "DrawMenu.h"
#define targetLib "libil2cpp.so"
#include "Hooks.h"
#include "Zygisk/zygisk.h"

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;
 



// ===================================================== //
#define targetPackageName OBFUSCATE("com.WandaSoftware.TruckersofEurope3") // Here add your game package name ..... For example purpose i will use sniper 3d game 

uintptr_t G_IL2CPP;


#include <ctime>
#include <cstdio>

void DrawNetBackground(const ImVec2& window_pos, const ImVec2& window_size, float time) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Parameters for the net
    const float grid_spacing = 40.0f; // Distance between grid points
    const float amplitude = 5.0f;     // Amount of vertical oscillation for animation
    const int cols = static_cast<int>(window_size.x / grid_spacing) + 2;
    const int rows = static_cast<int>(window_size.y / grid_spacing) + 2;

    ImU32 line_color = ImGui::GetColorU32(ImVec4(0.0f, 0.6f, 0.0f, 0.6f)); // Semi-transparent green
    ImU32 point_color = ImGui::GetColorU32(ImVec4(0.0f, 1.0f, 0.0f, 0.8f)); // Bright green for points

    // Store points with animation offset
    std::vector<ImVec2> points(rows * cols);

    // Calculate points with vertical wave animation
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            float x = window_pos.x + col * grid_spacing;
            float y = window_pos.y + row * grid_spacing;

            // Animate y with sine wave based on time and position for glowing wave effect
            float offset = sinf(time * 3.0f + (col + row) * 0.5f) * amplitude;
            points[row * cols + col] = ImVec2(x, y + offset);
        }
    }

    // Draw horizontal lines
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols - 1; ++col) {
            draw_list->AddLine(points[row * cols + col], points[row * cols + col + 1], line_color, 1.0f);
        }
    }

    // Draw vertical lines
    for (int col = 0; col < cols; ++col) {
        for (int row = 0; row < rows - 1; ++row) {
            draw_list->AddLine(points[row * cols + col], points[(row + 1) * cols + col], line_color, 1.0f);
        }
    }

    // Draw points
    for (const auto& pt : points) {
        draw_list->AddCircleFilled(pt, 2.0f, point_color);
    }
}


ImVec4 RainbowColor(float t) {
    float r = sinf(t * 6.283185f) * 0.5f + 0.5f;
    float g = sinf(t * 6.283185f + 2.094f) * 0.5f + 0.5f;
    float b = sinf(t * 6.283185f + 4.188f) * 0.5f + 0.5f;
    return ImVec4(r, g, b, 1.0f);
}

time_t GetExpiryTimestamp(const char* expiry_date_str) {
    // expiry_date_str format is "DD-MM-YY"
    struct tm expiry_tm = {0};
    int day, month, year;
    if (sscanf(expiry_date_str, "%d-%d-%d", &day, &month, &year) != 3) {
        return 0; // invalid format fallback, never expires
    }
    expiry_tm.tm_mday = day;
    expiry_tm.tm_mon = month - 1; // tm_mon is 0-11
    expiry_tm.tm_year = (year < 100) ? (year + 100) : year; // 2000-based year (e.g., 25 -> 2025)
    expiry_tm.tm_hour = 0;
    expiry_tm.tm_min = 0;
    expiry_tm.tm_sec = 0;
    expiry_tm.tm_isdst = -1; // let system determine
    return mktime(&expiry_tm);
}


void DrawMenu() {
    // Set your expiry date here in DD-MM-YY format
    static time_t expiry_timestamp = GetExpiryTimestamp("28-08-225"); // Set your ModMenu Expiry date...so it will automatically expire your ModMenu according to date you set date/month/year
    
    time_t now = time(nullptr);
    ImVec2 window_size = ImGui::GetIO().DisplaySize;

    // If expired, show expiry message centered and skip rest of menu
    if (now > expiry_timestamp && expiry_timestamp != 0) {
        ImGui::SetNextWindowBgAlpha(0.75f);
        ImGui::SetNextWindowPos(ImVec2(window_size.x / 2, window_size.y / 2), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::Begin("EXPIRED", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "- Note : ModMenu is expired -");
        ImGui::End();
        return; // Prevent drawing the rest of the menu when expired
    }
		
    const ImVec2 windowDim = ImVec2(700, 700);
    ImGui::SetNextWindowSize(windowDim, ImGuiCond_Once);

    if (ImGui::Begin(ICON_FA_GAMEPAD " TheGhost [x64]")) {
		ImVec2 pos = ImGui::GetWindowPos();   // <-- Declare pos here
        ImVec2 size = ImGui::GetWindowSize(); // <-- Declare size here
		float Meow = ImGui::GetTime();
		DrawNetBackground(pos, size, Meow);
        static int activeTab = 0;
        float windowWidth = ImGui::GetContentRegionAvail().x;

        // Tab buttons layout
        float spacing = 8.0f;
        int tabCount = 3;
        float buttonWidth = (windowWidth - spacing * (tabCount - 1)) / tabCount;
        float buttonHeight = 48.0f;

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        float time = ImGui::GetTime();

        auto TabButton = [&](const char* label, int id) {
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImGuiStyle& style = ImGui::GetStyle();

            float oldRounding = style.FrameRounding;
            style.FrameRounding = 0.0f;

            ImVec2 size(buttonWidth, buttonHeight);

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            bool pressed = ImGui::Button(label, size);
            ImGui::PopStyleVar();

            if (pressed)
                activeTab = id;

            style.FrameRounding = oldRounding;

            if (activeTab == id) {
                ImVec2 min = pos;
                ImVec2 max = ImVec2(pos.x + size.x, pos.y + size.y);
                float thickness = 3.0f;
                int segments = 80;

                for (int i = 0; i < segments; i++) {
                    float t0 = (float)i / segments;
                    float t1 = (float)(i + 1) / segments;
                    float shift = time * 0.5f;
                    ImVec4 col0 = RainbowColor(t0 + shift);
                    ImU32 c0 = ImGui::GetColorU32(col0);

                    ImVec2 p0, p1;
                    if (i < segments / 4) {
                        p0 = ImVec2(min.x + size.x * (t0 * 4), min.y);
                        p1 = ImVec2(min.x + size.x * (t1 * 4), min.y);
                    } else if (i < segments / 2) {
                        float tt0 = (t0 - 0.25f) * 4;
                        float tt1 = (t1 - 0.25f) * 4;
                        p0 = ImVec2(max.x, min.y + size.y * tt0);
                        p1 = ImVec2(max.x, min.y + size.y * tt1);
                    } else if (i < 3 * segments / 4) {
                        float tt0 = (t0 - 0.5f) * 4;
                        float tt1 = (t1 - 0.5f) * 4;
                        p0 = ImVec2(max.x - size.x * tt0, max.y);
                        p1 = ImVec2(max.x - size.x * tt1, max.y);
                    } else {
                        float tt0 = (t0 - 0.75f) * 4;
                        float tt1 = (t1 - 0.75f) * 4;
                        p0 = ImVec2(min.x, max.y - size.y * tt0);
                        p1 = ImVec2(min.x, max.y - size.y * tt1);
                    }

                    draw_list->AddLine(p0, p1, c0, thickness);
                }
            }
        };

        // Draw tab buttons
        for (int i = 0; i < tabCount; i++) {
            if (i > 0) ImGui::SameLine(0, spacing);
            if (i == 0) TabButton(ICON_FA_EYE " Visual", 0);
            else if (i == 1) TabButton(ICON_FA_BARS " Memory", 1);
            else if (i == 2) TabButton(ICON_FA_COGS " Setting", 2);
        }

        ImGui::Separator();

        float spacingChild = 8.0f;

        // Visual Tab
        if (activeTab == 0) {
            float halfWidth = (windowWidth - spacingChild) / 2.0f;

            ImGui::BeginChild("left", ImVec2(halfWidth, 0), true);
            ImGui::PushItemWidth(-1); 
			
            // --- Fitur Ekonomi ---
            ImGui::Checkbox("Unlimited Gold", &IsIntExample2); // Nama sudah diganti sayang 🩷
            ImGui::Checkbox("Unlimited Balance", &IsBalanceMod);             
            ImGui::Separator(); // Kasih garis pembatas biar rapi
            // --- Fitur Trailer (Sudah Digabung di Sini) ---
            ImGui::Checkbox("Remove Ads (AdFree)", &IsNoAds);
            ImGui::Checkbox("Unlock All Trailers", &IsUnlockAllTrailer);
            ImGui::Checkbox("Unlock All Driver Slots", &IsUnlockDrivers);
            ImGui::Checkbox("No Trailer Damage", &IsNoTrailerDamage);

            ImGui::Checkbox("Super Stability", &IsSuperStable);
            //ImGui::Checkbox("Anti-Weight Cargo", &IsLightTrailer);
            ImGui::Checkbox("Instant Connect", &IsInstantConnect); // Nah, ini dia sayang! 🩷
            ImGui::Checkbox("Super Traction (Anti-Slip)", &IsSuperTraction);
            ImGui::Checkbox("Remove All Ads", &IsNoAds);
            ImGui::Checkbox("Super Job (Big Money & XP)", &IsSuperJob);
            ImGui::Checkbox("Instant Max Level & XP", &IsMaxLevel);

            ImGui::Separator();
            
            // --- Fitur Bawaan Kamu ---
			/*ImGui::Checkbox("Ultra Vision", &Dummytoggle2);
			ImGui::Checkbox("Ghost Trail", &Dummytoggle3);
			ImGui::Checkbox("Turbo Fill", &Dummytoggle4);
			ImGui::Checkbox("Phase Shift", &Dummytoggle5);
			ImGui::Checkbox("Stealth Mode", &Dummytoggle6);
			ImGui::Checkbox("Magic Material", &Dummytoggle7);*/
			
            ImGui::PopItemWidth();
            ImGui::EndChild();

            ImGui::SameLine(0, spacingChild);

            ImGui::BeginChild("right", ImVec2(0, 0), true);
            ImGui::PushItemWidth(-1);
            /*ImGui::Checkbox("World", &Dummytoggle8);
            ImGui::Checkbox("Remap", &Dummytoggle9);
            ImGui::Checkbox("Night", &Dummytoggle10);*/
            ImGui::Separator();
            ImGui::TextDisabled("Other");

            static bool esp_box = false;
            static bool esp_enabled = false;
            static float esp_width = 3.4f;

            ImGui::Checkbox("ESP (3D Box)", &esp_box);
            ImGui::Checkbox("ESP", &esp_enabled);
            ImGui::SliderFloat("Width", &esp_width, 0.5f, 10.0f, "%.1f");
            ImGui::PopItemWidth();
            ImGui::EndChild();
            
            // This hacks are just dummy......you can add youe own hacks here
        }

        // Memory Tab
        if (activeTab == 1) {
            ImGui::BeginChild("memory_tab", ImVec2(0, 0), true);
            ImGui::PushItemWidth(-1);
            ImGui::Text("Memory Features Here...");
            ImGui::Checkbox("Instant Torque (Speed)", &IsInstantTorque);
            ImGui::Separator();
            ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Engine Sound Settings");
            ImGui::Checkbox("Custom RPM (Sound)", &IsCustomRPM);
        if (IsCustomRPM) {
           // Slider dari 0 sampai 8000 RPM
            ImGui::SliderFloat("RPM Level", &CustomRPMValue, 0.0f, 8000.0f, "%.0f RPM");
}
            ImGui::Checkbox("Super Brake (Instant)", &IsSuperBrake);
            
            ImGui::Checkbox("No Fuel Consumption", &IsNoFuelConsum);

            ImGui::PopItemWidth();
            ImGui::EndChild();
        }

        // Settings Tab
        if (activeTab == 2) {
            ImGui::BeginChild("settings_tab", ImVec2(0, 0), true);
            ImGui::PushItemWidth(-1);
            ImGui::Text("Settings Page...");
            ImGui::Checkbox("Dark Mode", &Dummytoggle13);
            ImGui::Checkbox("Show FPS", &Dummytoggle14);
            ImGui::PopItemWidth();
            ImGui::EndChild();
        }

        ImGui::End();
    }
}





EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    static bool is_setup = false;
    static bool should_clear_mouse_pos = false;
    if (!is_setup) {
		SetGUI();
        is_setup = true;
    }
	
    ImGuiIO &io = ImGui::GetIO();
    int (*TouchCount)(void*) = (int (*)(void*)) (Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("get_touchCount"), 0));
    int touchCount = TouchCount(nullptr);
    if (touchCount > 0) {
        UnityEngine_Touch_Fields touch = ((UnityEngine_Touch_Fields (*)(int)) (Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("GetTouch"), 1))) (0);
        float reverseY = io.DisplaySize.y - touch.m_Position.fields.y;

        switch (touch.m_Phase) {
            case TouchPhase::Began:
            case TouchPhase::Stationary:
                io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
                io.MouseDown[0] = true;
                break;
            case TouchPhase::Ended:
            case TouchPhase::Canceled:
                io.MouseDown[0] = false;
                should_clear_mouse_pos = true;
                break;
            case TouchPhase::Moved:
                io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
                break;
            default:
                break;
        }
    } else {
        io.MouseDown[0] = false;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    DrawMenu();
	ImGui::Render();
    glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::EndFrame();
    if (should_clear_mouse_pos) {
        io.MousePos = ImVec2(-1, -1);
        should_clear_mouse_pos = false;
    }
    return old_eglSwapBuffers(dpy, surface);
}

void *hack_start(void *) {
    while (!G_IL2CPP) {
    G_IL2CPP = Tools::GetBaseAddress(targetLib);
    sleep(1);
    }
    Tools::Hook((void *) dlsym(RTLD_NEXT, "eglSwapBuffers"), (void *) hook_eglSwapBuffers, (void **) &old_eglSwapBuffers);
    Il2CppAttach();
    sleep(5);
	
	// Namespace: User
	//public class Profile 
	//public Int32 get_HardCurrency() { }
	
	
	
// Bagian instalasi hook yang sudah diubah sesuai class Prefs
Tools::Hook(
    Il2CppGetMethodOffset(
        OBFUSCATE("Assembly-CSharp.dll"), 
        OBFUSCATE(""),               // Namespace dikosongkan (tadinya "User")
        OBFUSCATE("Prefs"),          // Class diganti jadi "Prefs" (tadinya "Profile")
        OBFUSCATE("get_Gold"),       // Method diganti jadi "get_Gold" (tadinya "get_HardCurrency")
        0                            // Parameter tetap 0
    ),
    (void *) &IntExample2, 
    (void **) &old_IntExample2
);

// Hook untuk Balance (Mata uang lain biasanya)
Tools::Hook(
    Il2CppGetMethodOffset(
        OBFUSCATE("Assembly-CSharp.dll"), 
        OBFUSCATE(""),           // Namespace kosong
        OBFUSCATE("Prefs"),      // Class: Prefs
        OBFUSCATE("get_Balance"),// Method: get_Balance
        0                        // Parameter: 0
    ),
    (void *) &get_Balance_Hook, 
    (void **) &old_Balance
);

// Hook untuk kestabilan trailer (SetStability)
Tools::Hook(
    Il2CppGetMethodOffset(
        OBFUSCATE("Assembly-CSharp.dll"), 
        OBFUSCATE(""),               // Namespace kosong
        OBFUSCATE("TrailerController"), 
        OBFUSCATE("SetStability"), 
        0                            // 0 parameter
    ),
    (void *) &SetStability_Hook, 
    (void **) &old_SetStability
);

// Hook untuk berat suspensi (SetSpringRate)
/*Tools::Hook(
    Il2CppGetMethodOffset(
        OBFUSCATE("Assembly-CSharp.dll"), 
        OBFUSCATE(""), 
        OBFUSCATE("TrailerController"), 
        OBFUSCATE("SetSpringRate"), 
        1                            // 1 parameter (float weight)
    ),
    (void *) &SetSpringRate_Hook, 
    (void **) &old_SetSpringRate
);*/

// Hook Update untuk Instant Connect
Tools::Hook(
    Il2CppGetMethodOffset(
        OBFUSCATE("Assembly-CSharp.dll"), 
        OBFUSCATE(""), 
        OBFUSCATE("TrailerController"), 
        OBFUSCATE("Update"), 
        0
    ), 
    (void *)&TrailerUpdate, 
    (void **)&old_TrailerUpdate
);

// Hook Max Power
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("VehiclePhysics.dll"), OBFUSCATE("VehiclePhysics"), OBFUSCATE("Engine"), OBFUSCATE("GetMaxPowerTorque"), 1), (void *)&GetMaxPower_Hook, (void **)&old_GetMaxPower);

// Hook Clamp (Penting banget biar gak ketahan!)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("VehiclePhysics.dll"), OBFUSCATE("VehiclePhysics"), OBFUSCATE("Engine"), OBFUSCATE("ClampPowerTorque"), 2), (void *)&ClampTorque_Hook, (void **)&old_ClampTorque);

// Hook Friction
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("VehiclePhysics.dll"), OBFUSCATE("VehiclePhysics"), OBFUSCATE("Engine"), OBFUSCATE("GetFrictionTorque"), 1), (void *)&Friction_Hook, (void **)&old_Friction);


// Hook Specific Fuel
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("VehiclePhysics.dll"), OBFUSCATE("VehiclePhysics"), OBFUSCATE("Engine"), OBFUSCATE("CalculateSpecificFuelConsumption"), 1), (void *)&SpecificFuel_Hook, (void **)&old_SpecificFuel);

// Hook Instant Fuel
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("VehiclePhysics.dll"), OBFUSCATE("VehiclePhysics"), OBFUSCATE("Engine"), OBFUSCATE("CalculateInstantFuelConsumption"), 1), (void *)&InstantFuel_Hook, (void **)&old_InstantFuel);

// Hook Max Fuel Rate
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("VehiclePhysics.dll"), OBFUSCATE("VehiclePhysics"), OBFUSCATE("Engine"), OBFUSCATE("GetMaxFuelRate"), 1), (void *)&MaxFuelRate_Hook, (void **)&old_MaxFuelRate);


// Hook untuk Sensor RPM (0 parameter)
Tools::Hook(
    Il2CppGetMethodOffset(
        OBFUSCATE("VehiclePhysics.dll"), 
        OBFUSCATE("VehiclePhysics"), 
        OBFUSCATE("Engine"), 
        OBFUSCATE("get_sensorRpm"), 
        0
    ),
    (void *) &get_sensorRpm_Hook, 
    (void **) &old_get_sensorRpm
);

// Hook AddFrictionTorque (1 parameter: frictionTorque)
Tools::Hook(
    Il2CppGetMethodOffset(
        OBFUSCATE("VehiclePhysics.dll"), 
        OBFUSCATE("VehiclePhysics"), 
        OBFUSCATE("Engine"), 
        OBFUSCATE("AddFrictionTorque"), 
        1                            // Karena ada (Single frictionTorque)
    ),
    (void *) &AddFrictionTorque_Hook, 
    (void **) &old_AddFrictionTorque
);

Tools::Hook(
    Il2CppGetMethodOffset(
        OBFUSCATE("VehiclePhysics.dll"), 
        OBFUSCATE("VehiclePhysics"), 
        OBFUSCATE("Engine"), 
        OBFUSCATE("get_sensorTcsEngaged"), 
        0
    ),
    (void *) &get_Tcs_Hook, 
    (void **) &old_get_Tcs
);

// Hook Status No Ads
Tools::Hook(
    Il2CppGetMethodOffset(
        OBFUSCATE("Wanda.GameServices.dll"), 
        OBFUSCATE("Wanda"), 
        OBFUSCATE("ServiceManager"), 
        OBFUSCATE("get_NoAds"), 
        0
    ),
    (void *) &get_NoAds_Hook, 
    (void **) &old_get_NoAds
);

// Hook Izin Banner Ad
Tools::Hook(
    Il2CppGetMethodOffset(
        OBFUSCATE("Wanda.GameServices.dll"), 
        OBFUSCATE("Wanda"), 
        OBFUSCATE("ServiceManager"), 
        OBFUSCATE("CanShowBannerAd"), 
        0
    ),
    (void *) &CanShowAds_Hook, 
    (void **) &old_CanShowAds
);

// --- Instalasi Hook Job ---

// Hook pas Ambil Kerja (Bonus Gaji)
Tools::Hook(
    Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("JobManager"), OBFUSCATE("GetJob"), 1),
    (void *) &GetJob_Hook, (void **) &old_GetJob
);

// Hook pas Selesai Kerja (Instant Rich & XP)
Tools::Hook(
    Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("JobComplete"), OBFUSCATE("Completed"), 1),
    (void *) &JobCompleted_Hook, (void **) &old_JobCompleted
);

// Hook Level (Class: Prefs, Method: get_Level)
Tools::Hook(
    Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("Prefs"), OBFUSCATE("get_Level"), 0),
    (void *) &GetLevel_Hook, (void **) &old_GetLevel
);

// Hook XP (Class: Prefs, Method: get_Xp)
Tools::Hook(
    Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("Prefs"), OBFUSCATE("get_Xp"), 0),
    (void *) &GetXP_Hook, (void **) &old_GetXP
);

// Hook AdFree
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("Prefs"), OBFUSCATE("get_AdFree"), 0), (void *)&AdFree_Hook, (void **)&old_AdFree);

// Hook Unlock Trailers
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("Prefs"), OBFUSCATE("get_UnlockTrailers"), 0), (void *)&UnlockTrailers_Hook, (void **)&old_UnlockTrailers);

// Hook Trailer Damage
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("Prefs"), OBFUSCATE("get_TrailerDamage"), 0), (void *)&GetTrailerDamage_Hook, (void **)&old_GetTrailerDamage);

// Hook SelectNewDriver (1 parameter: Int32 id)
Tools::Hook(
    Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("DriverManager"), OBFUSCATE("SelectNewDriver"), 1),
    (void *) &SelectNewDriver_Hook, 
    (void **) &old_SelectNewDriver
);

// Hook HireNewDriver (0 parameter)
Tools::Hook(
    Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("DriverManager"), OBFUSCATE("HireNewDriver"), 0),
    (void *) &HireNewDriver_Hook, 
    (void **) &old_HireNewDriver
);

	
	// And here you can add your hooks
	//Note for some games the Namespace is empty...so in this case you need to leave that blank like that
	
    pthread_exit(nullptr);
    return nullptr;
}


class ImGuiModMenu : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        this->api = api;
        this->env = env;
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        auto package_name = env->GetStringUTFChars(args->nice_name, nullptr);
        auto app_data_dir = env->GetStringUTFChars(args->app_data_dir, nullptr);
        preSpecialize(package_name, app_data_dir);
        env->ReleaseStringUTFChars(args->nice_name, package_name);
        env->ReleaseStringUTFChars(args->app_data_dir, app_data_dir);
    }

    void postAppSpecialize(const AppSpecializeArgs *) override {
        if (enable_hack) {
            std::thread hack_thread(hack_start, game_data_dir);
            hack_thread.detach();
        }
    }

private:
    Api *api;
    JNIEnv *env;
    bool enable_hack;
    char *game_data_dir;

    void preSpecialize(const char *package_name, const char *app_data_dir) {
        if (strcmp(package_name, targetPackageName) == 0) {
            enable_hack = true;
            game_data_dir = new char[strlen(app_data_dir) + 1];
            strcpy(game_data_dir, app_data_dir);
        }
    }
};

REGISTER_ZYGISK_MODULE(ImGuiModMenu)
