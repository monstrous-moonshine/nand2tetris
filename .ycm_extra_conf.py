import os
def Settings(**kwargs):
    path = os.path.dirname(kwargs['filename'])
    return {
        'flags': [
            f'-I{path}/obj_dir',
            '-I/usr/share/verilator/include',
            '-I/usr/share/verilator/include/vltstd',
        ]
    }
