import sys

print('Hello There!')

def add(num1, num2):
    return num1 + num2

def test():
    print('running test...')
    assert 1 == 1
    assert add(2, 3) == 5
    print('OK!')

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "test":
        test()

    sys.stdout.buffer.write(b"\x41\x41\x41\x41\n")