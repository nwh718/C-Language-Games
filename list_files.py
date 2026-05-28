import os
with open('file_list.txt', 'w') as f:
    for dp, dn, filenames in os.walk('.'):
        for file in filenames:
            f.write(os.path.join(dp, file) + '\n')
