import os
for root, dirs, files in os.walk('/app/C-Language-Games'):
    for f in files:
        if 'main.c' in f or 'Hit' in f or 'hit' in f.lower():
            print(os.path.join(root, f))
