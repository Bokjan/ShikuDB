import os
import platform

Platform = platform.system()
#OS: ['Darwin', 'Linux', 'Windows']
if 'NT' in Platform:
	Platform = 'Windows'

ROOT = './'
Env = Environment()
FilesToCompile = ['shikud.cpp', 'Dependency/mongoose.c']
IncludedDirectories = ['Frontend']

def IncludeDirectory(directory, filetype):
	return Env.Glob(ROOT + directory + '/' + filetype)

if Platform == 'Darwin':
	Env.Replace(CC = 'clang', CXX = 'clang++')
else:
	Env.Replace(CC = 'gcc', CXX = 'g++')
Env.Append(CFLAGS = ['-g', '-O2'])
Env.Append(CXXFLAGS = ['-g', '-O2', '-IHeader', '-IDependency', '-std=c++14'])
Env.Append(LINKFLAGS = ['-g', '-O2'])

for item in IncludedDirectories:
	FilesToCompile += IncludeDirectory(item, '*.cpp')

if Platform == 'Windows':
	Env.Append(LINKFLAGS = ['-static'])
	Env.Program(FilesToCompile, LIBS = ['ws2_32'])
else:
	Env.Program(FilesToCompile)