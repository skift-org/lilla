export module Kiss.Base:traits;

namespace Kiss {

export template <typename T>
concept Unsigned = __is_unsigned(T);

export template <typename T>
concept Signed = __is_signed(T);

} // namespace Kiss
