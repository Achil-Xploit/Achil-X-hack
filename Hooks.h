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


// ==================== [ TOGGLES ] ====================
inline bool IsIntExample2 = false;   // Unlimited Gold
inline bool IsBalanceMod = false;    // Unlimited Balance
inline bool IsSuperStable = false;   // Trailer Stability
//inline bool IsLightTrailer = false;  // Anti-Weight Cargo
inline bool IsInstantConnect = false; // Instant Connect Trailer
inline bool IsInstantTorque = false;
inline bool IsNoFuelConsum = false;
inline bool IsCustomRPM = false;
inline bool IsSuperBrake = false;
inline bool IsSuperTraction = false;
inline bool IsNoAds = false;
inline bool IsUnlockDrivers = false;

// ==================== [ 1. UNLIMITED GOLD ] ====================
int (*old_IntExample2)(); 
int IntExample2() {
    if (IsIntExample2) return 999999999;
    return old_IntExample2();
}

// ==================== [ 2. UNLIMITED BALANCE ] ====================
inline float (*old_Balance)(); 
inline float get_Balance_Hook() {
    if (IsBalanceMod) return 999999999.0f;
    return (old_Balance != nullptr) ? old_Balance() : 0.0f;
}

// ==================== [ 3. TRAILER STABILITY ] ====================
void (*old_SetStability)(void *instance);
void SetStability_Hook(void *instance) {
    old_SetStability(instance);
}

// ==================== [ 4. ANTI-WEIGHT CARGO ] ====================
/*void (*old_SetSpringRate)(void *instance, float weight);
void SetSpringRate_Hook(void *instance, float weight) {
    if (instance != NULL && IsLightTrailer) weight = 0.05f;
    old_SetSpringRate(instance, weight);
}*/

// ==================== [ 5. INSTANT CONNECT ] ====================
void (*old_TrailerUpdate)(void *instance);
void TrailerUpdate(void *instance) {
    if (instance != NULL && IsInstantConnect) {
        // Kita panggil fungsi Connect lewat Il2Cpp Method
        typedef void (*Connect_t)(void *);
        static Connect_t ConnectFunc = (Connect_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("TrailerController"), OBFUSCATE("Connect"), 0);
        
        if (ConnectFunc) ConnectFunc(instance);
        IsInstantConnect = false; // Langsung OFF setelah nyambung
    }
    old_TrailerUpdate(instance);
}

// 2. POINTER FUNGSI (TARUH DI SINI SAYANG 💋)
// Ini harus ada supaya compiler kenal siapa itu old_GetMaxPower dll.
inline float (*old_GetMaxPower)(void *instance, float rpm);
inline float (*old_ClampTorque)(void *instance, float powerTorque, float rpm);
inline float (*old_Friction)(void *instance, float rpm);

// 3. LOGIKA HOOK (Fungsi Sulap Kita)
float GetMaxPower_Hook(void *instance, float rpm) {
    if (instance != NULL && IsInstantTorque) return 9999999.0f;
    return old_GetMaxPower(instance, rpm);
}

float ClampTorque_Hook(void *instance, float powerTorque, float rpm) {
    if (instance != NULL && IsInstantTorque) return 999999.0f; // Versi Monster! 🚀
    return old_ClampTorque(instance, powerTorque, rpm);
}

float Friction_Hook(void *instance, float rpm) {
    if (instance != NULL && IsInstantTorque) return 0.0f;
    return old_Friction(instance, rpm);
}



// --- Hook Perhitungan Bensin (1 parameter: rpm/speed) ---
float (*old_SpecificFuel)(void *instance, float rpm);
float SpecificFuel_Hook(void *instance, float rpm) {
    if (IsNoFuelConsum) return 0.0f; // Konsumsi bensin jadi nol!
    return old_SpecificFuel(instance, rpm);
}

float (*old_InstantFuel)(void *instance, float speed);
float InstantFuel_Hook(void *instance, float speed) {
    if (IsNoFuelConsum) return 0.0f; // Konsumsi instan jadi nol!
    return old_InstantFuel(instance, speed);
}

float (*old_MaxFuelRate)(void *instance, float rpm);
float MaxFuelRate_Hook(void *instance, float rpm) {
    if (IsNoFuelConsum) return 0.0f; // Batas maksimal bensin juga nol!
    return old_MaxFuelRate(instance, rpm);
}


// --- Toggles & Values ---
inline float CustomRPMValue = 1000.0f; // Nilai awal RPM

// --- Hook get_sensorRpm ---
float (*old_get_sensorRpm)(void *instance);
float get_sensorRpm_Hook(void *instance) {
    if (instance != NULL && IsCustomRPM) {
        return CustomRPMValue; // Kembalikan nilai dari slider
    }
    return old_get_sensorRpm(instance);
}

// --- Hook AddFrictionTorque (Tipe Single = float) ---
float (*old_AddFrictionTorque)(void *instance, float frictionTorque);
float AddFrictionTorque_Hook(void *instance, float frictionTorque) {
    if (instance != NULL && IsSuperBrake) {
        // Jika sedang mengerem, kita lipat gandakan hambatannya!
        return old_AddFrictionTorque(instance, 50000.0f); 
    }
    return old_AddFrictionTorque(instance, frictionTorque);
}

bool (*old_get_Tcs)(void *instance);
bool get_Tcs_Hook(void *instance) {
    if (instance != NULL && IsSuperTraction) {
        return true; // Paksa TCS aktif terus biar ban dapet grip maksimal
    }
    return old_get_Tcs(instance);
}

// Hook NoAds Status
bool (*old_get_NoAds)(void *instance);
bool get_NoAds_Hook(void *instance) {
    if (IsNoAds) return true; // Status: Sudah hapus iklan
    return old_get_NoAds(instance);
}

// Hook Izin Iklan (Banner & AppOpen)
bool (*old_CanShowAds)(void *instance);
bool CanShowAds_Hook(void *instance) {
    if (IsNoAds) return false; // Jangan izinkan tampilkan iklan
    return old_CanShowAds(instance);
}

// ==================== [ TOGGLES ] ====================
inline bool IsSuperJob = false;

// ==================== [ HOOKS ] ====================

// 1. Hook Bonus Gaji saat Ambil Kerja (Class: JobManager)
void (*old_GetJob)(void *instance, float bonusMultiplier);
void GetJob_Hook(void *instance, float bonusMultiplier) {
    if (instance != NULL && IsSuperJob) {
        bonusMultiplier = 1000.0f; // Bonus 1000x lipat sayang 💋
    }
    old_GetJob(instance, bonusMultiplier);
}

// 2. Hook Gaji & XP saat Selesai Kerja (Class: JobComplete)
void (*old_JobCompleted)(void *instance, bool skipped);
void JobCompleted_Hook(void *instance, bool skipped) {
    if (instance != NULL && IsSuperJob) {
        // Offset 0x48 = _income, 0x4c = _totalPenalties, 0x70 = xpToAdd
        *(float *)((uintptr_t)instance + 0x48) = 999999.0f; // Gaji Max
        *(float *)((uintptr_t)instance + 0x4c) = 0.0f;      // No Potongan
        *(float *)((uintptr_t)instance + 0x70) = 50000.0f;  // XP Gede
    }
    old_JobCompleted(instance, skipped);
}

// --- Toggles Baru ---
inline bool IsMaxLevel = false;

// Hook Level (Static - Tanpa instance)
int (*old_GetLevel)();
int GetLevel_Hook() {
    if (IsMaxLevel) {
        return 999999999; // Set level sesukamu, misal 100
    }
    return old_GetLevel();
}

// Hook XP (Static - Tanpa instance)
int (*old_GetXP)();
int GetXP_Hook() {
    if (IsMaxLevel) {
        return 999999999; // Set XP biar bar-nya penuh terus
    }
    return old_GetXP();
}

// --- Toggles Baru ---
inline bool IsUnlockAllTrailer = false;
inline bool IsNoTrailerDamage = false;

// Hook No Ads (Status AdFree)
bool (*old_AdFree)();
bool AdFree_Hook() {
    if (IsNoAds) return true;
    return old_AdFree();
}

// Hook Unlock Trailers (Bisa bawa semua jenis muatan)
bool (*old_UnlockTrailers)();
bool UnlockTrailers_Hook() {
    if (IsUnlockAllTrailer) return true;
    return old_UnlockTrailers();
}

// Hook No Trailer Damage (Biar muatan nggak rusak)
float (*old_GetTrailerDamage)();
float GetTrailerDamage_Hook() {
    if (IsNoTrailerDamage) return 0.0f; // Kerusakan selalu 0
    return old_GetTrailerDamage();
}

// Hook untuk Unlock Slot Supir (Class: DriverManager)
void (*old_SelectNewDriver)(void *instance, int id);
void SelectNewDriver_Hook(void *instance, int id) {
    if (instance != NULL && IsUnlockDrivers) {
        // Kita paksa jumlah supir (driverCount) di offset 0x38 jadi maksimal (misal 50)
        *(int *)((uintptr_t)instance + 0x38) = 50; 
    }
    old_SelectNewDriver(instance, id);
}

// Hook untuk Memastikan Supir Bisa Direkrut (HireNewDriver)
void (*old_HireNewDriver)(void *instance);
void HireNewDriver_Hook(void *instance) {
    if (instance != NULL && IsUnlockDrivers) {
        // Logika tambahan jika diperlukan saat klik tombol Hire
    }
    old_HireNewDriver(instance);
}
