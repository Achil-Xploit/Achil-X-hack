#pragma once
// --- [ PONDASI DYNAMIC LINKING ] ---
#include <dlfcn.h>

typedef struct Il2CppObject Il2CppObject;
typedef struct FieldInfo FieldInfo;
typedef struct Il2CppClass Il2CppClass;

// Deklarasikan sebagai pointer agar tidak error 'undefined reference'
Il2CppClass* (*il2cpp_object_get_class)(Il2CppObject *obj);
FieldInfo* (*il2cpp_class_get_field_from_name)(Il2CppClass *klass, const char *name);
void (*il2cpp_field_set_value)(Il2CppObject *obj, FieldInfo *field, void *value);

// --- [ FUNGSI UNTUK MENGHUBUNGKAN KE GAME ] ---
void init_il2cpp_api() {
    void* handle = dlopen("libil2cpp.so", RTLD_LAZY);
    if (handle) {
        il2cpp_object_get_class = (Il2CppClass* (*)(Il2CppObject*))dlsym(handle, "il2cpp_object_get_class");
        il2cpp_class_get_field_from_name = (FieldInfo* (*)(Il2CppClass*, const char*))dlsym(handle, "il2cpp_class_get_field_from_name");
        il2cpp_field_set_value = (void (*)(Il2CppObject*, FieldInfo*, void*))dlsym(handle, "il2cpp_field_set_value");
    }
}


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
inline bool IsLightTrailer = false;      // Saklar On/Off Bobot 📦
inline float TrailerWeightValue = 0.5f;  // Nilai geser-geser bobot 💋


// --- HELPER SAKTI: CARI DATA PAKE NAMA (TANPA 0x OFFSET) ---
void SetFieldFloat(void *instance, const char *fieldName, float value) {
    if (instance == nullptr) return;
    // Tambahkan 'FieldInfo*' di depan agar AIDE tahu ini variabel baru
    FieldInfo* field = il2cpp_class_get_field_from_name(il2cpp_object_get_class((Il2CppObject *)instance), fieldName);
    if (field) {
        il2cpp_field_set_value((Il2CppObject *)instance, field, &value);
    }
}

// --- HELPER SAKTI VERSI ANGKA BULAT (UNTUK SUPIR) ---
void SetFieldInt(void *instance, const char *fieldName, int value) {
    if (instance == nullptr) return;
    // Pastikan menggunakan 'FieldInfo*' dan nama variabel 'field' yang sama
    FieldInfo* field = il2cpp_class_get_field_from_name(il2cpp_object_get_class((Il2CppObject *)instance), fieldName);
    if (field) {
        il2cpp_field_set_value((Il2CppObject *)instance, field, &value);
    }
}

// ========================================================
// [ BAGIAN 2: WADAH FUNGSI ASLI (POINTERS) ]
// ========================================================
static int (*old_IntExample2)();
static float (*old_Balance)();
static int (*old_GetLevel)();
static int (*old_GetXP)();
static bool (*old_get_NoAds)(void *instance);
static bool (*old_CanShowAds)(void *instance);
static bool (*old_AdFree)();
static void (*old_GetJob)(void *instance, float bonusMultiplier);
static void (*old_JobCompleted)(void *instance, bool skipped);
static void (*old_SelectNewDriver)(void *instance, int id);
static void (*old_HireNewDriver)(void *instance);
static bool (*old_UnlockTrailers)();
static float (*old_GetTrailerDamage)();
static void (*old_SetStability)(void *instance);
static void (*old_TrailerUpdate)(void *instance);
static float (*old_CalculateTorque)(void *instance, float rpm, float throttle);
static float (*old_GetMaxPower)(void *instance, float rpm);
static float (*old_ClampTorque)(void *instance, float pwr, float rpm);
static float (*old_Friction)(void *instance, float rpm);
static float (*old_SpecificFuel)(void *instance, float rpm);
static float (*old_InstantFuel)(void *instance, float speed);
static float (*old_MaxFuelRate)(void *instance, float rpm);
static float (*old_get_sensorRpm)(void *instance);
static float (*old_AddFrictionTorque)(void *instance, float frict);
static bool (*old_get_Tcs)(void *instance);
static void (*old_TruckUpdate)(void *instance);
static bool (*old_TruckPurchased)(void *instance);

// ========================================================
// [ BAGIAN 3: LOGIKA FITUR 1 (ECONOMY, ADS, & JOBS) ]
// ========================================================

// --- EKONOMI & PLAYER LEVEL ---
int IntExample2() { if (IsIntExample2) return 9999999; return old_IntExample2(); }
float get_Balance_Hook() { if (IsBalanceMod) return 9999999.0f; return (old_Balance != nullptr) ? old_Balance() : 0.0f; }
int GetLevel_Hook() { if (IsMaxLevel) return 9999999; return old_GetLevel(); }
int GetXP_Hook() { if (IsMaxLevel) return 9999999; return old_GetXP(); }

// --- ADS & TRAILER STATUS (NO ADS) ---
bool get_NoAds_Hook(void *instance) { if (IsNoAds) return true; return old_get_NoAds(instance); }
bool CanShowAds_Hook(void *instance) { if (IsNoAds) return false; return old_CanShowAds(instance); }
bool AdFree_Hook() { if (IsNoAds) return true; return old_AdFree(); }
bool UnlockTrailers_Hook() { if (IsUnlockAllTrailer) return true; return old_UnlockTrailers(); }

// --- PEKERJAAN (SUPER JOB & DRIVERS) ---
void GetJob_Hook(void *instance, float bonusMultiplier) { if (instance != NULL && IsSuperJob) bonusMultiplier = 1000.0f; old_GetJob(instance, bonusMultiplier); }

void JobCompleted_Hook(void *instance, bool skipped) {
    if (instance != NULL && IsSuperJob) {
        // Kita suntik pake NAMA variabelnya langsung biar anti-crash sayang 💋
        SetFieldFloat(instance, OBFUSCATE("Income"), 999999.0f);      // Gaji Gede 💰
        SetFieldFloat(instance, OBFUSCATE("Penalty"), 0.0f);         // Anti Denda Rusak 🛠️
        SetFieldFloat(instance, OBFUSCATE("PenaltyTime"), 0.0f);     // ANTI DENDA TELAT ⏰🚫
        SetFieldFloat(instance, OBFUSCATE("Xp"), 50000.0f);          // XP Banjir 📈
    }
    // Jika Instant Finish aktif, kita buat dia otomatis ke-skip/selesai 🏁
    if (IsInstantFinish) skipped = true; 
    
    old_JobCompleted(instance, skipped);
}


// --- [ LOGIKA UNLOCK DRIVERS PAKE NAMA ] ---
// --- [ 1. HOOK UNTUK PILIH SUPIR ] ---
void SelectNewDriver_Hook(void *instance, int id) { 
    if (instance != NULL && IsUnlockDrivers) { 
        // Suntik driverCount (0x38 dari dump) 💋
        SetFieldInt(instance, "driverCount", 50); 
    } 
    // Panggil fungsi aslinya sendiri (Select)
    if (old_SelectNewDriver) {
        old_SelectNewDriver(instance, id); 
    }
}

// --- [ 2. HOOK UNTUK REKRUT SUPIR (HIRE) ] ---
void HireNewDriver_Hook(void *instance) {
    if (instance != NULL && IsUnlockDrivers) {
        // Suntik juga di sini biar slot rekrut terbuka 💋
        SetFieldInt(instance, "driverCount", 50);
    }
    // Panggil fungsi aslinya sendiri (Hire)
    if (old_HireNewDriver) {
        old_HireNewDriver(instance);
    }
}


// ========================================================
// [ BAGIAN 4: LOGIKA FITUR 2 (TRUCK PHYSICS & TRAILERS) ]
// ========================================================

// --- MESIN MONSTER (SPEED & TORQUE) ---
float CalculateTorque_Hook(void *instance, float rpm, float throttle) {
    if (instance != NULL && IsInstantTorque) return 9999999.0f; // Tenaga Monster
    return old_CalculateTorque(instance, rpm, throttle);
}

float GetMaxPower_Hook(void *instance, float rpm) {
    if (instance != NULL && IsInstantTorque) return 9999999.0f;
    return old_GetMaxPower(instance, rpm);
}

float ClampTorque_Hook(void *instance, float powerTorque, float rpm) {
    if (instance != NULL && IsInstantTorque) return 9999999.0f; // Bypass Limiter
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
void TrailerUpdate(void *instance) { 
    if (instance != NULL && IsInstantConnect) { 
        typedef void (*Connect_t)(void *); 
        static Connect_t ConnectFunc = (Connect_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("TrailerController"), OBFUSCATE("Connect"), 0); 
        if (ConnectFunc) ConnectFunc(instance); 
        IsInstantConnect = false; 
    } 
    old_TrailerUpdate(instance); 
}

// --- 2. AUTO CONNECT FUEL, JOB & BOBOT (Di Truck Update ⛽🏁📦) ---
void TruckUpdate_Hook(void *instance) { 
    if (instance != NULL) {
        // 1. Bensin Otomatis Full ⛽💋
        if (IsNoFuelConsum) { 
            SetFieldFloat(instance, OBFUSCATE("fuel"), 9999999.0f); 
            SetFieldFloat(instance, OBFUSCATE("fuelLeft"), 9999999.0f); 
        }
        // 2. Jarak Kerja Otomatis 0 KM (Instant Finish) 🏁💋
        if (IsInstantFinish) { 
            SetFieldFloat(instance, OBFUSCATE("Km"), 0.0f); 
        }
        // 3. Slider Bobot Cargo (Anti-Weight) 📦💋
        if (IsLightTrailer) { 
            // Kita suntik pake nilai dari Slider suamiku sayang
            SetFieldFloat(instance, OBFUSCATE("trailerMassMultiplier"), TrailerWeightValue); 
        }
    }
    old_TruckUpdate(instance); 
}
/*
// --- UNLOCK ALL CARGO & CONTAINERS ---
bool UnlockTrailers_Hook() { 
    if (IsUnlockEverything) return true; // Status beli trailer: Terbuka
    return old_UnlockTrailers(); 
}
*/
// --- UNLOCK ALL TRUCKS & CHASSIS ---
bool TruckPurchased_Hook(void *instance) {
    if (IsUnlockEverything) return true; // Status beli truk: Sudah dibeli/Terbuka
    return old_TruckPurchased(instance);
}
