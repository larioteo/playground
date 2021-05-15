module;

#include <memory>

#include "Ultra/Core.h"

export module Ultra.Debug.Memory;

import Ultra.Log;

// Data
struct AllocationMetrics {
    std::size_t TotalAllocated = 0;
    std::size_t TotalFreed = 0;

    std::size_t CurrentUsage() { return TotalAllocated - TotalFreed; }
} static iAllocationMetrics;

// Overrides
export void *operator new(size_t size) {
    iAllocationMetrics.TotalAllocated += size;
    return malloc(size);
}

export void operator delete(void *memory, size_t const size) noexcept {
    iAllocationMetrics.TotalFreed += size;
    free(memory);
}

// Verification
export void VerifyMemoryUsage() {
    Ultra::logger << "Memory Usage: " << iAllocationMetrics.CurrentUsage() << " bytes\n";
}
