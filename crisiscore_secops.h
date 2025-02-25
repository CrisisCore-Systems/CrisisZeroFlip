#ifndef CRISISCORE_SECOPS_H
#define CRISISCORE_SECOPS_H

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <storage/storage.h>
#include <notification/notification_messages.h>

/**
 * CrisisCore-SecOps Tactical Recon Header
 * Enterprise-grade scanning utility for security operations
 * and tactical reconnaissance scenarios.
 * 
 * Version: 1.0.0
 * Copyright (c) 2025 SecOps Enterprises
 * Licensed under proprietary license
 */

// Application Metadata
#define APP_NAME "CrisisCore"
#define APP_VERSION "1.0.0"

// File Paths
#define LOG_FILENAME "/ext/crisiscore_recon.log"
#define CONFIG_FILENAME "/ext/crisiscore_config.txt"
#define ENCRYPTION_KEY_FILE "/ext/crisiscore_key.bin"

// Scan Parameters
#define SCAN_DURATION_DEFAULT 30000 // Default scan duration (ms)
#define SCAN_INTERVAL_DEFAULT 300   // Interval between scans (ms)
#define LOG_BUFFER_SIZE 10    // Number of log entries to buffer before writing
#define LOG_ENTRY_MAX_SIZE 256   // Maximum size of a log entry

// Authentication
#define PIN_LENGTH 6     // Length of PIN code
#define MAX_PIN_ATTEMPTS 3     // Maximum attempts before temporary lockout

// Region Codes
typedef enum {
    REGION_US = 0,
    REGION_EU,
    REGION_JP,
    REGION_CN,
    REGION_AU,
    REGION_UNKNOWN
} RegionCode;

// Module Types
typedef enum {
    MODULE_SUBGHZ = 0,
    MODULE_NFC,
    MODULE_WIFI,
    MODULE_BT,
    MODULE_COUNT
} ModuleType;

// Error Codes
typedef enum {
    ERROR_NONE = 0,
    ERROR_INIT_FAILED,
    ERROR_MALLOC_FAILED,
    ERROR_FILE_OPEN_FAILED,
    ERROR_FILE_READ_FAILED,
    ERROR_FILE_WRITE_FAILED,
    ERROR_HARDWARE_INIT_FAILED,
    ERROR_AUTHENTICATION_FAILED,
    ERROR_SCAN_FAILED,
    ERROR_INVALID_PARAMETER,
    ERROR_UNKNOWN
} ErrorCode;

// Log Levels
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_CRITICAL
} LogLevel;

// Log Entry Buffer
typedef struct {
    char entries[LOG_BUFFER_SIZE][LOG_ENTRY_MAX_SIZE];
    uint8_t count;
    FuriMutex* mutex;
} LogBuffer;

// Scan State and Configuration
typedef struct {
    bool initialized;
    bool authenticated;
    bool scan_active;
    bool exit_requested;
    uint32_t scan_duration;
    uint32_t scan_interval;
    RegionCode region;
    uint8_t pin_code[PIN_LENGTH];
    LogLevel log_level;
    bool module_enabled[MODULE_COUNT];
    FuriMutex* state_mutex;
    Storage* storage;
    NotificationApp* notifications;
    LogBuffer* log_buffer;
} ScanState;

// Core Application Functions
ErrorCode app_init(ScanState** state);
void app_free(ScanState* state);
void app_run(ScanState* state);
ErrorCode app_authenticate(ScanState* state, const uint8_t* pin);

// Configuration Management
ErrorCode load_config(ScanState* state);
ErrorCode save_config(ScanState* state);
ErrorCode reset_config_to_defaults(ScanState* state);
RegionCode detect_region(void);

// Logging Functions
ErrorCode log_init(ScanState* state);
ErrorCode log_message(ScanState* state, LogLevel level, const char* module, const char* format, ...);
void log_flush(ScanState* state, bool force);

// Scanning Modules
ErrorCode subghz_scan_range(ScanState* state, uint32_t freq_start, uint32_t freq_end, uint32_t freq_step);
ErrorCode nfc_scan(ScanState* state);

#endif // CRISISCORE_SECOPS_H
