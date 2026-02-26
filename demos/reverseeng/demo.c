// hidden_password.c
// Build (Linux/macOS):  gcc -O0 -fno-pie -no-pie hidden_password.c -o hidden_password
// Build (Windows, MinGW): gcc -O0 hidden_password.c -o hidden_password.exe
// Tip: -O0 keeps it easy to read in Ghidra.

#include <stdio.h>
#include <string.h>
#include <stdint.h>

static void decode_xor(char *dst, const uint8_t *src, size_t n, uint8_t key)
{
    for (size_t i = 0; i < n; i++)
    {
        dst[i] = (char)(src[i] ^ key);
    }
    dst[n] = '\0';
}

static int check_password(const char *user)
{
    // "opensesame" XOR 0x5A -> stored as bytes to avoid plain-text string in binary.
    static const uint8_t enc[] = {
        0x35, 0x2a, 0x3f, 0x34, 0x29, 0x3f, 0x29, 0x3b, 0x37, 0x3f};
    char real[32];
    decode_xor(real, enc, sizeof(enc), 0x5A);

    // Small decoy branch to explore in Ghidra:
    if (user[0] == 'z')
    {
        // Not used for real validation, just noise.
        return 0;
    }

    return strcmp(user, real) == 0;
}

static void reveal_flag(const char *user)
{
    // A “flag-like” output, derived from input so people see data flow.
    // This is *not* cryptography—just something to trace in the decompiler.
    unsigned sum = 0;
    for (size_t i = 0; user[i]; i++)
        sum += (unsigned char)user[i];

    printf("Access granted!\n");
    printf("FLAG{demo_%04X_%s}\n", (sum & 0xFFFFu), "ghidra_intro");
}

int main(void)
{
    char buf[64];

    puts("== Demo Crackme (for Ghidra) ==");
    puts("Hint: the password is not stored as plain text.");
    puts("Enter password:");

    if (!fgets(buf, sizeof(buf), stdin))
    {
        puts("Input error.");
        return 1;
    }

    // trim newline
    buf[strcspn(buf, "\r\n")] = 0;

    // Another decoy string for string/xref practice:
    if (strlen(buf) == 42)
    {
        puts("Wow. That is... oddly specific.");
    }

    if (check_password(buf))
    {
        reveal_flag(buf);
        return 0;
    }
    else
    {
        puts("Access denied.");
        return 1;
    }
}
