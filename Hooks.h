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

// ========================================================
// [ BAGIAN 1: SEMUA TOMBOL / TOGGLES ]
// ========================================================
// --- [ FITUR EQUIPMENT MODS ] ---
inline bool IsGodHealth = false;      // HP Abadi
inline bool IsMaxArmor = false;       // Armor Kebal
inline bool IsSuperDamage = false;    // Satu Kali Pukul Mati
inline bool IsGodSpeed = false;       // Saklar On/Off
inline float GodSpeedValue = 1.0f;    // Nilai default (1.0 = Normal)
inline bool IsNoCooldown = false;     // Skill Tanpa Delay
inline bool IsInstantApply = false;   // Pakai Item Langsung Jadi
inline bool IsInfiniteDuration = false; // Durasi Efek Abadi
inline bool IsMaxVehicleHP = false;   // Kendaraan Gak Bisa Hancur
// --- [ TOGGLES WEAPON ] ---
inline bool IsFastFire = false;       // Rapid Fire
inline bool IsInstantReload = false;   // No Reload Time
inline bool IsOneTapHead = false;      // Super Headshot
inline bool IsUnlimitedAmmo = false;   // Amunisi Abadi
inline bool IsNoRecoil = false; // No Recoil & No Spread
inline bool IsFarRange = false;        // Jarak Tembak Jauh
// --- [ TOGGLES FISIK ] ---
inline bool IsGhostMode = false;   
inline float GhostRadiusValue = 0.5f; // Normal radius biasanya 0.5
inline float GhostHeightValue = 2.0f; // Normal height biasanya 2.0
inline bool IsAirJump = false;    // Lompat di udara


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
// --- [ WADAH FUNGSI ASLI EQUIPMENT ] ---
static uint32_t (*old_Health)(void *instance);
static uint32_t (*old_Armor)(void *instance);
static int32_t (*old_MaxHealth)(void *instance);
static float (*old_DamageMultiplier)(void *instance);
static float (*old_SpeedMultiplier)(void *instance);
static float (*old_VehicleSpeedMultiplier)(void *instance);
static int32_t (*old_Cooldown)(void *instance);
static float (*old_ApplyTime)(void *instance);
static uint32_t (*old_Duration)(void *instance);
static int32_t (*old_RecursiveHealth)(void *instance);
static int32_t (*old_Delay)(void *instance);
static uint32_t (*old_Repeats)(void *instance);
static int32_t (*old_VehicleHealth)(void *instance);
// --- [ POINTERS ORIGINAL WEAPON ] ---
static int32_t (*old_FireRateMs)(void *instance);
static int32_t (*old_ReloadTimeMs)(void *instance);
static float (*old_HeadFactor)(void *instance);
static float (*old_Range)(void *instance);
static float (*old_SpreadAngle)(void *instance);
static int32_t (*old_SpreadCount)(void *instance);
static bool (*old_HasUnlimitedAmmo)(void *instance);
static float (*old_FireDistance)(void *instance);
static bool (*old_isGrounded)(void *instance);
static float (*old_radius)(void *instance);
static float (*old_height)(void *instance);


// ========================================================
// [ BAGIAN 3: LOGIKA FITUR 1 (ECONOMY, ADS, & JOBS) ]
// ========================================================
// --- HEALTH & ARMOR ---
uint32_t h_Health(void *instance) { 
    if (IsGodHealth) return 999999; 
    return old_Health(instance); 
}
uint32_t h_Armor(void *instance) { 
    if (IsMaxArmor) return 999999; 
    return old_Armor(instance); 
}
int32_t h_MaxHealth(void *instance) { 
    if (IsGodHealth) return 999999; 
    return old_MaxHealth(instance); 
}

// --- MULTIPLIERS (DAMAGE & SPEED) ---
float h_DamageMultiplier(void *instance) { 
    if (IsSuperDamage) return 100.0f; 
    return old_DamageMultiplier(instance); 
}
float h_SpeedMultiplier(void *instance) { 
    if (IsGodSpeed) return GodSpeedValue; // Mengikuti nilai dari slider
    return old_SpeedMultiplier(instance); 
}

float h_VehicleSpeedMultiplier(void *instance) { 
    if (IsGodSpeed) return GodSpeedValue; // Mengikuti nilai dari slider
    return old_VehicleSpeedMultiplier(instance); 
}
// --- MEKANIK & TIME ---
int32_t h_Cooldown(void *instance) { 
    if (IsNoCooldown) return 0; 
    return old_Cooldown(instance); 
}
float h_ApplyTime(void *instance) { 
    if (IsInstantApply) return 0.0f; 
    return old_ApplyTime(instance); 
}
uint32_t h_Duration(void *instance) { 
    if (IsInfiniteDuration) return 999999; 
    return old_Duration(instance); 
}

// --- REGEN / RECURSIVE ---
int32_t h_RecursiveHealth(void *instance) { 
    if (IsGodHealth) return 10000; 
    return old_RecursiveHealth(instance); 
}
int32_t h_Delay(void *instance) { 
    if (IsGodHealth) return 0; 
    return old_Delay(instance); 
}
uint32_t h_Repeats(void *instance) { 
    if (IsGodHealth) return 999; 
    return old_Repeats(instance); 
}

// --- KENDARAAN ---
int32_t h_VehicleHealth(void *instance) { 
    if (IsMaxVehicleHP) return 999999; 
    return old_VehicleHealth(instance); 
}

// --- [ LOGIKA FITUR WEAPON ] ---

// Tembakan Cepat (Semakin kecil ms, semakin cepat)
int32_t h_FireRateMs(void *instance) {
    if (IsFastFire) return 0; // 0ms = Super Fast
    return old_FireRateMs(instance);
}

// Reload Instan
int32_t h_ReloadTimeMs(void *instance) {
    if (IsInstantReload) return 0; 
    return old_ReloadTimeMs(instance);
}

// Damage Headshot Gila
float h_HeadFactor(void *instance) {
    if (IsOneTapHead) return 100.0f; 
    return old_HeadFactor(instance);
}

// Jarak Tembak & Range
float h_Range(void *instance) {
    if (IsFarRange) return 999.0f;
    return old_Range(instance);
}

// Akurasi (No Spread)
float h_SpreadAngle(void *instance) {
    if (IsNoRecoil) return 0.0f; // 0 = Peluru lurus lurus terus kayak laser!
    return old_SpreadAngle(instance);
}

int32_t h_SpreadCount(void *instance) {
    if (IsNoRecoil) return 1; // Pastikan peluru fokus di 1 titik
    return old_SpreadCount(instance);
}

// Amunisi Tak Terbatas
bool h_HasUnlimitedAmmo(void *instance) {
    if (IsUnlimitedAmmo) return true;
    return old_HasUnlimitedAmmo(instance);
}

// --- [ LOGIKA HOOK ] ---

// Mengatur lebar badan (Radius)
float h_radius(void *instance) {
    if (instance != NULL && IsGhostMode) {
        return GhostRadiusValue; // Ikut nilai slider
    }
    return old_radius(instance);
}

// Mengatur tinggi badan (Height)
float h_height(void *instance) {
    if (instance != NULL && IsGhostMode) {
        return GhostHeightValue; // Ikut nilai slider
    }
    return old_height(instance);
}

bool h_isGrounded(void *instance) {
    if (instance != NULL && IsAirJump) {
        return true; 
    }
    return old_isGrounded(instance);
}

