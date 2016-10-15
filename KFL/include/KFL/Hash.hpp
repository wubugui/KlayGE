/**
 * @file Hash.hpp
 * @author Minmin Gong
 *
 * @section DESCRIPTION
 *
 * This source file is part of KFL, a subproject of KlayGE
 * For the latest info, see http://www.klayge.org
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * You may alternatively use this source under the terms of
 * the KlayGE Proprietary License (KPL). You can obtained such a license
 * from http://www.klayge.org/licensing/.
 */

#ifndef _KFL_HASH_HPP
#define _KFL_HASH_HPP

#pragma once

#include <KFL/PreDeclare.hpp>

#include <boost/functional/hash.hpp>

namespace KlayGE
{
#define PRIME_NUM 0x9e3779b9

#ifdef KLAYGE_CXX11_CORE_CONSTEXPR_SUPPORT
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(disable: 4307) // The hash here could cause integral constant overflow
#endif

	constexpr size_t _Hash(char const * str, size_t seed)
	{
		return 0 == *str ? seed : _Hash(str + 1, seed ^ (*str + PRIME_NUM + (seed << 6) + (seed >> 2)));
	}

#ifdef KLAYGE_COMPILER_MSVC
	template <size_t N>
	struct EnsureConst
	{
		static const size_t value = N;
	};

	#define CT_HASH(x) (EnsureConst<_Hash(x, 0)>::value)
#else
	#define CT_HASH(x) (_Hash(x, 0))
#endif
#else
	#if defined(KLAYGE_COMPILER_MSVC)
		#define FORCEINLINE __forceinline
	#else
		#define FORCEINLINE inline
	#endif

	FORCEINLINE size_t _Hash(const char (&str)[1])
	{
		return *str + PRIME_NUM;
	}

	template <size_t N>
	FORCEINLINE size_t _Hash(const char (&str)[N])
	{
		typedef const char (&truncated_str)[N - 1];
		size_t seed = _Hash((truncated_str)str);
		return seed ^ (*(str + N - 1) + PRIME_NUM + (seed << 6) + (seed >> 2));
	}

	template <size_t N>
	FORCEINLINE size_t CT_HASH(const char (&str)[N])
	{
		typedef const char (&truncated_str)[N - 1];
		return _Hash((truncated_str)str);
	}

	#undef FORCEINLINE
#endif

	template <typename SizeT>
	inline void HashCombineImpl(SizeT& seed, SizeT value)
	{
		seed ^= value + PRIME_NUM + (seed << 6) + (seed >> 2);
	}

	inline size_t RT_HASH(char const * str)
	{
		size_t seed = 0;
		while (*str != 0)
		{
			HashCombineImpl(seed, static_cast<size_t>(*str));
			++ str;
		}
		return seed;
	}

#undef PRIME_NUM

	template <typename T>
	inline size_t HashValue(T v)
	{
		return static_cast<size_t>(v);
	}

	template <typename T>
	inline void HashCombine(size_t& seed, T const & v)
	{
		boost::hash<T> hasher;
		return HashCombineImpl(seed, hasher(v));
	}

	template <typename T>
	inline void HashRange(size_t& seed, T first, T last)
	{
		for (; first != last; ++ first)
		{
			HashCombine(seed, *first);
		}
	}

	template <typename T>
	inline size_t HashRange(T first, T last)
	{
		size_t seed = 0;
		HashRange(seed, first, last);
		return seed;
	}
}

#endif		// _KFL_HASH_HPP
