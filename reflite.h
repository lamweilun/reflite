#ifndef REFLITE_H
#define REFLITE_H

#include <tuple>
#include <functional>
#include <variant>
#include <array>

namespace reflite
{
	template <typename Tup, typename R, typename Func, std::size_t... Idxs>
	struct tuple_runtime_access_table {
		using tuple_type = Tup;
		using return_type = R;
		using converter_fun = Func;

		template <std::size_t N>
		static return_type access_tuple(tuple_type& t, converter_fun& f) {
			return f(std::get<N>(t));
		}

		using accessor_fun_ptr = return_type(*)(tuple_type&, converter_fun&);
		const static auto table_size = sizeof...(Idxs);

		constexpr static std::array<accessor_fun_ptr, table_size> lookup_table = {
			{&access_tuple<Idxs>...}
		};
	};

	template <typename R, typename Tup, typename Func, std::size_t... Idxs>
	auto call_access_function(Tup& t, std::size_t i, Func f, std::index_sequence<Idxs...>) {
		auto& table = tuple_runtime_access_table<Tup, R, Func, Idxs...>::lookup_table;
		auto* access_function = table[i];
		return access_function(t, f);
	}

	template <typename Tup> struct common_tuple_access;

	template <typename... Ts>
	struct common_tuple_access<std::tuple<Ts...>> {
		using type = std::variant<std::reference_wrapper<Ts>...>;
	};

	template <typename T1, typename T2>
	struct common_tuple_access<std::pair<T1, T2>> {
		using type = std::variant<std::reference_wrapper<T1>, std::reference_wrapper<T2>>;
	};

	template <typename T, auto N>
	struct common_tuple_access<std::array<T, N>> {
		using type = std::variant<std::reference_wrapper<T>>;
	};

	template <typename Tup>
	using common_tuple_access_t = typename common_tuple_access<Tup>::type;

	template <typename Tup>
	auto runtime_get(Tup& t, std::size_t i) {
		return call_access_function<common_tuple_access_t<Tup>>(
			t, i,
			[](auto& element) { return std::ref(element); },
			std::make_index_sequence<std::tuple_size_v<Tup>>{}
		);
	}

	template <typename Tup> class tuple_iterator {
		Tup& t;
		size_t i;
	public:
		tuple_iterator(Tup& tup, size_t idx)
			: t{ tup }, i{ idx }
		{}

		tuple_iterator& operator++() {
			++i; return *this;
		}
		bool operator==(tuple_iterator const& other) const {
			return std::addressof(other.t) == std::addressof(t)
				&& other.i == i;
		}

		bool operator!=(tuple_iterator const& other) const {
			return !(*this == other);
		}

		auto operator*() const {
			return runtime_get(t, i);
		}
	};

	template <typename Tup>
	class to_range {
		Tup& t;
	public:
		to_range(Tup& tup) : t{ tup } {}
		auto begin() {
			return tuple_iterator{ t, 0 };
		}
		auto end() {
			return tuple_iterator{ t, std::tuple_size_v<Tup> };
		}

		auto operator[](std::size_t i) {
			return runtime_get(t, i);
		}
	};

	template <class ... Fs>
	struct overload : Fs... {
		overload(Fs&&... fs) : Fs{ fs }... {}
		using Fs::operator()...;
	};

	template <class ... Fs>
	struct overload_unref : overload<Fs...> {
		overload_unref(Fs&&... fs)
			: overload<Fs...>{ std::forward<Fs>(fs)... }
		{}

		using overload<Fs...>::operator();

		template <class T>
		auto operator()(std::reference_wrapper<T> rw) {
			return (*this)(rw.get());
		}
	};

	// https://github.com/manuelgustavo/cx_hash
	constexpr size_t cx_hash(const char* input)
	{
		size_t hash = sizeof(size_t) == 8 ? 0xcbf29ce484222325 : 0x811c9dc5;
		const size_t prime = sizeof(size_t) == 8 ? 0x00000100000001b3 : 0x01000193;

		while (*input) {
			hash ^= static_cast<size_t>(*input);
			hash *= prime;
			++input;
		}

		return hash;
	}

	struct meta_type
	{
		const char* name;
		size_t typeID;
	};

	template <typename T, typename M, size_t I>
	struct meta_data
	{
		using type = T;
		using data_type = M;

		const char* name;
		M T::* data;
	};

	template <typename T>
	constexpr const char* get_meta_type_name()
	{
		return T::_metaType.name;
	}

	template <typename T>
	constexpr const char* get_meta_type_ID()
	{
		return T::_metaType.typeID;
	}

	template <typename T>
	constexpr auto& get_meta_data()
	{
		return T::_metaData;
	}
}

#define REFLITE_START(X) \
using self = X; \
inline static constexpr reflite::meta_type _metaType = { #X, reflite::cx_hash(#X) }; \
inline static std::tuple _metaData = {

#define REFLITE_ADD(MEMBER) \
reflite::meta_data<self, decltype(self::MEMBER), reflite::cx_hash(#MEMBER)> { #MEMBER, &self::MEMBER },

#define REFLITE_END \
};

#define REFLITE_VISIT_START(TYPE, ELEMENT) \
for (auto const& _reflite_elem : reflite::to_range(reflite::get_meta_data<TYPE>())) \
{ \
	std::visit \
	( \
		reflite::overload_unref \
		( \
			[&](auto const& ELEMENT) \
			{ 

#define REFLITE_VISIT_END \
} \
			), \
			_reflite_elem \
		); \
	} 

#endif
