#pragma once

IP_NAMESPACE_START

struct PixelImage;

enum class FilterType
{
	Grayscale,
	Invert,
	Sepia,
	Brightness
};

class ImageFilter
{
public:
	virtual ~ImageFilter() = default;
	virtual void Apply(PixelImage& image) const = 0;
};

class ImageFilterFactory final
{
public:
	[[nodiscard]] static std::unique_ptr<ImageFilter> Create(FilterType type, float amount = FILTER_BRIGHTNESS_AMOUNT);
};

IP_NAMESPACE_END
