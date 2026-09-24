#pragma once

/**
 * @brief Marks a spot where you have to write the code yourself
 *
 * Expands to a static_assert that always fails, so the compiler stops right here and prints the
 * message. Write the missing code, then delete the STEGO_TODO line. LEARNING.md has hints for each id.
 */
#define STEGO_TODO(id, what) static_assert(false, "TODO(" id "): " what " (see LEARNING.md)")
