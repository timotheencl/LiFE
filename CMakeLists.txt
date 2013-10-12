cmake_minimum_required(VERSION 2.8)

project(LiFE)

option(USE_SSE "Use SIMD SSE3 instructions" OFF)

if(USE_SSE)
	add_definitions( -DUSE_SSE )
	add_definitions( -msse3 )
	message(status "** SSE3 optimized version **")
endif(USE_SSE)

add_definitions(-O3)

file(
	GLOB_RECURSE
	source_files
	src/*
)

add_executable(
	LiFE
	${source_files}
	)

target_link_libraries(
	LiFE
	m
	SDL
	SDL_image
	SDL_ttf
	GL
	GLU
)
