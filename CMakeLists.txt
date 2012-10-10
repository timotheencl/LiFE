project(LiFE)

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
