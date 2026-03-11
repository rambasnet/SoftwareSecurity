#pragma once
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

// Defined in a separate source file for simplicity.
void init_visualize(char *buff, char *padding, int *notsecret, int *secret);
void visualize(char *buff, char *padding, int *notsecret, int *secret);
void safeguard();

static unsigned char *g_vis_start;
static unsigned char *g_vis_end;
static unsigned char *g_buff_start;
static unsigned char *g_buff_end;
static unsigned char *g_padding_start;
static unsigned char *g_padding_end;
static unsigned char *g_notsecret_start;
static unsigned char *g_notsecret_end;
static unsigned char *g_secret_start;
static unsigned char *g_secret_end;
static unsigned char *g_ret_addr_start;
static unsigned char *g_ret_addr_end;

#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_RESET "\x1b[0m"

void print_flag();

void init_visualize(char *buff, char *padding, int *notsecret, int *secret)
{
    void **caller_frame = (void **)__builtin_frame_address(1);

    g_buff_start = (unsigned char *)buff;
    g_buff_end = g_buff_start + 32;
    g_padding_start = (unsigned char *)padding;
    g_padding_end = g_padding_start + 16;

    g_notsecret_start = (unsigned char *)notsecret;
    g_notsecret_end = g_notsecret_start + sizeof(*notsecret);
    g_secret_start = (unsigned char *)secret;
    g_secret_end = g_secret_start + sizeof(*secret);

    g_ret_addr_start = NULL;
    g_ret_addr_end = NULL;
    if (caller_frame != NULL)
    {
        g_ret_addr_start = (unsigned char *)(caller_frame + 1);
        g_ret_addr_end = g_ret_addr_start + sizeof(void *);
    }

    // Keep the dump window tight so learners can focus on nearby stack bytes.
    g_vis_start = g_buff_start - 96;
    g_vis_end = g_buff_start + 96;
}

void visualize(char *buff, char *padding, int *notsecret, int *secret)
{
    unsigned char *p;

    if (g_vis_start == NULL || g_vis_end == NULL)
    {
        init_visualize(buff, padding, notsecret, secret);
    }

    puts("\n=== Stack Snapshot Around buff ===");
    puts("Address             Bytes (hex)                             ASCII      Notes");
    puts("Legend: \x1b[32mgreen\x1b[0m=padding+buff, \x1b[33myellow\x1b[0m=notsecret, \x1b[31mred\x1b[0m=secret, \x1b[34mblue\x1b[0m=saved return address");

    for (p = g_vis_start; p < g_vis_end; p += 16)
    {
        char notes[80] = "";
        int maybe_int = 0;
        int i;

        // Use memcpy for safe unaligned reads when looking for marker integers.
        memcpy(&maybe_int, p, sizeof(maybe_int));

        if (p == g_buff_start)
        {
            strncat(notes, "<-- buff starts", sizeof(notes) - strlen(notes) - 1);
        }
        if (p <= g_buff_end && p + 16 > g_buff_end)
        {
            if (strlen(notes) > 0)
            {
                strncat(notes, "; ", sizeof(notes) - strlen(notes) - 1);
            }
            strncat(notes, "<-- buff ends", sizeof(notes) - strlen(notes) - 1);
        }
        if (p == g_padding_start)
        {
            if (strlen(notes) > 0)
            {
                strncat(notes, "; ", sizeof(notes) - strlen(notes) - 1);
            }
            // strncat(notes, "padding starts", sizeof(notes) - strlen(notes) - 1);
        }
        if (p <= g_padding_end && p + 16 > g_padding_end)
        {
            if (strlen(notes) > 0)
            {
                strncat(notes, "; ", sizeof(notes) - strlen(notes) - 1);
            }
            // strncat(notes, "padding ends", sizeof(notes) - strlen(notes) - 1);
        }
        if (maybe_int == (int)0xdeadbeef)
        {
            if (strlen(notes) > 0)
            {
                strncat(notes, "; ", sizeof(notes) - strlen(notes) - 1);
            }
            strncat(notes, "secret", sizeof(notes) - strlen(notes) - 1);
        }
        if (maybe_int == (int)0xffffff00)
        {
            if (strlen(notes) > 0)
            {
                strncat(notes, "; ", sizeof(notes) - strlen(notes) - 1);
            }
            strncat(notes, "notsecret", sizeof(notes) - strlen(notes) - 1);
        }
        if (g_ret_addr_start != NULL && p <= g_ret_addr_start && p + 16 > g_ret_addr_start)
        {
            if (strlen(notes) > 0)
            {
                strncat(notes, "; ", sizeof(notes) - strlen(notes) - 1);
            }
            strncat(notes, "saved return address", sizeof(notes) - strlen(notes) - 1);
        }

        printf("%p  ", (void *)p);
        for (i = 0; i < 16; i++)
        {
            unsigned char *byte_addr = p + i;
            int is_buff_byte = (byte_addr >= g_buff_start && byte_addr < g_buff_end);
            int is_padding_byte = (byte_addr >= g_padding_start && byte_addr < g_padding_end);
            int is_notsecret_byte = (byte_addr >= g_notsecret_start && byte_addr < g_notsecret_end);
            int is_secret_byte = (byte_addr >= g_secret_start && byte_addr < g_secret_end);
            int is_ret_byte = (g_ret_addr_start != NULL && byte_addr >= g_ret_addr_start && byte_addr < g_ret_addr_end);

            if (is_secret_byte)
            {
                printf(ANSI_RED "%02x " ANSI_RESET, p[i]);
            }
            else if (is_notsecret_byte)
            {
                printf(ANSI_YELLOW "%02x " ANSI_RESET, p[i]);
            }
            else if (is_ret_byte)
            {
                printf(ANSI_BLUE "%02x " ANSI_RESET, p[i]);
            }
            else if (is_buff_byte || is_padding_byte)
            {
                printf(ANSI_GREEN "%02x " ANSI_RESET, p[i]);
            }
            else
            {
                printf("%02x ", p[i]);
            }
        }

        printf(" ");
        for (i = 0; i < 16; i++)
        {
            unsigned char c = p[i];
            putchar((c >= 32 && c <= 126) ? (int)c : '.');
        }

        printf("  %s\n", notes);
    }
    puts("==================================\n");
}

void safeguard()
{
    puts("[safeguard] This lab is intentionally vulnerable (gets + stack buffer).\n"
         "[safeguard] Use this binary only in a controlled training environment.");

    if (geteuid() == 0)
    {
        puts("[safeguard] Warning: running as root is not recommended for this exercise.");
    }

    if (isatty(STDIN_FILENO) == 0)
    {
        puts("[safeguard] Tip: run interactively to see the stack visualization clearly.");
    }
}

void print_flag()
{
    size_t i;
    const unsigned char key = 0x5A;
    const unsigned char encoded[] = {
        0x1c, 0x16, 0x1b, 0x1d, 0x21, 0x38, 0x3b, 0x29, 0x33, 0x39,
        0x05, 0x38, 0x2f, 0x3c, 0x3c, 0x3f, 0x28, 0x05, 0x35, 0x2c,
        0x3f, 0x28, 0x3c, 0x36, 0x35, 0x2d, 0x05, 0x37, 0x3b, 0x29,
        0x2e, 0x3f, 0x28, 0x3f, 0x3e, 0x27};
    char flag[sizeof(encoded) + 1];

    for (i = 0; i < sizeof(encoded); i++)
    {
        flag[i] = (char)(encoded[i] ^ key);
    }
    flag[sizeof(encoded)] = '\0';

    puts(flag);
}