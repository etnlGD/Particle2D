#pragma once
#include <math3d/vector4d.h>
#include <math3d/math3d.h>

namespace particle2d
{
	using namespace math3d;

	template <typename T>
	struct SFixedPointColor
	{
		typedef uint8_t u8;
		typedef uint32_t u32;
		typedef SFixedPointColor<T> MyType;

	public:
		enum E_COLOR_CHANNEL 
		{ 
			ECC_ALPHA_CHANNEL, 
			ECC_RED_CHANNEL, 
			ECC_GREEN_CHANNEL, 
			ECC_BLUE_CHANNEL,
		};

		enum 
		{
			CHANNEL_BITS = sizeof(T) << 3,
			FRANCTIONAL_BITS = (sizeof(T) << 3) - 8,
			CHANNEL_MAX_VALUE = (1 << CHANNEL_BITS) - 1,
		};

	public:
		static u8 fixed_point_to_u8(T  value) 
		{ 
			return (u8) (value >> FRANCTIONAL_BITS); 
		}

		static T  u8_to_fixed_point(u8 value) 
		{ 
			return ((T) value) << FRANCTIONAL_BITS; 
		}

		static T float_to_fixed_point(float value)
		{
			return (T) clamp(value * CHANNEL_MAX_VALUE, 
								0.f, (float) CHANNEL_MAX_VALUE);
		}

		static float fixed_point_to_float(T value)
		{
			static inv = 1.f / CHANNEL_MAX_VALUE;
			return inv * value;
		}

	public:
		SFixedPointColor() {}
		
		SFixedPointColor(float r, float g, float b, float a = 1.f) 
		{ 
			setARGB(a, r, g, b); 
		}

		SFixedPointColor(u32 color) { setARGBInt(color); }

		SFixedPointColor(const vec4& colorf)
		{
			setARGB(colorf.w, colorf.x, colorf.y, colorf.z);
		}

		bool operator==(const MyType& other) const
		{
			for (u32 i = 0; i < 4; ++i)
			{
				if (this->channels[i] != other.channels[i])
					return false;
			}

			return true;
		}

		bool operator!=(const MyType& other) const 
		{ 
			return !((*this) == other);
		}

		void setChannel(E_COLOR_CHANNEL channel, float value) 
		{ 
			channels[channel] = float_to_fixed_point(value);
		}

		inline void setAlpha(float a) { setChannel(ECC_ALPHA_CHANNEL,	a);	}
		inline void setRed	(float r) { setChannel(ECC_RED_CHANNEL,		r);	}
		inline void setGreen(float g) { setChannel(ECC_GREEN_CHANNEL,	g);	}
		inline void setBlue (float b) { setChannel(ECC_BLUE_CHANNEL,	b); }

		inline void setRGB(float r, float g, float b)
		{
			setRed(r);
			setGreen(g);
			setBlue(b);
		}

		inline void setARGB(float a, float r, float g, float b)
		{
			setAlpha(a);
			setRGB(r, g, b);
		}

		float getChannel(E_COLOR_CHANNEL channel) const 
		{ 
			return fixed_point_to_float(channels[channel]);
		}

		float getAlpha() const { return getChannel(ECC_ALPHA_CHANNEL);	}
		float getRed  () const { return getChannel(ECC_RED_CHANNEL);	}
		float getGreen() const { return getChannel(ECC_GREEN_CHANNEL);	}
		float getBlue () const { return getChannel(ECC_BLUE_CHANNEL); 	}
		
		void interpolate(const MyType& source, const MyType& target, float d)
		{
			_interpolate_ch(source, target, d, ECC_ALPHA_CHANNEL);
			_interpolate_ch(source, target, d, ECC_RED_CHANNEL);
			_interpolate_ch(source, target, d, ECC_GREEN_CHANNEL);
			_interpolate_ch(source, target, d, ECC_BLUE_CHANNEL);
		}
		
		MyType lerp(const MyType& target, float d)
		{
			MyType res;
			res.interpolate(*this, target, d);_lerp_channel(target, d, ECC_ALPHA_CHANNEL);
			lerp(target, d, ECC_RED_CHANNEL);
			lerp(target, d, ECC_GREEN_CHANNEL);
			lerp(target, d, ECC_BLUE_CHANNEL);
		}

		void setARGBInt(u32 color)
		{
			_set_channel(ECC_ALPHA_CHANNEL, (color & 0xff000000) >> 24);
			_set_channel(ECC_RED_CHANNEL,	(color & 0x0000ff00) >> 16);
			_set_channel(ECC_GREEN_CHANNEL, (color & 0x0000ff00) >> 8);
			_set_channel(ECC_BLUE_CHANNEL,	(color & 0x000000ff) >> 0);
		}

		u32 toARGBInt()
		{
			u32 res = 0;
			res |= (u32) fixed_point_to_u8(channels[ECC_ALPHA_CHANNEL]) << 24;
			res |= (u32) fixed_point_to_u8(channels[ECC_RED_CHANNEL])	<< 16;
			res |= (u32) fixed_point_to_u8(channels[ECC_GREEN_CHANNEL]) << 8;
			res |= (u32) fixed_point_to_u8(channels[ECC_BLUE_CHANNEL])	<< 0;
			return res;
		}

		vec4 toVec4() const 
		{
			return vec4(getRed(), getGreen(), getBlue(), getAlpha());
		}

	private:
		void _interpolate_ch(const MyType& source, const MyType& target, 
							 float d, E_COLOR_CHANNEL channel)
		{
			channels[channel] = source._lerp_channel(target, d, channel);
		}

		T _lerp_channel(const MyType& target, float d, E_COLOR_CHANNEL channel)
		{
			T sourceValue = channels[channel];
			T targetValue = target.channels[channel];
			return lerp(sourceValue, targetValue, d);
		}

		inline void _set_channel(E_COLOR_CHANNEL channel, u8 value)
		{
			channels[channel] = u8_to_fixed_point(value);
		}

	private:
		T channels[4];
	};

	typedef SFixedPointColor<uint16_t> SColor16;
	typedef SFixedPointColor<uint32_t> SColor32;
}

