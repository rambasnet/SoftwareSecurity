# Python fundamentals

- Learn fundamentals and basic syntax
- learn in-depth from my Jupyter Notebooks: https://github.com/rambasnet/Python-Fundamentals
- find and learn from tutorials, e.g. https://realpython.com/
- use interactive mode on a Terminal
- use Jupyter Notebook with ipykernel (default) with conda installation
- learn how script works
- Basic Building Blocks/Concepts
    1. Data types and Variables
    2. Input/Output
    3. Basic Math and operators
    4. Functions
    5. Conditional Statements
    6. Loops
    

## Data Types and Variables
- 3 fundamental types (int, float and string)
- boolen type is also supported


```python
num = 10
```


```python
num = "sfasf"
```


```python
name = "John Smith"
```


```python
speed = 19.5
```


```python
ans = num*speed
```


```python
ans
```


```python
hex(10)
```


```python
int('10')
```


```python
str(99.9)
```


```python
# convert hex string to integer
int('0xa', 16)
```




    10



## Standard Input/Output


```python
print('Hello World!')
```

    Hello World!



```python
print(b'Hello World!', 11,  4.55)
```

    b'Hello World!' 11 4.55



```python
name = input("Enter your full name: ")
```

    Enter your full name:  afasd asdfad



```python
print(f'Hello there {name}!')
```

    Hello there afasd asdfad!


## Mathematics


```python
2+2
```




    4




```python
2-2
```




    0




```python
2*2
```




    4




```python
2**3
```




    8




```python
4%2
```




    0




```python
3/2
```




    1.5




```python
3//2
```




    1



## Conditional Statements


```python
if True:
    print("this line...")
    print(1)
```


      File "<ipython-input-35-4a444961ab9f>", line 3
        print(1)
        ^
    IndentationError: unexpected indent




```python
if name == "John Smith":
    print("You're John Smith!")
else:
    print("You're not John Smith!")
```

    You're not John Smith!



```python
num = int(input('Enter an integer:' ))
if num == 0:
    print(f'{num} is Zero.')
elif num%2 == 0:
    print(f'{num} is Even.')
else:
    print(f'{num} is odd.')
```

    Enter an integer: 10


    10 is Even.


## Loops


```python
for i in range(10):
    print(f'{i+1} Mississippi!')
```

    1 Mississippi!
    2 Mississippi!
    3 Mississippi!
    4 Mississippi!
    5 Mississippi!
    6 Mississippi!
    7 Mississippi!
    8 Mississippi!
    9 Mississippi!
    10 Mississippi!



```python
while True:
    num = int(input('Enter a number: '))
    if num == 0:
        break
    else:
        print(f'You entered {num}')
```

    Enter a number:  10


    You entered 10


    Enter a number:  23


    You entered 23


    Enter a number:  4


    You entered 4


    Enter a number:  0


## Functions


```python
def sayHello(str:name="John") -> str:
    return f"Hello {name}"
```


```python
print(sayHello())
```

    Hello afasd asdfad



```python
print(sayHello("David"))
```

    Hello afasd asdfad


## Built-in Data Structures

### Tuple


```python
a = (1, 2, 'hello')
```


```python
a[0]
```




    1



### List
- similar to array/vector in C++


```python
values = [1, 2, 'hi', [1, 2, 3], a]
```


```python
values
```




    [1, 2, 'hi', [1, 2, 3], (1, 2, 'hello')]




```python
help(list)
```

    Help on class list in module builtins:
    
    class list(object)
     |  list(iterable=(), /)
     |  
     |  Built-in mutable sequence.
     |  
     |  If no argument is given, the constructor creates a new empty list.
     |  The argument must be an iterable if specified.
     |  
     |  Methods defined here:
     |  
     |  __add__(self, value, /)
     |      Return self+value.
     |  
     |  __contains__(self, key, /)
     |      Return key in self.
     |  
     |  __delitem__(self, key, /)
     |      Delete self[key].
     |  
     |  __eq__(self, value, /)
     |      Return self==value.
     |  
     |  __ge__(self, value, /)
     |      Return self>=value.
     |  
     |  __getattribute__(self, name, /)
     |      Return getattr(self, name).
     |  
     |  __getitem__(...)
     |      x.__getitem__(y) <==> x[y]
     |  
     |  __gt__(self, value, /)
     |      Return self>value.
     |  
     |  __iadd__(self, value, /)
     |      Implement self+=value.
     |  
     |  __imul__(self, value, /)
     |      Implement self*=value.
     |  
     |  __init__(self, /, *args, **kwargs)
     |      Initialize self.  See help(type(self)) for accurate signature.
     |  
     |  __iter__(self, /)
     |      Implement iter(self).
     |  
     |  __le__(self, value, /)
     |      Return self<=value.
     |  
     |  __len__(self, /)
     |      Return len(self).
     |  
     |  __lt__(self, value, /)
     |      Return self<value.
     |  
     |  __mul__(self, value, /)
     |      Return self*value.
     |  
     |  __ne__(self, value, /)
     |      Return self!=value.
     |  
     |  __repr__(self, /)
     |      Return repr(self).
     |  
     |  __reversed__(self, /)
     |      Return a reverse iterator over the list.
     |  
     |  __rmul__(self, value, /)
     |      Return value*self.
     |  
     |  __setitem__(self, key, value, /)
     |      Set self[key] to value.
     |  
     |  __sizeof__(self, /)
     |      Return the size of the list in memory, in bytes.
     |  
     |  append(self, object, /)
     |      Append object to the end of the list.
     |  
     |  clear(self, /)
     |      Remove all items from list.
     |  
     |  copy(self, /)
     |      Return a shallow copy of the list.
     |  
     |  count(self, value, /)
     |      Return number of occurrences of value.
     |  
     |  extend(self, iterable, /)
     |      Extend list by appending elements from the iterable.
     |  
     |  index(self, value, start=0, stop=9223372036854775807, /)
     |      Return first index of value.
     |      
     |      Raises ValueError if the value is not present.
     |  
     |  insert(self, index, object, /)
     |      Insert object before index.
     |  
     |  pop(self, index=-1, /)
     |      Remove and return item at index (default last).
     |      
     |      Raises IndexError if list is empty or index is out of range.
     |  
     |  remove(self, value, /)
     |      Remove first occurrence of value.
     |      
     |      Raises ValueError if the value is not present.
     |  
     |  reverse(self, /)
     |      Reverse *IN PLACE*.
     |  
     |  sort(self, /, *, key=None, reverse=False)
     |      Sort the list in ascending order and return None.
     |      
     |      The sort is in-place (i.e. the list itself is modified) and stable (i.e. the
     |      order of two equal elements is maintained).
     |      
     |      If a key function is given, apply it once to each list item and sort them,
     |      ascending or descending, according to their function values.
     |      
     |      The reverse flag can be set to sort in descending order.
     |  
     |  ----------------------------------------------------------------------
     |  Static methods defined here:
     |  
     |  __new__(*args, **kwargs) from builtins.type
     |      Create and return a new object.  See help(type) for accurate signature.
     |  
     |  ----------------------------------------------------------------------
     |  Data and other attributes defined here:
     |  
     |  __hash__ = None
    


### Dictionary
- key->vlue mapping DS


```python
engToSpan = {'one': 'uno'}
```


```python
engToSpan['two'] = 'dos'
```


```python
engToSpan
```




    {'one': 'uno', 'two': 'dos'}




```python
engToSpan.get('three', 'nada')
```




    'nada'




```python
help(dict)
```

    Help on class dict in module builtins:
    
    class dict(object)
     |  dict() -> new empty dictionary
     |  dict(mapping) -> new dictionary initialized from a mapping object's
     |      (key, value) pairs
     |  dict(iterable) -> new dictionary initialized as if via:
     |      d = {}
     |      for k, v in iterable:
     |          d[k] = v
     |  dict(**kwargs) -> new dictionary initialized with the name=value pairs
     |      in the keyword argument list.  For example:  dict(one=1, two=2)
     |  
     |  Built-in subclasses:
     |      StgDict
     |  
     |  Methods defined here:
     |  
     |  __contains__(self, key, /)
     |      True if the dictionary has the specified key, else False.
     |  
     |  __delitem__(self, key, /)
     |      Delete self[key].
     |  
     |  __eq__(self, value, /)
     |      Return self==value.
     |  
     |  __ge__(self, value, /)
     |      Return self>=value.
     |  
     |  __getattribute__(self, name, /)
     |      Return getattr(self, name).
     |  
     |  __getitem__(...)
     |      x.__getitem__(y) <==> x[y]
     |  
     |  __gt__(self, value, /)
     |      Return self>value.
     |  
     |  __init__(self, /, *args, **kwargs)
     |      Initialize self.  See help(type(self)) for accurate signature.
     |  
     |  __iter__(self, /)
     |      Implement iter(self).
     |  
     |  __le__(self, value, /)
     |      Return self<=value.
     |  
     |  __len__(self, /)
     |      Return len(self).
     |  
     |  __lt__(self, value, /)
     |      Return self<value.
     |  
     |  __ne__(self, value, /)
     |      Return self!=value.
     |  
     |  __repr__(self, /)
     |      Return repr(self).
     |  
     |  __reversed__(self, /)
     |      Return a reverse iterator over the dict keys.
     |  
     |  __setitem__(self, key, value, /)
     |      Set self[key] to value.
     |  
     |  __sizeof__(...)
     |      D.__sizeof__() -> size of D in memory, in bytes
     |  
     |  clear(...)
     |      D.clear() -> None.  Remove all items from D.
     |  
     |  copy(...)
     |      D.copy() -> a shallow copy of D
     |  
     |  get(self, key, default=None, /)
     |      Return the value for key if key is in the dictionary, else default.
     |  
     |  items(...)
     |      D.items() -> a set-like object providing a view on D's items
     |  
     |  keys(...)
     |      D.keys() -> a set-like object providing a view on D's keys
     |  
     |  pop(...)
     |      D.pop(k[,d]) -> v, remove specified key and return the corresponding value.
     |      If key is not found, d is returned if given, otherwise KeyError is raised
     |  
     |  popitem(self, /)
     |      Remove and return a (key, value) pair as a 2-tuple.
     |      
     |      Pairs are returned in LIFO (last-in, first-out) order.
     |      Raises KeyError if the dict is empty.
     |  
     |  setdefault(self, key, default=None, /)
     |      Insert key with a value of default if key is not in the dictionary.
     |      
     |      Return the value for key if key is in the dictionary, else default.
     |  
     |  update(...)
     |      D.update([E, ]**F) -> None.  Update D from dict/iterable E and F.
     |      If E is present and has a .keys() method, then does:  for k in E: D[k] = E[k]
     |      If E is present and lacks a .keys() method, then does:  for k, v in E: D[k] = v
     |      In either case, this is followed by: for k in F:  D[k] = F[k]
     |  
     |  values(...)
     |      D.values() -> an object providing a view on D's values
     |  
     |  ----------------------------------------------------------------------
     |  Class methods defined here:
     |  
     |  fromkeys(iterable, value=None, /) from builtins.type
     |      Create a new dictionary with keys from iterable and values set to value.
     |  
     |  ----------------------------------------------------------------------
     |  Static methods defined here:
     |  
     |  __new__(*args, **kwargs) from builtins.type
     |      Create and return a new object.  See help(type) for accurate signature.
     |  
     |  ----------------------------------------------------------------------
     |  Data and other attributes defined here:
     |  
     |  __hash__ = None
    


## Modules
- see `python/module.py` file
- typically run module from terminal


```python
! cat python/module.py
```

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


```python
# run script without argument
! python python/module.py
```

    Hello There!
    AAAA



```python
# run script with argument
! python python/module.py test
```

    Hello There!
    running test...
    OK!
    AAAA


## System command
- execute system/shell commands from with Python

### os.system()


```python
import os
```


```python
cmd = "git --version"
ret_val = os.system(cmd)
print(f'returned: {ret_val}')
```

    returned: 0


### subprocess.call( )
- subprocess.check_output() allows to store the output in a variable


```python
import subprocess
```


```python
ret_val = subprocess.call(cmd, shell=True)
```


```python
ret_val
```




    0




```python
output = subprocess.check_output(cmd, shell=True)
```


```python
output
```




    b'git version 2.34.1\n'




```python
# print the output binary in ASCII
print(*output)
```

    103 105 116 32 118 101 114 115 105 111 110 32 50 46 51 52 46 49 10



```python
# print the returned binary as a utf-8 encoded string
print(output.decode('utf-8'))
```

    git version 2.34.1
    



```python
# providing list as command is recommended
output = subprocess.check_output(['git --version'], shell=True)
```


```python
print(output.decode('utf-8'))
```

    git version 2.37.1 (Apple Git-137.1)
    



```python
import sys
# providing list as command is recommended
output = subprocess.check_output(["/bin/sh", "-c", "ls -l; exit 0"], 
             stderr=sys.stdout)
```


```python
print(output)
```

    b'total 4416\n-rw-r--r--   1 rbasnet  AD\\Domain Users    4376 Feb  1 08:41 00-TableOfContents.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   35262 Feb  1 08:38 BufferOverflowBasics.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   20213 Feb  1 08:38 BufferOverflowProtections.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   25444 Feb  1 08:38 C-Arrays.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users    7763 Feb  1 08:38 C-Strings.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   48332 Feb  1 08:38 CS1-Review.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   25731 Feb  1 08:38 CTF.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   43531 Feb  1 08:38 DetectingMemoryCorruptionErrors.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users  229567 Feb  1 08:38 ELF.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   22537 Feb  1 08:38 ExploitCode-UsingBashScript.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   19210 Feb  1 08:38 ExploitCode-UsingCPP.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   28785 Feb  1 08:38 ExploitCode-UsingPythonScript.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users  411383 Feb  1 08:38 FormatStrings.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users    6889 Feb  1 08:38 Function-Pointers.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   23327 Feb  1 08:38 FunctionsAndUnittesting.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users    9231 Feb  1 08:38 GDB-Peda.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   64126 Feb  1 08:38 GlobalSegmentOverflow.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users  470021 Feb  1 08:38 HackingTools.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   13663 Feb  1 08:38 HeapOverflowExploitation.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users    1067 Feb  1 08:38 LICENSE\n-rw-r--r--   1 rbasnet  AD\\Domain Users    7272 Feb  1 08:38 MemorySegments.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   53905 Feb  1 08:38 OverflowingFunctionPointers.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users    8163 Feb  1 08:38 PenTesting.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   14457 Feb  1 08:38 Pointers.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   57683 Feb  1 08:38 PwntoolsTutorials.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   28427 Feb  1 08:49 PythonBasics.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users    2011 Feb  1 08:38 README.md\n-rw-r--r--   1 rbasnet  AD\\Domain Users   31600 Feb  1 08:38 ROP-new.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   27536 Feb  1 08:38 ROP.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   14599 Feb  1 08:38 SQLInjection.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   12016 Feb  1 08:38 SQLite3-Database.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   12099 Feb  1 08:38 SetupVM.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   10524 Feb  1 08:38 StackOverflow-EnvVariable.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   37751 Feb  1 08:38 StackOverflow-ExecutionFlow.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users  248976 Feb  1 08:38 StackOverflow-MemoryCorruption.ipynb\n-rw-r--r--   1 rbasnet  AD\\Domain Users   43270 Feb  1 08:38 StackOverflow-RemoteCodeExecution.ipynb\n-rwxr-xr-x   1 rbasnet  AD\\Domain Users     699 Feb  1 08:38 compile.sh\ndrwxr-xr-x   7 rbasnet  AD\\Domain Users     224 Feb  1 08:38 ctf-demos\ndrwxr-xr-x  30 rbasnet  AD\\Domain Users     960 Feb  1 08:38 demos\n-rw-r--r--   1 rbasnet  AD\\Domain Users     100 Feb  1 08:38 exploit_fmt.bin\n-rw-r--r--   1 rbasnet  AD\\Domain Users    1244 Feb  1 08:38 exploit_vuln_cpp.py\n-rw-r--r--   1 rbasnet  AD\\Domain Users    1222 Feb  1 08:38 hacking.h\ndrwxr-xr-x   6 rbasnet  AD\\Domain Users     192 Feb  1 08:38 labs\ndrwxr-xr-x  20 rbasnet  AD\\Domain Users     640 Feb  1 08:38 media\n-rw-r--r--   1 rbasnet  AD\\Domain Users     164 Feb  1 08:38 payload.bin\n-rw-r--r--   1 rbasnet  AD\\Domain Users      80 Feb  1 08:38 payload_env.bin\ndrwxr-xr-x   6 rbasnet  AD\\Domain Users     192 Feb  1 08:38 pwntools-demos\ndrwxr-xr-x   4 rbasnet  AD\\Domain Users     128 Feb  1 08:38 python\ndrwxr-xr-x   7 rbasnet  AD\\Domain Users     224 Feb  1 08:38 shellcode\n-rw-r--r--   1 rbasnet  AD\\Domain Users      24 Feb  1 08:38 shellcode.bin\n-rw-r--r--   1 rbasnet  AD\\Domain Users      24 Feb  1 08:38 shellcode1_py3.bin\n-rw-r--r--   1 rbasnet  AD\\Domain Users      24 Feb  1 08:38 shellcode_bash.bin\n-rw-r--r--   1 rbasnet  AD\\Domain Users      24 Feb  1 08:38 shellcode_py3.bin\n-rw-r--r--   1 rbasnet  AD\\Domain Users      35 Feb  1 08:38 shellcode_root.bin\n-rw-r--r--   1 rbasnet  AD\\Domain Users     101 Feb  1 08:38 so_arg_cpp_exploit.sh\n-rw-r--r--   1 rbasnet  AD\\Domain Users     110 Feb  1 08:38 so_arg_cpp_exploit_aslr_on.sh\n-rw-r--r--   1 rbasnet  AD\\Domain Users    1976 Feb  1 08:38 so_arg_exploit.sh\n-rw-r--r--   1 rbasnet  AD\\Domain Users     144 Feb  1 08:38 stdio_payload.bin\n-rw-r--r--   1 rbasnet  AD\\Domain Users     126 Feb  1 08:38 testfile\n-rw-r--r--   1 rbasnet  AD\\Domain Users    5881 Feb  1 08:38 x86-AssemblyAndStack.ipynb\n'



```python
for line in output.decode('utf-8').split('\n'):
    print(line)
```

    total 4416
    -rw-r--r--   1 rbasnet  AD\Domain Users    4376 Feb  1 08:41 00-TableOfContents.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   35262 Feb  1 08:38 BufferOverflowBasics.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   20213 Feb  1 08:38 BufferOverflowProtections.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   25444 Feb  1 08:38 C-Arrays.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users    7763 Feb  1 08:38 C-Strings.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   48332 Feb  1 08:38 CS1-Review.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   25731 Feb  1 08:38 CTF.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   43531 Feb  1 08:38 DetectingMemoryCorruptionErrors.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users  229567 Feb  1 08:38 ELF.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   22537 Feb  1 08:38 ExploitCode-UsingBashScript.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   19210 Feb  1 08:38 ExploitCode-UsingCPP.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   28785 Feb  1 08:38 ExploitCode-UsingPythonScript.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users  411383 Feb  1 08:38 FormatStrings.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users    6889 Feb  1 08:38 Function-Pointers.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   23327 Feb  1 08:38 FunctionsAndUnittesting.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users    9231 Feb  1 08:38 GDB-Peda.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   64126 Feb  1 08:38 GlobalSegmentOverflow.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users  470021 Feb  1 08:38 HackingTools.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   13663 Feb  1 08:38 HeapOverflowExploitation.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users    1067 Feb  1 08:38 LICENSE
    -rw-r--r--   1 rbasnet  AD\Domain Users    7272 Feb  1 08:38 MemorySegments.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   53905 Feb  1 08:38 OverflowingFunctionPointers.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users    8163 Feb  1 08:38 PenTesting.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   14457 Feb  1 08:38 Pointers.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   57683 Feb  1 08:38 PwntoolsTutorials.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   28427 Feb  1 08:49 PythonBasics.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users    2011 Feb  1 08:38 README.md
    -rw-r--r--   1 rbasnet  AD\Domain Users   31600 Feb  1 08:38 ROP-new.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   27536 Feb  1 08:38 ROP.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   14599 Feb  1 08:38 SQLInjection.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   12016 Feb  1 08:38 SQLite3-Database.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   12099 Feb  1 08:38 SetupVM.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   10524 Feb  1 08:38 StackOverflow-EnvVariable.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   37751 Feb  1 08:38 StackOverflow-ExecutionFlow.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users  248976 Feb  1 08:38 StackOverflow-MemoryCorruption.ipynb
    -rw-r--r--   1 rbasnet  AD\Domain Users   43270 Feb  1 08:38 StackOverflow-RemoteCodeExecution.ipynb
    -rwxr-xr-x   1 rbasnet  AD\Domain Users     699 Feb  1 08:38 compile.sh
    drwxr-xr-x   7 rbasnet  AD\Domain Users     224 Feb  1 08:38 ctf-demos
    drwxr-xr-x  30 rbasnet  AD\Domain Users     960 Feb  1 08:38 demos
    -rw-r--r--   1 rbasnet  AD\Domain Users     100 Feb  1 08:38 exploit_fmt.bin
    -rw-r--r--   1 rbasnet  AD\Domain Users    1244 Feb  1 08:38 exploit_vuln_cpp.py
    -rw-r--r--   1 rbasnet  AD\Domain Users    1222 Feb  1 08:38 hacking.h
    drwxr-xr-x   6 rbasnet  AD\Domain Users     192 Feb  1 08:38 labs
    drwxr-xr-x  20 rbasnet  AD\Domain Users     640 Feb  1 08:38 media
    -rw-r--r--   1 rbasnet  AD\Domain Users     164 Feb  1 08:38 payload.bin
    -rw-r--r--   1 rbasnet  AD\Domain Users      80 Feb  1 08:38 payload_env.bin
    drwxr-xr-x   6 rbasnet  AD\Domain Users     192 Feb  1 08:38 pwntools-demos
    drwxr-xr-x   4 rbasnet  AD\Domain Users     128 Feb  1 08:38 python
    drwxr-xr-x   7 rbasnet  AD\Domain Users     224 Feb  1 08:38 shellcode
    -rw-r--r--   1 rbasnet  AD\Domain Users      24 Feb  1 08:38 shellcode.bin
    -rw-r--r--   1 rbasnet  AD\Domain Users      24 Feb  1 08:38 shellcode1_py3.bin
    -rw-r--r--   1 rbasnet  AD\Domain Users      24 Feb  1 08:38 shellcode_bash.bin
    -rw-r--r--   1 rbasnet  AD\Domain Users      24 Feb  1 08:38 shellcode_py3.bin
    -rw-r--r--   1 rbasnet  AD\Domain Users      35 Feb  1 08:38 shellcode_root.bin
    -rw-r--r--   1 rbasnet  AD\Domain Users     101 Feb  1 08:38 so_arg_cpp_exploit.sh
    -rw-r--r--   1 rbasnet  AD\Domain Users     110 Feb  1 08:38 so_arg_cpp_exploit_aslr_on.sh
    -rw-r--r--   1 rbasnet  AD\Domain Users    1976 Feb  1 08:38 so_arg_exploit.sh
    -rw-r--r--   1 rbasnet  AD\Domain Users     144 Feb  1 08:38 stdio_payload.bin
    -rw-r--r--   1 rbasnet  AD\Domain Users     126 Feb  1 08:38 testfile
    -rw-r--r--   1 rbasnet  AD\Domain Users    5881 Feb  1 08:38 x86-AssemblyAndStack.ipynb
    



```python
import sys
# providing list as command is recommended
subprocess.check_output(["python", "-c", "print(f'{(2**3)=}')"], 
             stderr=sys.stdout)
```




    b'(2**3)=8\n'



## Exercise

- Complete the following quizzes from: https://realpython.com/quizzes/
- Take the screenshot of the first attempt. Take the screenshot of the final attempt when you receive 100%

1. Basic Data Types in Python ( https://realpython.com/quizzes/python-data-types/) 
2. How to Run Your Python Scrpts (https://realpython.com/quizzes/run-python-scripts/)
3. Python Basics: Code Your First Python Program - https://realpython.com/quizzes/python-basics-first-program/
4. Python Basics: Conditional Logic and Control Flow (https://realpython.com/quizzes/python-basics-conditional-logic-and-control-flow/)
5. Python Basics: Dictionaries - https://realpython.com/quizzes/python-basics-dictionaries/
6. Python Basics: Functions and Loops - https://realpython.com/quizzes/python-basics-functions-and-loops/
7. Python Basics: Numbers and Math - https://realpython.com/quizzes/python-basics-numbers-and-math/
8. Python Basics: Scopes - https://realpython.com/quizzes/python-basics-scopes/
9. Python Basics: Strings and String Methods - https://realpython.com/quizzes/python-basics-strings-and-string-methods/
10. Python Basics: Python Lists and Tuples - https://realpython.com/quizzes/python-lists-tuples/
11. Python Operators and Expressions - https://realpython.com/quizzes/python-operators-expressions/
12. Python f-String: https://realpython.com/quizzes/f-strings/
13. HTTP Requests with "requests" Library - https://realpython.com/quizzes/python-requests/


```python

```
