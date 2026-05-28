import os
with open('/app/C-Language-Games/all_files.txt', 'w') as f:
    for root, dirs, files in os.walk('/app/C-Language-Games'):
        for file in files:
            f.write(os.path.join(root, file) + '\n')
