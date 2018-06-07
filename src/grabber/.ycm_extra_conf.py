def FlagsForFile( filename, **kwargs ):
  return {
         'flags': [ '-x', 'c++', '-Wall', '-Wextra', '-Werror', '-I/opt/pylon5/include','-I/usr/include/opencv' ],
         }
