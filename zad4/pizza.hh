//Authors: Marcin Kania, Milena Wojtczak, Jan Marcinkiewicz

#ifndef __PIZZA_H__
#define __PIZZA_H__

#include <cstddef>
#include <array>
#include <type_traits>

namespace {
	/**
		Struct which checks if `Kind` is in `Kinds...`
		As value returns number of occurences in `Kinds...`
		*/
	template<typename Kind, typename...>
	struct is_in {
		static constexpr size_t value = 0;
	};

	template<typename Kind, typename Head, typename... Tail>
	struct is_in<Kind, Head, Tail...> {
		static constexpr size_t value = std::is_same<Kind, Head>::value + is_in<Kind, Tail...>::value;
	};

	template<typename Kind, typename Head>
	struct is_in<Kind, Head> {
		static constexpr size_t value = std::is_same<Kind, Head>::value;
	};
}

template<typename... Kinds>
struct Pizzeria {
private:
	/**
		Struct which represents `type` of pizza
		*/
	template<size_t... SlicesList>
	struct type {
		using pizzeria = Pizzeria;

		/**
			Returns number of slices for specific `Kind`
		*/
		template<typename Kind>
		static constexpr size_t count() {
			return ((std::is_same<Kind, Kinds>::value * SlicesList) + ...);
		};

		/**
			Returns number of slices in pizza for each kind of pizzas
		*/
		static constexpr std::array<size_t, sizeof...(Kinds)> as_array() {
			return {{ SlicesList... }};
		}

		using sliced_type = type<(SlicesList * 2) ...>;
	};

public:
	/**
		Struct which mixes and represents mixed `types` of pizza
		*/
	template<typename TypePizza1, typename TypePizza2>
	struct best_mix {
	private:
		/**
			Searches for best `N` to maximize `yumminess` of specific `Kind`
			*/
		template<typename Kind>
		static constexpr size_t best(size_t limit) {
			static_assert(Kind::yumminess(0) == 0, "Yumminess for 0 slices is not 0");

			auto max = Kind::yumminess(0);
			size_t value = 0;

			for (size_t i = 1; i <= limit; ++i) {
				auto cur = Kind::yumminess(i);
				if (cur > max) {
					max = cur;
					value = i;
				}
			}

			return value;
		}

	public:
		/**
			Here we unfold `Kinds` and for each `Kind` we
			try to find best amount of slices composed from two pizzas
			*/
		using type = type<best<Kinds>(
						TypePizza1::template count<Kinds>() +
						TypePizza2::template count<Kinds>()
				) ...>;
	};

	/**
		Creates `type` of pizza of specific kind
		We make `type` from list of 0 and 8, where 8 is only on position of `Kind`
	*/
	template<typename Kind>
	struct make_pizza {
		static_assert(is_in<Kind, Kinds...>::value == 1,
					  "This kind of pizza is not in the menu or is duplicated");

		using type = type<(std::is_same<Kind, Kinds>::value ? 8 : 0) ...>;
	};
};

/**
	Mixes two `types` of pizzas into the best one
	*/
template<typename TypePizza1, typename TypePizza2>
struct best_mix {
	static_assert(std::is_same<
					typename TypePizza1::pizzeria,
					typename TypePizza2::pizzeria
				>::value, "Incompatible pizzerias");

	using type = typename TypePizza1::pizzeria::template best_mix<TypePizza1, TypePizza2>::type;
};

#endif
