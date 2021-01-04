# os.system function

import os

cmd = "git --version"
ret_val = os.system(cmd)
print(f'returned: {ret_val}')