
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
#define targetPackageName OBFUSCATE("com.jarvigames.viceonline") // Here add your game package name ..... For example purpose i will use sniper 3d game 

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
                // --- TAB 0: MAIN (Visual / General) ---
        if (activeTab == 0) {
            float halfWidth = (windowWidth - spacingChild) / 2.0f;

            // KOLOM KIRI: Ekonomi & Supir
            ImGui::TextColored(ImVec4(1, 1, 1, 1), ICON_FA_BARS " FLEX CITY MOD MENU");
        ImGui::Separator();

        // Langsung tampilkan isi fitur tanpa pengecekan activeTab
        ImGui::BeginChild("main_scroll", ImVec2(0, 0), true);
        ImGui::PushItemWidth(-1);
        
        // --- [ BAGIAN 1: PERTAHANAN ] ---
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "PLAYER DEFENSE");
        ImGui::Checkbox("God Mode (HP/Armor)", &IsGodHealth);
        ImGui::Checkbox("Infinite Armor", &IsMaxArmor);
        ImGui::Checkbox("Max Vehicle HP", &IsMaxVehicleHP);
        
        ImGui::Separator();
        
        // --- [ BAGIAN 2: SERANGAN & GERAKAN ] ---
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "COMBAT & MOVEMENT");
        ImGui::Checkbox("Super Damage (1-Hit)", &IsSuperDamage);
        
        // --- SLIDER SPEED ---
        ImGui::Checkbox("Custom Speed Hack", &IsGodSpeed);
        if (IsGodSpeed) {
            ImGui::SliderFloat("Speed Level", &GodSpeedValue, 1.0f, 90.0f, "%.1f x");
        }
        
        ImGui::Separator();
        
        // --- [ BAGIAN 3: SKILL & MEKANIK ] ---
        ImGui::TextColored(ImVec4(0, 1, 0.5f, 1), "SKILL & EQUIPMENT");
        ImGui::Checkbox("No Cooldown Skill", &IsNoCooldown);
        ImGui::Checkbox("Instant Use Items (Medkit)", &IsInstantApply);
        ImGui::Checkbox("Infinite Duration", &IsInfiniteDuration);
        
        ImGui::Separator();
        
        // --- [ BAGIAN 4: WEAPON SYSTEM ] ---
        ImGui::TextColored(ImVec4(1, 0, 0.5f, 1), "WEAPON SYSTEM");
        ImGui::Checkbox("Rapid Fire", &IsFastFire);
        ImGui::Checkbox("Instant Reload", &IsInstantReload);
        ImGui::Checkbox("One Tap Headshot", &IsOneTapHead);
        ImGui::Checkbox("No Recoil / Laser Aim", &IsNoRecoil);
        ImGui::Checkbox("Unlimited Ammo", &IsUnlimitedAmmo);
        ImGui::Checkbox("Extreme Range", &IsFarRange);
        
        ImGui::Separator();
        
        // --- [ BAGIAN 5: PHYSICS MODS ] ---
        ImGui::TextColored(ImVec4(1, 0, 1, 1), "PHYSICS MODS");
        ImGui::Checkbox("Air Jump / Fly", &IsAirJump);
        
        // --- SLIDER GHOST HITBOX ---
        ImGui::Checkbox("Ghost Hitbox (Size Mod)", &IsGhostMode);
        if (IsGhostMode) {
            ImGui::SliderFloat("Body Width", &GhostRadiusValue, 0.01f, 1.0f, "%.2f");
            ImGui::SliderFloat("Body Height", &GhostHeightValue, 0.01f, 3.0f, "%.2f");
            
            if (ImGui::Button("Reset Size")) {
                GhostRadiusValue = 0.5f;
                GhostHeightValue = 2.0f;
            }
        }
        
        ImGui::PopItemWidth();
        ImGui::EndChild();
        }
        if (activeTab == 1) {
            ImGui::BeginChild("memory_tab", ImVec2(0, 0), true);
            ImGui::PushItemWidth(-1);

            ImGui::PopItemWidth();
            ImGui::EndChild();
        }
        
        if (activeTab == 2) {
            ImGui::BeginChild("settings_tab", ImVec2(0, 0), true);
            ImGui::PushItemWidth(-1);
            ImGui::EndChild();
            
        }

        ImGui::End();
    }
}

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    static bool is_setup = false;
    static bool should_clear_mouse_pos = false;

    // 1. Setup Awal (Penting: Ganti versi shader ke 300 es jika Unity baru)
    if (!is_setup) {
        if (ImGui::GetCurrentContext() == nullptr) {
            ImGui::CreateContext();
        }
        // Flex City biasanya butuh shader GLES3 (#version 300 es)
        ImGui_ImplOpenGL3_Init("#version 300 es"); 
        is_setup = true;
    }
	
    ImGuiIO &io = ImGui::GetIO();

    // 2. PERBAIKAN UTAMA: Ambil ukuran layar asli (Viewport)
    // Tanpa ini, io.DisplaySize adalah 0, yang bikin ImGui invisible/crash
    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    io.DisplaySize = ImVec2((float)m_viewport[2], (float)m_viewport[3]);

    // Ambil fungsi Touch Unity secara dinamis
    int (*TouchCount)(void*) = (int (*)(void*)) (Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("get_touchCount"), 0));
    
    // Cek jika fungsi TouchCount ditemukan agar tidak crash
    if (TouchCount) {
        int touchCount = TouchCount(nullptr);
        if (touchCount > 0) {
            UnityEngine_Touch_Fields touch = ((UnityEngine_Touch_Fields (*)(int)) (Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("GetTouch"), 1))) (0);
            
            // Balik koordinat Y agar pas dengan posisi menu
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
    }

    // 3. Render Menu
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    DrawMenu(); // Pastikan ada ImGui::Begin() & ImGui::End() di sini

    ImGui::Render();
    
    // Reset shader game agar tidak tabrakan dengan ImGui
    glUseProgram(0); 
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Bersihkan posisi mouse jika jari sudah diangkat
    if (should_clear_mouse_pos) {
        io.MousePos = ImVec2(-1, -1);
        should_clear_mouse_pos = false;
    }

    return old_eglSwapBuffers(dpy, surface);
}

void *hack_start(void *) {
    // Tunggu libil2cpp terload sempurna
    while (!G_IL2CPP) {
        G_IL2CPP = Tools::GetBaseAddress(targetLib);
        sleep(1);
    }
    
    // Hook eglSwapBuffers untuk rendering menu
    Tools::Hook((void *) dlsym(RTLD_NEXT, "eglSwapBuffers"), (void *) hook_eglSwapBuffers, (void **) &old_eglSwapBuffers);
    
    Il2CppAttach();
    init_il2cpp_api(); 
    sleep(5);
    
	// Namespace: User
	//public class Profile 
	//public Int32 get_HardCurrency() { }
	
	
	
// ========================================================
// [ 1. FITUR EQUIPMENT (JarviGames.Configs.Models.dll) ]
// ========================================================

// Fitur: God Health (Cocok dengan IsGodHealth di Hooks.h)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Equipment"), OBFUSCATE("get_Health"), 0), (void *)&h_Health, (void **)&old_Health);

// Fitur: Unlimited Armor (Cocok dengan IsMaxArmor di Hooks.h)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Equipment"), OBFUSCATE("get_Armor"), 0), (void *)&h_Armor, (void **)&old_Armor);

// Fitur: Max Base Health (Cocok dengan IsGodHealth di Hooks.h)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Equipment"), OBFUSCATE("get_MaxHealth"), 0), (void *)&h_MaxHealth, (void **)&old_MaxHealth);

// Fitur: Vehicle God Mode (Cocok dengan IsMaxVehicleHP di Hooks.h)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Equipment"), OBFUSCATE("get_VehicleHealth"), 0), (void *)&h_VehicleHealth, (void **)&old_VehicleHealth);

// Fitur: Super Damage Multiplier (Cocok dengan IsSuperDamage di Hooks.h)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Equipment"), OBFUSCATE("get_DamageMultiplier"), 0), (void *)&h_DamageMultiplier, (void **)&old_DamageMultiplier);

// Fitur: Player Speed Hack (Cocok dengan IsGodSpeed di Hooks.h)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Equipment"), OBFUSCATE("get_SpeedMultiplier"), 0), (void *)&h_SpeedMultiplier, (void **)&old_SpeedMultiplier);

// Fitur: Vehicle Speed Hack (Cocok dengan IsGodSpeed di Hooks.h)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Equipment"), OBFUSCATE("get_VehicleSpeedMultiplier"), 0), (void *)&h_VehicleSpeedMultiplier, (void **)&old_VehicleSpeedMultiplier);

// Fitur: Skill No Cooldown (Cocok dengan IsNoCooldown di Hooks.h)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Equipment"), OBFUSCATE("get_Cooldown"), 0), (void *)&h_Cooldown, (void **)&old_Cooldown);

// Fitur: Infinite Duration (Cocok dengan IsInfiniteDuration di Hooks.h)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Equipment"), OBFUSCATE("get_Duration"), 0), (void *)&h_Duration, (void **)&old_Duration);

// Fitur: Instant Apply Item (Cocok dengan IsInstantApply di Hooks.h)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Equipment"), OBFUSCATE("get_ApplyTime"), 0), (void *)&h_ApplyTime, (void **)&old_ApplyTime);

// Fitur: High HP Regeneration (Cocok dengan IsGodHealth di Hooks.h)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Equipment"), OBFUSCATE("get_RecursiveHealth"), 0), (void *)&h_RecursiveHealth, (void **)&old_RecursiveHealth);

// Fitur: No Regen Delay (Cocok dengan IsGodHealth di Hooks.h)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Equipment"), OBFUSCATE("get_Delay"), 0), (void *)&h_Delay, (void **)&old_Delay);

// Fitur: Max Regen Repeats (Cocok dengan IsGodHealth di Hooks.h)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Equipment"), OBFUSCATE("get_Repeats"), 0), (void *)&h_Repeats, (void **)&old_Repeats);

// ========================================================
// [ 2. FITUR WEAPON (JarviGames.Configs.Models.dll) ]
// ========================================================

// Fitur: Rapid Fire (Tembak Cepat)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Weapon"), OBFUSCATE("get_FireRateMs"), 0), (void *)&h_FireRateMs, (void **)&old_FireRateMs);

// Fitur: Instant Reload (Tanpa Delay)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Weapon"), OBFUSCATE("get_ReloadTimeMs"), 0), (void *)&h_ReloadTimeMs, (void **)&old_ReloadTimeMs);

// Fitur: Super Headshot (Damage Kepala)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Weapon"), OBFUSCATE("get_HeadFactor"), 0), (void *)&h_HeadFactor, (void **)&old_HeadFactor);

// Fitur: Unlimited Ammo (Amunisi Abadi)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Weapon"), OBFUSCATE("get_HasUnlimitedAmmo"), 0), (void *)&h_HasUnlimitedAmmo, (void **)&old_HasUnlimitedAmmo);

// Fitur: No Recoil / Peluru Lurus (Spread Angle)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Weapon"), OBFUSCATE("get_SpreadAngle"), 0), (void *)&h_SpreadAngle, (void **)&old_SpreadAngle);

// Fitur: Fokus 1 Titik (Spread Count)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Weapon"), OBFUSCATE("get_SpreadCount"), 0), (void *)&h_SpreadCount, (void **)&old_SpreadCount);


// Fitur: Extreme Range (Jarak Jauh)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("JarviGames.Configs.Models.dll"), OBFUSCATE("JarviGames.Multiplayer.Common"), OBFUSCATE("Weapon"), OBFUSCATE("get_Range"), 0), (void *)&h_Range, (void **)&old_Range);

// ========================================================
// [ 3. FITUR PHYSICS (UnityEngine.PhysicsModule.dll) ]
// ========================================================

// Hook CharacterController: isGrounded (Lompat Tanpa Batas)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.PhysicsModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("CharacterController"), OBFUSCATE("get_isGrounded"), 0), (void *)&h_isGrounded, (void **)&old_isGrounded);

// Hook CharacterController: radius & height (Hitbox Mod)
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.PhysicsModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("CharacterController"), OBFUSCATE("get_radius"), 0), (void *)&h_radius, (void **)&old_radius);
Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.PhysicsModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("CharacterController"), OBFUSCATE("get_height"), 0), (void *)&h_height, (void **)&old_height);

	
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
