#pragma once

bool Dummytoggle1 = false;
bool Dummytoggle2 = false;
bool Dummytoggle3 = false;
bool Dummytoggle4 = false;
bool Dummytoggle5 = false;
bool Dummytoggle6 = false;
bool Dummytoggle7 = false;
bool Dummytoggle8 = false;
bool Dummytoggle9 = false;
bool Dummytoggle10 = false;
bool Dummytoggle11 = false;
bool Dummytoggle12 = false;
bool Dummytoggle13 = false;
bool Dummytoggle14 = false;
bool Dummytoggle15 = false;
bool Dummytoggle16 = false;
bool Dummytoggle17 = false;
bool Dummytoggle18 = false;
bool Dummytoggle19 = false;
bool Dummytoggle20 = false;


// ========================================================
// [ BAGIAN 1: SEMUA TOMBOL / TOGGLES ]
// ========================================================
inline bool IsIntExample2 = false;      // Unlimited Gold
inline bool IsBalanceMod = false;       // Unlimited Balance
inline bool IsMaxLevel = false;         // Instant Max Level & XP
inline bool IsNoAds = false;            // Remove All Ads (Global)
inline bool IsSuperJob = false;         // Gaji Job & XP Gede
inline bool IsInstantFinish = false;    // Langsung Selesai Kerja
inline bool IsUnlockDrivers = false;    // Buka Slot Supir Karyawan
inline bool IsUnlockAllTrailer = false; // Buka Semua Jenis Trailer
inline bool IsNoTrailerDamage = false;  // Muatan Gak Bisa Rusak
inline bool IsInstantConnect = false;   // Sambung Trailer Instan
inline bool IsInstantTorque = false;    // Speed Monster & No Limit
inline bool IsNoFuelConsum = false;     // Bensin Abadi
inline bool IsCustomRPM = false;        // Suara Mesin (Screamer)
inline bool IsSuperBrake = false;       // Rem Pakem (Anchor)
inline bool IsSuperTraction = false;    // Ban Gak Selip (Anti-Slip)
inline bool IsSuperStable = false;      // Trailer Gak Terbalik
inline float CustomRPMValue = 1000.0f;  // Nilai Slider RPM
inline bool IsUnlockEverything = false; // Toggle buat Unlock Container & Cargo

// --- HELPER SAKTI: CARI DATA PAKE NAMA (TANPA 0x OFFSET) ---
void SetFieldFloat(void *instance, const char *fieldName, float value) {
    if (instance == nullptr) return;
    auto field = il2cpp_class_get_field_from_name(il2cpp_object_get_class((Il2CppObject *)instance), fieldName);
    if (field) {
        il2cpp_field_set_value((Il2CppObject *)instance, field, &value);
    }
}


// ========================================================
// [ BAGIAN 2: WADAH FUNGSI ASLI (POINTERS) ]
// ========================================================
inline int (*old_IntExample2)();
inline float (*old_Balance)();
inline int (*old_GetLevel)();
inline int (*old_GetXP)();
inline bool (*old_get_NoAds)(void *instance);
inline bool (*old_CanShowAds)(void *instance);
inline bool (*old_AdFree)();
inline void (*old_GetJob)(void *instance, float bonusMultiplier);
inline void (*old_JobCompleted)(void *instance, bool skipped);
inline void (*old_SelectNewDriver)(void *instance, int id);
inline void (*old_HireNewDriver)(void *instance);
inline bool (*old_UnlockTrailers)();
inline float (*old_GetTrailerDamage)();
inline void (*old_SetStability)(void *instance);
inline void (*old_TrailerUpdate)(void *instance);
inline float (*old_CalculateTorque)(void *instance, float rpm, float throttle);
inline float (*old_GetMaxPower)(void *instance, float rpm);
inline float (*old_ClampTorque)(void *instance, float pwr, float rpm);
inline float (*old_Friction)(void *instance, float rpm);
inline float (*old_SpecificFuel)(void *instance, float rpm);
inline float (*old_InstantFuel)(void *instance, float speed);
inline float (*old_MaxFuelRate)(void *instance, float rpm);
inline float (*old_get_sensorRpm)(void *instance);
inline float (*old_AddFrictionTorque)(void *instance, float frict);
inline bool (*old_get_Tcs)(void *instance);
inline bool (*old_UnlockTrailers)();
inline bool (*old_TruckPurchased)(void *instance);

// ========================================================
// [ BAGIAN 3: LOGIKA FITUR 1 (ECONOMY, ADS, & JOBS) ]
// ========================================================

// --- EKONOMI & PLAYER LEVEL ---
int IntExample2() { if (IsIntExample2) return 999999999; return old_IntExample2(); }
float get_Balance_Hook() { if (IsBalanceMod) return 999999999.0f; return (old_Balance != nullptr) ? old_Balance() : 0.0f; }
int GetLevel_Hook() { if (IsMaxLevel) return 999999999; return old_GetLevel(); }
int GetXP_Hook() { if (IsMaxLevel) return 999999999; return old_GetXP(); }

// --- ADS & TRAILER STATUS (NO ADS) ---
bool get_NoAds_Hook(void *instance) { if (IsNoAds) return true; return old_get_NoAds(instance); }
bool CanShowAds_Hook(void *instance) { if (IsNoAds) return false; return old_CanShowAds(instance); }
bool AdFree_Hook() { if (IsNoAds) return true; return old_AdFree(); }
bool UnlockTrailers_Hook() { if (IsUnlockAllTrailer) return true; return old_UnlockTrailers(); }

// --- PEKERJAAN (SUPER JOB & DRIVERS) ---
void GetJob_Hook(void *instance, float bonusMultiplier) { if (instance != NULL && IsSuperJob) bonusMultiplier = 1000.0f; old_GetJob(instance, bonusMultiplier); }
void JobCompleted_Hook(void *instance, bool skipped) {
    if (instance != NULL) {
        if (IsSuperJob) {
            *(float *)((uintptr_t)instance + 0x48) = 999999.0f; // Set Gaji (Income)
            *(float *)((uintptr_t)instance + 0x4c) = 0.0f;      // Set Penalty 0
            *(float *)((uintptr_t)instance + 0x70) = 50000.0f;  // Set XP Gede
        }
        if (IsInstantFinish) skipped = true; // Langsung Selesai
    }
    old_JobCompleted(instance, skipped);
}
void SelectNewDriver_Hook(void *instance, int id) { if (instance != NULL && IsUnlockDrivers) *(int *)((uintptr_t)instance + 0x38) = 50; old_SelectNewDriver(instance, id); }
void HireNewDriver_Hook(void *instance) { old_HireNewDriver(instance); }

// ========================================================
// [ BAGIAN 4: LOGIKA FITUR 2 (TRUCK PHYSICS & TRAILERS) ]
// ========================================================

// --- MESIN MONSTER (SPEED & TORQUE) ---
float CalculateTorque_Hook(void *instance, float rpm, float throttle) {
    if (instance != NULL && IsInstantTorque) return 80000.0f; // Tenaga Monster
    return old_CalculateTorque(instance, rpm, throttle);
}

float GetMaxPower_Hook(void *instance, float rpm) {
    if (instance != NULL && IsInstantTorque) return 999999999.0f;
    return old_GetMaxPower(instance, rpm);
}

float ClampTorque_Hook(void *instance, float powerTorque, float rpm) {
    if (instance != NULL && IsInstantTorque) return 999999.0f; // Bypass Limiter
    return old_ClampTorque(instance, powerTorque, rpm);
}

float Friction_Hook(void *instance, float rpm) {
    if (instance != NULL && IsInstantTorque) return 0.0f; // Tanpa Hambatan
    return old_Friction(instance, rpm);
}

// --- BENSIN ABADI (INFINITE FUEL) ---
float SpecificFuel_Hook(void *instance, float rpm) {
    if (IsNoFuelConsum) return 0.0f;
    return old_SpecificFuel(instance, rpm);
}

float InstantFuel_Hook(void *instance, float speed) {
    if (IsNoFuelConsum) return 0.0f;
    return old_InstantFuel(instance, speed);
}

float MaxFuelRate_Hook(void *instance, float rpm) {
    if (IsNoFuelConsum) return 0.0f;
    return old_MaxFuelRate(instance, rpm);
}

// --- HANDLING & SUARA (SENSOR) ---
float get_sensorRpm_Hook(void *instance) {
    if (instance != NULL && IsCustomRPM) return CustomRPMValue;
    return old_get_sensorRpm(instance);
}

float AddFrictionTorque_Hook(void *instance, float frictionTorque) {
    if (instance != NULL && IsSuperBrake) return 50000.0f; // Rem Pakem
    return old_AddFrictionTorque(instance, frictionTorque);
}

bool get_Tcs_Hook(void *instance) {
    if (instance != NULL && IsSuperTraction) return true; // Anti-Slip
    return old_get_Tcs(instance);
}

// --- TRAILER HANDLING (GANDENGAN) ---
void SetStability_Hook(void *instance) { old_SetStability(instance); }
float GetTrailerDamage_Hook() { if (IsNoTrailerDamage) return 0.0f; return old_GetTrailerDamage(); }

// ==================== [ LOGIKA AUTO CONNECT (UPDATE) ] ====================

// --- 1. AUTO CONNECT TRAILER (Update) ---
void (*old_TrailerUpdate)(void *instance);
void TrailerUpdate(void *instance) {
    if (instance != NULL && IsInstantConnect) {
        typedef void (*Connect_t)(void *);
        static Connect_t ConnectFunc = (Connect_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("TrailerController"), OBFUSCATE("Connect"), 0);
        if (ConnectFunc) ConnectFunc(instance);
        IsInstantConnect = false; 
    }
    old_TrailerUpdate(instance);
}

// --- 2. AUTO CONNECT FUEL (Numpang di Truck Update ⛽) ---
void (*old_TruckUpdate)(void *instance);
void TruckUpdate_Hook(void *instance) {
    if (instance != NULL) {
        if (IsNoFuelConsum) {
            SetFieldFloat(instance, OBFUSCATE("fuel"), 100.0f);
            SetFieldFloat(instance, OBFUSCATE("fuelLeft"), 100.0f);
        }
    }
    old_TruckUpdate(instance);
}

// --- 3. AUTO CONNECT JOB (Numpang di Job Update 🏁) ---
void (*old_JobUpdate)(void *instance);
void JobUpdate_Hook(void *instance) {
    if (instance != NULL) {
        if (IsInstantFinish) {
            SetFieldFloat(instance, OBFUSCATE("Km"), 0.0f);
        }
    }
    old_JobUpdate(instance);
}


// --- UNLOCK ALL CARGO & CONTAINERS ---
bool UnlockTrailers_Hook() { 
    if (IsUnlockEverything) return true; // Status beli trailer: Terbuka
    return old_UnlockTrailers(); 
}

// --- UNLOCK ALL TRUCKS & CHASSIS ---
bool TruckPurchased_Hook(void *instance) {
    if (IsUnlockEverything) return true; // Status beli truk: Sudah dibeli/Terbuka
    return old_TruckPurchased(instance);
}
