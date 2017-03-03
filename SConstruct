import os
import platform

Platform = platform.system()
#OS: ['Darwin', 'Linux', 'Windows']
if 'NT' in Platform:
	Platform = 'Windows'

ROOT = './'
Env = Environment()
FilesToCompile = ['shiku.cpp']
IncludedDirectories = ['ThirdParty', 'HttpServer']

def IncludeDirectory(directory, filetype):
	return Env.Glob(ROOT + directory + '/' + filetype)

if Platform == 'Darwin':
	Env.Replace(CC = 'clang', CXX = 'clang++')
else:
	Env.Replace(CC = 'gcc', CXX = 'g++')
Env.Append(CXXFLAGS = ['-O2', '-IHeader', '-IThirdParty', '-std=c++14'])
Env.Append(LINKFLAGS = ['-static'])

for item in IncludedDirectories:
	FilesToCompile += IncludeDirectory(item, '*.c')
	FilesToCompile += IncludeDirectory(item, '*.cpp')

if Platform == 'Windows':
	Env.Program(FilesToCompile, LIBS = ['ws2_32'])
else:
	Env.Program(FilesToCompile)