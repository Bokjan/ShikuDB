import os
import platform

Platform = platform.system()
#OS: ['Darwin', 'Linux', 'Windows']
if 'NT' in Platform:
    Platform = 'Windows'

ROOT = './'
Env = Environment()
FilesToCompile = []
IncludedDirectories = []

def IncludeDirectory(directory, filetype):
    return Env.Glob(ROOT + directory + '/' + filetype)

if Platform == 'Darwin':
    Env.Replace(CC = 'clang', CXX = 'clang++')
else:
    Env.Replace(CC = 'gcc', CXX = 'g++')
Env.Append(CXXFLAGS = ['-O2', '-std=c++14', '-lmicrohttpd'])
Env.Append(LINKFLAGS = ['-lmicrohttpd'])
if Platform == 'Windows':
    Env.Append(LINKFLAGS = ['-lws2_32', '-static'])

for item in IncludedDirectories:
    FilesToCompile += IncludeDirectory(item, '*cpp')

Env.Program(FilesToCompile)