#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	// This method should be implemented in objetive-c
	extern void particle_system_ios_native_render(
		void* native_render_ptr, int particle_index,
		float x, float y, 
		float a, float r, float g, float b, 
		float w, float h);

	extern void* particle_system_ios_create(void* native_render_ptr, 
											const char* args);

	extern void particle_system_ios_animate(void* particle_system_ptr, 
											float time);

	extern void particle_system_ios_destroy(void* particle_system_ptr);

#ifdef __cplusplus
};
#endif // __cplusplus

